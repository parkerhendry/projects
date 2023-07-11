//Parker Hendry
//June 2023

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <pthread.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 65536


struct Buffer
{
	char contents[BUFFER_SIZE];
	int file;
};


void *server()
{

	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	struct Buffer buffer;
	buffer.file = 0;
	char temp[1024] = { 0 };	
	struct ifreq ifr;

	// Create server socket descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Socket created.\n");
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}	

	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);	
	ifr.ifr_addr.sa_family = AF_INET;	
	address.sin_addr.s_addr = INADDR_ANY;

	//Bind socket to port
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) 
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Bind successful.\n");
	}
	
	//Listen for incoming connection
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Listening...\n");
	}
	
	//Accept client connection if received
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Connected to client.\n");
	}
	
	while (1)
	{
		char foo[50];
	
		//Get command from client
		valread = read(new_socket, foo, 50);
		
		char words[15][15];
		
		memset(words, 0, sizeof(words));
		
		int ctr = 0, j = 0;
			
		if (strlen(foo) > 1)
		{
		
		//Split command into words		
		for (int i = 0; i < strlen(foo); i++)
		{
			if (foo[i] == ' '  || foo[i] == '\0')
			{
				ctr++;
				j = 0;
			}
			else
			{
				words[ctr][j] = foo[i];
				j++;
			}
		}
		
		char cd[3] = "cd";
		char *download = "download";
		
		if (strcmp(words[0], cd) == 0)
		{	
			//Client enters cd [PATH]
		
			words[1][strcspn(words[1], "\n")] = 0;
		
			//Change directory to the path entered by client
			chdir(words[1]);
			
			char ch[50];
			
			char bar[50] = "Now in directory ";
			
			strcat(bar, getcwd(ch, 50));
			strcat(bar, "\n");
			
			memset(buffer.contents, 0, BUFFER_SIZE);
			
			//Print new current directory into buffer, which will be sent back to client
			sprintf(buffer.contents, "%s", bar);
			buffer.file = 0;
		}
		else if (strcmp(words[0], download) == 0)
		{
			//Client enters download [FILE]
			
			words[1][strcspn(words[1], "\n")] = 0;
		
			memset(buffer.contents, 0, BUFFER_SIZE);
			buffer.file = 0;
		
			FILE *fp;
			
			fp = fopen(words[1], "r");
			
			//If file can't be opened, print error into buffer
			//Otherwise, print file contents into buffer
			if (fp == NULL)
			{
				sprintf(buffer.contents + strlen(buffer.contents), "%s", "File cannot be opened at this location.\n");
				buffer.file = 0;
			}
			else
			{
			
				while (fgets(temp, 1024, fp) != NULL)
				{
					sprintf(buffer.contents + strlen(buffer.contents), "%s", temp);
				}
				buffer.file = 1;
				
				fclose(fp);
			}
			
		}
		else
		{
		
			//Else attempt shell command with client command
		
			FILE *fp;
		
			fp = popen(foo, "r");
		
			//If shell command can't be executed, print error to buffer
			if (fp == NULL) 
			{
    				printf("Failed to run command\n" );
    				exit(1);
  			}
  		
			memset(buffer.contents, 0, BUFFER_SIZE);
			buffer.file = 0;
		
			//Print output of command to buffer
	 		while (fgets(temp, 1024, fp) != NULL)
			{	
				sprintf(buffer.contents + strlen(buffer.contents), "%s", temp);	
			}
		
		}
		
		}
		
		//Send buffer to client
		send(new_socket, (char *)&buffer, sizeof(buffer), 0);
		
		memset(buffer.contents, 0, BUFFER_SIZE);
		buffer.file = 0;
				
			
	}
	
	// closing the connected socket
	close(new_socket);
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	
}

int main(int argc, char const* argv[])
{
	pthread_t server_thread;     
        
        //Create separate thread to handle server
        if (pthread_create(&server_thread, NULL, server, NULL) != 0) 
        {
            perror("Error in creating server thread.");
            exit(1);
        }

	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	struct Buffer buffer;
	buffer.file = 0;
	char temp[1024] = { 0 };
	struct ifreq ifr;
	
	//Create client socket descriptor
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		printf("\n Socket creation error \n");
		exit(1);
	}
	else
	{
		printf("Socket created.\n");
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	//Print ip of local machine
	ifr.ifr_addr.sa_family = AF_INET;	
	strncpy(ifr.ifr_name, "wlp2s0", IFNAMSIZ - 1);	
	ioctl(client_fd, SIOCGIFADDR, &ifr);	
	char *ipCurrent = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
	
	sleep(1);

	printf("Current IP: %s\n", ipCurrent);	
	
	char ip[12];
	
	//Get ip of server from user
	printf("Please enter the ip address of the client you would like to connect to.\n");
	scanf("%s", ip);

	//Convert ip address entered by user to binary
	while (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) 
	{	
		printf("Invalid address. Please try again.\n");
		scanf("%s", ip);		
	}
	
	//Connect to server 
	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) 
	{
		printf("\nConnection Failed \n");
		exit(1);
	}
	else
	{
		printf("Connected to server.\n");
	}
	
	
	printf("You are now connected!\n");

	
	while (1)
	{
	
		printf("Enter a command: ");
	
		char foo[50];
		char ch[30];
	
		//Get command from user
		fgets(foo, 30, stdin);
	
		//Send that command to server
		send(client_fd, foo, 50, 0);
		
		//Wait for server
		sleep(1);

		//Read response from server
		valread = read(client_fd, (char *)&buffer, sizeof(buffer));
		
		if (buffer.file == 1)
		{
			//If file downloaded from server
		
			//Select where to save the file
			printf("Current directory: %s\n", getcwd(ch, 30));
			printf("Enter a path to save the file: ");
			fgets(foo, 30, stdin);
			
			foo[strcspn(foo, "\n")] = 0;
			
			//Move to the directory entered
			chdir(foo);
			
			//Select name of the file to be saved
			printf("Current directory: %s\n", getcwd(ch, 30));			
			printf("Name of the file: ");
			fgets(foo, 30, stdin);
			
			foo[strcspn(foo, "\n")] = 0;
			
			FILE *fp;
			
			//Create file
			fp = fopen(foo, "w");
			
			if (fp == NULL)
			{
				printf("File cannot be saved. This could be due to the permissions of the directory you are trying to save it to.\n");
			}
			else
			{
				//Move buffer contents to file
				fprintf(fp, "%s", buffer.contents);
			
				printf("File saved!\n");
			
				fclose(fp);
			
			}
			
			
		}
		else
		{
			//If file not downloaded from server
	
			//Print output of command
			fputs(buffer.contents, stdout);
			
		
		}
		
		memset(buffer.contents, 0, BUFFER_SIZE);
		buffer.file = 0;
	
	}
	

	// closing the connected socket
	close(client_fd);
        
        //Detach server thread
        if (pthread_detach(server_thread) != 0) {
            perror("Error in detaching server thread.");
            exit(1);
        }


	return 0;
}

