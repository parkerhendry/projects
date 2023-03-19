//Parker Hendry

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <csignal>
#include <algorithm>
#include <sys/wait.h>
using namespace std;

const char *bar;

//Singal function to catch interrupts
void signalHandler(int signum) 
{
	cout << "Interrupt signal (" << signum << ") received.\n";
   
	cout << "***Command history***: " << endl;
			
	system(bar);
   
	exit(signum);  
}

//Function to create pipe and then forks to create a child process
void hiMom()
{
	int pipefd[2];		//create pipe
	char result[9];		
	
    	if (pipe(pipefd) == -1) 
    	{
        	perror("pipe");
        	exit(EXIT_FAILURE);
    	}
    
    	int pid = fork();	//create child process
    
    	if (pid == -1) 
    	{
        	perror("fork");
        	exit(EXIT_FAILURE);
    	}
			
	if (pid == 0)	//Child process
	{	
		close(pipefd[1]); 		//close write end of pipe
		read(pipefd[0], result, 9);	//read from pipe
		close(pipefd[0]);		//close read end of pipe
				
		cout << "Child received the message '" << result << "'.\n";
		
		exit(0);	//Exit child process
				 
	}
	else		//Parent process
	{
		char message[9] = "Hi Mom!";
		
		close(pipefd[0]);		//close read end of pipe
		write(pipefd[1], message, 9);	//write to pipe
		close(pipefd[1]);		//close write end of pipe
		
		cout << "Generating message '" << message << "' in parent and sending to child...\n";
		
		wait(NULL);			//wait for child
	}
}

int main()
{
	//Function to catch interrupts
	signal(SIGINT, signalHandler);
	
	//Print current working directory and get command from user
	char ch[100];
	cout << "[" << getcwd(ch, 100) << "] " << "Enter a command: ";	
	string line;	
	getline(cin, line);

	//Create history file and history file path	
	string historys = "history.txt";	
	string path = "cat ";	
	path.append(getcwd(ch, 100));
	path.append("/");
	path.append(historys);	
	bar = path.c_str();	
	fstream history;	
	history.open(historys, ios::out);	
		
	//While user does not enter 'quit' or give interrupt
	while (1)
	{
		//Add command to history file
		history << line << endl;
	
		string first, second;
	
		stringstream s(line);
	
		s >> first;
	
		if (first == "allprocesses")
		{
			//List all processes
		
			system("ps");
		}
		else if (first == "myprocess")
		{
			//List current process ID number
		
			cout << getpid() << endl;
		}
		else if (first == "chgd")
		{
			//Change directory
		
			s >> second;
			
			const char *foo = second.c_str();
		
			chdir(foo);				
			
			cout << "Now in directory: " << getcwd(ch, 100) << endl;
				
		}
		else if (first == "dir")
		{
			//Print contents of directory
		
			s >> second;
			
			string change = "ls -al ";
			
			change.append(second);
			
			const char *foo = change.c_str();
			
			system(foo);
		}
		else if (first == "clr")
		{
			//Clear screen
		
			system("clear");
		}
		else if (first == "environ")
		{
			//Print all environmental variables
		
			char **foo = environ;
			
			while (*foo)
			{
				cout << *foo++ << endl;
			}	
		}
		else if (first == "quit")
		{
			//Close history file, print command history, exit
		
			history.close();			
			cout << "***Command history***: " << endl;			
			system(bar);		
			exit(0);	
		}
		else if (first == "help")
		{
			//Print help
		
			cout << "***Supported commands***: \n";
			cout << "myprocess:\t\tPrint current process ID\n";
			cout << "allprocesses:\t\tList all processes\n";
			cout << "chgd <directory>:\tChange directory\n";
			cout << "clr:\t\t\tClear the screen\n";
			cout << "dir <directory>:\tList directory contents\n";
			cout << "environ:\t\tList environment strings\n";		
			cout << "quit:\t\t\tQuit the shell\n";
			cout << "help:\t\t\tDisplay help\n";
			cout << "repeat <string>:\tEcho's string (also supports file redirection via > symbol followed by file)\n";
			cout << "hiMom:\t\t\tCreates a child process using fork and communicates to the parent via pipe.\n";
		}
		else if (first == "repeat")
		{
			//Echo string from input
			//Also supports file redirection
		
			string full;
			
			s >> second;
			
			//Remove double quotes
			second.erase(remove(second.begin(), second.end(), '"'), second.end());
			
			//Append string until closing double quotes
			while (second.find("\"") == string::npos)
			{
				full.append(second);
				
				full.append(" ");
			
				s >> second;
			}
			
			//Remove closing double quotes
			second.erase(remove(second.begin(), second.end(), '"'), second.end());
			
			full.append(second);
			
			s >> second;
			
			
			//If input contains >, redirect to file
			if (second == ">")
			{	
				s >> second;
				
				fstream file;
				
				file.open(second, ios::out);
				
				file << full << endl;	
			}
			else
			{
				cout << full << endl;
			}
				
		}
		else if (first == "hiMom")
		{
			//Call hiMom() function
			hiMom();
		}
		else
		{
			//Else attempt bash command		
			const char *foo = line.c_str();			
			system(foo);
		}
		
		//Print current working directory and get command from user		
		cout << "[" << getcwd(ch, 100) << "] " << "Enter a command: ";		
		getline(cin, line);
	
	}

}
