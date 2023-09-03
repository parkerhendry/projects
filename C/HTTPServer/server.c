//Parker Hendry
//September 3, 2023

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt))) 
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	//Bind socket to port
	if (bind(server_fd, (struct sockaddr*)&address,sizeof(address))< 0) 
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	//Listen for incoming connections
	if (listen(server_fd, 3) < 0) 
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	
	printf("Server listening on port 8080.\n");
	
	while (1)
	{
	
	//Accept new client connection
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))< 0) 
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	
	//Read GET request
	valread = recv(new_socket, buffer, 1024, 0);
	
	//Parse GET request
	char *foo = strchr(buffer, '\n');
	
	*foo = '\0';
	
	char fileName[32];
	char response[8192];
	char html[4096] = { 0 };
	
	//If file is included, load file
	if (strlen(buffer) > 15)
	{		
		int i = 0;
		int j = 5;
		
		//Read filename
		while (buffer[j] != ' ')
		{
			fileName[i] = buffer[j];
			
			i++;
			j++;		
		}
		
		fileName[i] = '\0';
		
		//Attempt to open file
		FILE *file = fopen(fileName, "r");
		
		//If file cannot be opened, send 404 error
		//Else read file and put in response
		if (file == NULL)
		{
			snprintf(response, 8192,
                	"HTTP/1.1 404 Not Found\r\n"
                	"Content-Type: text/plain\r\n"
                	"\r\n"
                	"404 Not Found");	
		}
		else
		{
			char ch;
		
			while (1)
			{
			
				ch = fgetc(file);
				
				if (ch == EOF)
					break;
				
				strncat(html, &ch, 1);
			
			} 	
			
			snprintf(response, 8192,
                 	"HTTP/1.1 200 OK\r\n"
                 	"Content-Type: text/html\r\n"
                 	"\r\n");
                 	
                 	strcat(response, html);	
                 	
                 	fclose(file);						
		}	
		
	}
	else
	{
		//Welcome response if file is not included
		 snprintf(response, 8192,
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "Welcome to Parker's Webserver!");	
	}
	
	//Send response
	send(new_socket, response, strlen(response), 0);	

	// closing the connected socket
	close(new_socket);
	
	memset(response, 0, sizeof(response));
	memset(buffer, 0, sizeof(buffer));
	memset(html, 0, sizeof(html));
	memset(fileName, 0, sizeof(fileName));
	
	}
	
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}

