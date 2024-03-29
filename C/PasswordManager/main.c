//Parker Hendry
//July 2023

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
#include <string.h>

//Convert hex to int value
int hex_to_int(char c)
{
    if (c >= 97)
        c = c - 32;
        
    int first = c / 16 - 3;
    
    int second = c % 16;
    
    int result = first * 10 + second;
    
    if (result > 9) result--;
    
    return result;
}

//Convert hex to ascii value
int hex_to_ascii(char c, char d)
{
        int high = hex_to_int(c) * 16;
        
        int low = hex_to_int(d);
        
  	return (high + low);
}

//Count how many entries in master key table
//Put number of entries in data
int count(void *data, int argc, char **argv, char **azColName)
{
	int bar = atoi(argv[0]);
	
	int *foo = (int*)data;
	
	if (bar == 0)
	{
		*foo = 1;
	}
	else
	{
		*foo = 0;
	}
	
	return 0;
}

//Retrieve salt for master key from database
int getSalt(void *data, int argc, char **argv, char **azColName)
{
	char *foo = (char*)data;
	
	for (int i = 0; i < 64; i++)
	{
		foo[i] = argv[1][i];
	}
	
	return 0;	
}

//Retrieve hash for master key from database
int getHash(void *data, int argc, char **argv, char **azColName)
{
	char *foo = (char*)data;
	
	for (int i = 0; i < 129; i++)
	{
		foo[i] = argv[0][i];
	}
	
	return 0;
}

//List saved entries from database
int getSaved(void *data, int argc, char **argv, char **azColName)
{
	printf("%s\n", argv[0]);
	
	return 0;
}

int viewSaved(void *data, int argc, char **argv, char **azColName)
{	
	char *foo = (char*)data;

	AES_KEY dec_key;
	unsigned char dec_out[256];
	char new_out[256];

	char fun = 0;
	char fred[2];
	fred[1] = '\0';
	
	printf("Entry name: %s\n", argv[0]);
				
	memset(new_out, 0, sizeof(new_out));
			
	//Convert hex to ascii 	
	for (int i = 0; i < strlen(argv[1]); i++)
	{
		if (i % 2 != 0)
		{
			fred[0] = (char)hex_to_ascii(fun, argv[1][i]);
			strcat(new_out, fred);
		}	
		else
			fun = argv[1][i];
	}
	
	AES_set_decrypt_key(foo, 256, &dec_key);
    	
    	//Decrypt username			
	for (int i = 0; i < strlen(new_out); i += 16)
	{
    		AES_decrypt(new_out + i, dec_out + i, &dec_key); 
    	}
    				
    	printf("Username: %s\n", dec_out); 
    	
    	memset(new_out, 0, sizeof(new_out));
    	
    	//Convert hex to ascii
    	for (int i = 0; i < strlen(argv[2]); i++)
	{
		if (i % 2 != 0)
		{
			fred[0] = (char)hex_to_ascii(fun, argv[2][i]);
			strcat(new_out, fred);
		}	
		else
			fun = argv[2][i];
	}
	
	AES_set_decrypt_key(foo, 256, &dec_key);
    	
    	//Decrypt password			
	for (int i = 0; i < strlen(new_out); i += 16)
	{
    		AES_decrypt(new_out + i, dec_out + i, &dec_key); 
    	}
    	
    	printf("Password: %s\n", dec_out);
    		
    	return 0;
}

