//Parker Hendry
//September 1, 2023

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#define PORT 25

int main()
{
	int status, valread, client_fd;
	
	struct sockaddr_in serv_addr;
	char buffer[1024] = { 0 };
	char foo[1024] = { 0 };
	char from[64] = { 0 };
	char to[64] = { 0 };
	char body[1024] = { 0 };
	struct ifreq ifr;
	
	//Create client socket descriptor
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
        	printf("\n Socket creation error \n");
        	exit(1);
    	}	
    	
    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(PORT);
    	
    	//Print IP address of local machine
    	ifr.ifr_addr.sa_family = AF_INET;	
	strncpy(ifr.ifr_name, "wlp2s0", IFNAMSIZ - 1);	
	ioctl(client_fd, SIOCGIFADDR, &ifr);	
	char *ipCurrent = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

	printf("Current IP: %s\n", ipCurrent);
	
	//Get IP of SMTP server to connect to
	printf("(Winthrop's SMTP server is 10.2.0.56)\n");
	printf("Enter an SMTP server to connect to: ");
	fgets(to, 64, stdin);	
	to[strcspn(to, "\n")] = 0;	
  
    	// Convert IPv4 and IPv6 addresses from text to binary
    	while (inet_pton(AF_INET, to, &serv_addr.sin_addr) <= 0) 
   	{
    	    	printf("Invalid address, please try again.\n");
		printf("Enter an SMTP server to connect to: ");
		fgets(to, 64, stdin);	
		to[strcspn(to, "\n")] = 0;
    	}
    	
    	memset(to, 0, sizeof(to));
  
 	//Connect to server
    	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr))) < 0) 
    	{
        	printf("Connection Failed.\n");
        	exit(1);
    	}
    	
    	//Get response from server
    	valread = read(client_fd, buffer, 1024);
    	printf("%s", buffer);
    	
    	strcpy(foo, "ehlo ");
    	strcat(foo, ipCurrent);
    	strcat(foo, "\n");
    	
    	//Provide identification of sender
   	send(client_fd, foo, strlen(foo), 0);
   	
   	//Get response from server
   	valread = read(client_fd, buffer, 1024);
   	
    	printf("%s", buffer);
   	
   	memset(foo, 0, sizeof(foo));
   	memset(buffer, 0, sizeof(buffer));
    	
    	printf("What is the email to be sent from?\n");
    	fgets(from, 64, stdin);
    	
    	strcpy(foo, "mail from: ");
    	strcat(foo, from);
    	
    	//Specify originator of the email
    	send(client_fd, foo, strlen(foo), 0);
    	
    	//Get response from server
   	valread = read(client_fd, buffer, 1024);
   	
    	printf("%s", buffer);
    	
	memset(foo, 0, sizeof(foo));
	memset(buffer, 0, sizeof(buffer));
	
	printf("What is the email to be sent to?\n");
	fgets(to, 64, stdin);
	
    	strcpy(foo, "rcpt to: ");
    	strcat(foo, to);
    	
    	//Specify recipient of the email
    	send(client_fd, foo, strlen(foo), 0);
    	
    	//Get response from server
   	valread = read(client_fd, buffer, 1024);
   	
    	printf("%s", buffer);
    	
    	memset(foo, 0, sizeof(foo));
    	memset(buffer, 0, sizeof(buffer));
    	
    	strcpy(foo, "data");
    	strcat(foo, "\n");
    	
    	//Specify beginning of email
    	send(client_fd, foo, strlen(foo), 0);
    	
    	//Get response from server
   	valread = read(client_fd, buffer, 1024);
   	
    	printf("%s", buffer);
    	
    	memset(foo, 0, sizeof(foo));
    	memset(buffer, 0, sizeof(buffer));
    	
    	strcpy(foo, "from: ");
    	strcat(foo, from);
    	
    	//Specify the originator of the email
    	send(client_fd, foo, strlen(foo), 0);
    	
    	memset(foo, 0, sizeof(foo));
    	
    	strcpy(foo, "to: ");
    	strcat(foo, to);
    	
    	//Specify the recipient of the email
    	send(client_fd, foo, strlen(foo), 0);
    	    	
    	memset(foo, 0, sizeof(foo));
    	
    	strcpy(foo, "Subject: ");
    	
    	printf("Subject: ");
    	fgets(body, 1024, stdin);
    	
    	strcat(foo, body);
    	strcat(foo, "\n");
    	
    	//Specify subject of the email
    	send(client_fd, foo, strlen(foo), 0);  
    	
    	memset(foo, 0, sizeof(foo)); 
    	memset(body, 0, sizeof(body)); 
    	 
    	printf("(Enter a single '.' (period) to end your body text.)\n");  	
    	printf("Body: \n");
    	
    	while (strcmp(body, ".\n") != 0)
    	{   
    		strcat(foo, body); 
    	 	
    		fgets(body, 1024, stdin);   	
    	}
	
	//Specify body of email
    	send(client_fd, foo, strlen(foo), 0); 
    	
    	memset(foo, 0, sizeof(foo)); 
    	memset(body, 0, sizeof(body));
    	
    	//End body
    	send(client_fd, ".\n", strlen(".\n"), 0);  
    	
    	//Get response from server
   	valread = read(client_fd, buffer, 1024);
   	
    	printf("%s", buffer);
    	
    	memset(buffer, 0, sizeof(buffer));
    	
    	//End connection
    	send(client_fd, "quit\n", strlen("quit\n"), 0);	
    	
    	//Get response from server
   	valread = read(client_fd, buffer, 1024);
   	
    	printf("%s", buffer);	
    	  
    	//Close the connected socket
    	close(client_fd);
    	return 0;
    	
    	
    		
}