int main()
{
	sqlite3 *db;	
	int rc;
	char sql[1024];
	int data;
	
	rc = sqlite3_open("bamboo.db", &db);
	
	//Create table for master key (bamboo)
	strcpy(sql, "CREATE TABLE IF NOT EXISTS bamboo (cypher CHAR(129), salt CHAR(129));"); 
	
	rc = sqlite3_exec(db, sql, NULL, 0, NULL);
	
	//Count how many entries are in master key database
	strcpy(sql, "SELECT COUNT(*) FROM bamboo");
	
	rc = sqlite3_exec(db, sql, count, (void*)&data, NULL);
	
	char master[128];
	SHA512_CTX ctx;
	unsigned char buffer[64];
	char salt[32];
	char saltHash[129];
	char hash[129];
	int fail = 0;
   
   	//Data = 1 if no master key
   	if (data == 1)
   	{
   		printf("You do not have a master key yet! Please create one.\n");
   		printf("Create Master Key: ");
   		
   		fgets(master, sizeof(master), stdin);  		
   		master[strcspn(master, "\n")] = 0;
   		
   		//Generate random salt
		RAND_bytes(salt, 32);
		
		int i = 0;
		int j = 0;
		
		//Convert salt to hex
		while (i < 32)
		{
			sprintf((char*)(saltHash + j), "%02x", salt[i]);
			i++;
			j += 2;
		}
		saltHash[j++] = '\0';
   		
   		//Concat salt to end of master key
   		strncat(master, saltHash, strlen(saltHash));
   		
   		//Generate hash for master + salt
		SHA512_Init(&ctx);
		SHA512_Update(&ctx, (unsigned char *)master, strlen(master));
		SHA512_Final(buffer, &ctx);
		
		i = 0;
		j = 0;
		
		//Convert hash to hex
		while (i < 64)
		{
			sprintf((char*)(hash + j), "%02x", buffer[i]);
			i++;
			j += 2;
		}
		hash[j++] = '\0';
			
		//Insert master key into master key database with hash and salt			
		strcpy(sql, "INSERT INTO bamboo (cypher, salt) VALUES ('");
		
		strncat(sql, hash, 129);
		
		char *foo = "', '";
		
		strncat(sql, foo, sizeof(foo));
		
		strncat(sql, saltHash, 64);
		
		foo = "' ); ";
		
		strncat(sql, foo, sizeof(foo));
		
		rc = sqlite3_exec(db, sql, NULL, 0, NULL);
		
		printf("Master key created!\n");		
		
   	}
   	else
   	{
   		printf("Enter Master Key: ");
   		
   		fgets(master, sizeof(master), stdin);   		
   		master[strcspn(master, "\n")] = 0;
   		
   		//Retrive salt from master key database
   		strcpy(sql, "SELECT * FROM bamboo");
   		
   		sqlite3_exec(db, sql, getSalt, (void*)saltHash, NULL);
   		
   		//Concat salt to end of master key
   		strncat(master, saltHash, 64);
   		
   		//Generate hash from master + salt
 		SHA512_Init(&ctx);
		SHA512_Update(&ctx, (unsigned char *)master, strlen(master));
		SHA512_Final(buffer, &ctx);
		
		int i = 0;
		int j = 0;
		
		//Convert hash to hex
		while (i < 64)
		{
			sprintf((char*)(hash + j), "%02x", buffer[i]);
			i++;
			j += 2;
		}
		hash[j++] = '\0';
		
		//Retrive hash from master key database
		strcpy(sql, "SELECT * FROM bamboo"); 
		
		unsigned char hash2[129];
		
		sqlite3_exec(db, sql, getHash, (void*)hash2, NULL);
		
		//Compare hashes
		//If not the same, then the entered master key is invalid and the program terminates
		for (int i = 0; i < 129; i++)
		{
			if (hash[i] != hash2[i])
			{
				fail = 1;
				break;
			}			
		}
		
		if (fail == 1)
		{
			printf("Invalid master key!\n");
		}	
		else
		{
			printf("--------------------------\n");
			printf("Authentication successful!\n");
			printf("--------------------------\n");
			printf("Welcome!\n");
			printf("--------------------------\n");
			printf("--------------------------\n");
		}
   	}
   	
   	char command[32];
   	char words[15][15];
   	
   	//If master key is valid
   	if (fail != 1)
   	{
   		//Create table for entries
		strcpy(sql, "CREATE TABLE IF NOT EXISTS frog (name CHAR(64), username CHAR(128), password CHAR(128));"); 
	
		rc = sqlite3_exec(db, sql, NULL, 0, NULL);
		
		printf("Listing saved...\n");
		
		//List already saved entries
		strcpy(sql, "SELECT * FROM frog");
		
		sqlite3_exec(db, sql, getSaved, 0, NULL);      		
   		
   		//Infinite loop for commands
   		while (1)
   		{
   		
   			printf("Enter a command: ");
   			fgets(command, 32, stdin);
   			
   			command[strcspn(command, "\n")] = 0;
   			
   			int ctr = 0, j = 0, dome = 0;
   			
   			memset(words, 0, sizeof(words));
   			
   			//Get first word from command and store the rest in words[1]
   			for (int i = 0; i < strlen(command); i++)
			{
				if ((command[i] == ' '  || command[i] == '\0') && dome == 0)
				{
					ctr++;
					j = 0;
					dome = 1;
				}
				else
				{
					words[ctr][j] = command[i];
					j++;
				}
			}
   			
   			if (strcmp(command, "create") == 0)
   			{
   				/*Create a new entry*/
   			
   				char name[128];
   				char user[128];
   				char pass[128];
   			
   				printf("Entry name: ");
   				fgets(name, sizeof(name), stdin);
   				name[strcspn(name, "\n")] = 0;
   				
   				printf("Username: ");
   				fgets(user, sizeof(user), stdin);
   				user[strcspn(user, "\n")] = 0;
   				
   				printf("Password: ");
   				fgets(pass, sizeof(pass), stdin);
   				pass[strcspn(pass, "\n")] = 0;
   				
   				unsigned char enc_out[128];
   				unsigned char dec_out[128];
    				unsigned char baz[2][256];
    				
    				memset(enc_out, 0, sizeof(enc_out));
    				memset(dec_out, 0, sizeof(dec_out));
    				memset(baz, 0, sizeof(baz));

    				AES_KEY enc_key;
    				
    				AES_set_encrypt_key(master, 256, &enc_key);

				//Encrypt username
				for (int i = 0; i < strlen(user); i += 16)
				{
    					AES_encrypt(user + i, enc_out + i, &enc_key); 
    				}    
    				
    				
				int i = 0;
				int j = 0;
		
				//Convert encryption to hex
				while (i < strlen(enc_out))
				{
					sprintf((char*)(baz[0] + j), "%02x", enc_out[i]);
					i++;
					j += 2;
				}
				baz[0][j++] = '\0';
				
				memset(enc_out, 0, sizeof(enc_out));
				
   				AES_set_encrypt_key(master, 256, &enc_key);

				//Encrypt password
				for (int i = 0; i < strlen(pass); i += 16)
				{
    					AES_encrypt(pass + i, enc_out + i, &enc_key); 
    				}   
    				
				i = 0;
				j = 0;
		
				//Convert encryption to hex
				while (i < strlen(enc_out))
				{
					sprintf((char*)(baz[1] + j), "%02x", enc_out[i]);
					i++;
					j += 2;
				}
				baz[1][j++] = '\0';
				
				//Store encrypted values in database
				strcpy(sql, "INSERT INTO frog (name, username, password) VALUES ('");
				
				strcat(sql, name);
				strcat(sql, "', '");
				strcat(sql, baz[0]);
				strcat(sql, "', '");
				strcat(sql, baz[1]);
				strcat(sql, "' ); ");
				
				rc = sqlite3_exec(db, sql, NULL, 0, NULL);
				
				printf("%s has been added!\n", name);
				
				
				
   			}
   			else if (strcmp(words[0], "view") == 0)
   			{
   			
   				//View username and password for an entry
   				strcpy(sql, "SELECT * FROM frog WHERE name = '");
   				strcat(sql, words[1]);
   				strcat(sql, "';");
   				
   				rc = sqlite3_exec(db, sql, viewSaved, (void*)master, NULL);
   			}
   			else if (strcmp(command, "exit") == 0)
   			{
   				//Exit program
   				printf("Thank you for using my password manager!\n");
   				break;
   			}
   			else if (strcmp(command, "list") == 0)
   			{
   				//List entries by entry name
				strcpy(sql, "SELECT * FROM frog");
		
				sqlite3_exec(db, sql, getSaved, 0, NULL);  				
   			}
   			else if (strcmp(words[0], "update") == 0)
   			{
   				/*Update an existing entry*/
   			
   				char user[128];
   				char pass[128];
   				
   				printf("New Username: ");
   				fgets(user, sizeof(user), stdin);
   				user[strcspn(user, "\n")] = 0;
   				
   				printf("New Password: ");
   				fgets(pass, sizeof(pass), stdin);
   				pass[strcspn(pass, "\n")] = 0;
   				
   				unsigned char enc_out[128];
    				unsigned char baz[2][256];
    				
    				memset(enc_out, 0, sizeof(enc_out));
    				memset(baz, 0, sizeof(baz));

    				AES_KEY enc_key;

   				AES_set_encrypt_key(master, 256, &enc_key);

				//Encrypt username
				for (int i = 0; i < strlen(user); i += 16)
				{
    					AES_encrypt(user + i, enc_out + i, &enc_key); 
    				}   
    				
				int i = 0;
				int j = 0;
		
				//Convert encryption to hex
				while (i < strlen(enc_out))
				{
					sprintf((char*)(baz[0] + j), "%02x", enc_out[i]);
					i++;
					j += 2;
				}
				baz[0][j++] = '\0';
				
				memset(enc_out, 0, sizeof(enc_out));
				
   				AES_set_encrypt_key(master, 256, &enc_key);

				//Encrypt password
				for (int i = 0; i < strlen(pass); i += 16)
				{
    					AES_encrypt(pass + i, enc_out + i, &enc_key); 
    				}  
    				
				i = 0;
				j = 0;
		
				//Convert encryption to hex
				while (i < strlen(enc_out))
				{
					sprintf((char*)(baz[1] + j), "%02x", enc_out[i]);
					i++;
					j += 2;
				}
				baz[1][j++] = '\0';
				
				//Update encypted values
				strcpy(sql, "UPDATE frog SET username = '");
				strcat(sql, baz[0]);
				strcat(sql, "', password = '");
				strcat(sql, baz[1]);
				strcat(sql, "' WHERE name = '");
				strcat(sql, words[1]);
				strcat(sql, "';");
				
				rc = sqlite3_exec(db, sql, NULL, 0, NULL);
				
				printf("%s has been updated!\n", words[1]);
				
   			}
   			else if (strcmp(words[0], "delete") == 0)
   			{
   				//Delete an entry
   				strcpy(sql, "DELETE from frog where name = '");
   				strcat(sql, words[1]);
   				strcat(sql, "';");
   				
   				rc = sqlite3_exec(db, sql, NULL, 0, NULL);
   				
   				printf("%s has been deleted!\n", words[1]);
   			}
   			else if (strcmp(command, "help") == 0)
   			{
   				//List available commands
   				printf("Available commands: \n");
   				printf("create : Store a new entry\n");
   				printf("view [ENTRY] : View username and password for an entry\n");
   				printf("list : Lists all entries\n");
   				printf("update [ENTRY] : Update existing username and password\n");
   				printf("delete [ENTRY] : Delete existing username and password\n");
   				printf("exit : Exit password manager\n");
   			}
   			else
   			{
   				printf("Invalid command! Use command \"help\" for info.\n");
   			}
   			
   		}
   			
   	}
   	
   	
   	sqlite3_close(db);
   	
   	return 0;

   	
   	
}
