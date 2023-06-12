//Parker Hendry
//March 7, 2023

#include <iostream>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <iomanip>
using namespace std;

//Function to get permissions of files and directory status
char* permissions(char *file)
{
   struct stat st;
    
   //string to hold permissions and directory status
   char *modeval = (char*)malloc(sizeof(char) * 10 + 1);
   
   //Determine permissions based on stat macros
   if(stat(file, &st) == 0)
   {    
        mode_t perm = st.st_mode;
        
        //Read write execute permissions
        modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
        
        modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
        
        modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
        
        modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
        
        modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
        
        modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
        
        modeval[6] = (perm & S_IROTH) ? 'r' : '-';
        
        modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
        
        modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
        
        modeval[9] = '\0';
        
        //Directory status
        modeval[10] = (perm & S_IFDIR) ? 'd' : ' ';
        
        return modeval;     
    }
    else
    {
    	return modeval;
    }
    
}

int main(int argc, char *argv[])
{
	struct dirent *pDirent;
	DIR *pDir;
	
	char ch[100];
	
	string path;
	
	//Validate command line input
	if (argc > 2)
	{
		cerr << "Please enter either no command line argument or one argument that represents a directory to analyze.\n";
		return -1;
	}
	
	//Determine directory path to open
	if (argc > 1)
	{	
		if (argv[1][0] == '/')
		{
			path = argv[1];
		}
		else
		{
			path = getcwd(ch, 100);
			path.append("/");
			path.append(argv[1]);
		}
			
	}
	else
	{

		path = getcwd(ch, 100);
	
	}
		
	const char *foo = path.c_str();
	
	//Open directory path
	pDir = opendir(foo);
	
	//If directory does not exist, exit
	if (pDir == NULL)
	{
		cerr << "Error: The directory " << path << " cannot be opened.\n";
		return -1;
	}
	
	//Print directory that will be analyzed
	cout << "Listing directory: " << path << endl;
	
	//Read contents of directory
	while ((pDirent = readdir(pDir)) != NULL)
	{
		//Append current file to directory path
		char buffer[1024];		
		strcpy(buffer, foo);		
		strcat(buffer, "/");	
		strcat(buffer, pDirent->d_name);
	
		//Get permissions from permissions function
		char *perm = permissions(buffer);
		
		//Print DIR if directory
		if (perm[10] == 'd')
		{
			cout << "DIR ";
		}
		else
		{
			cout << "    ";
		}			
		
		//Print permissions
		for (int i = 0; i < strlen(perm); i++)
		{
			cout << perm[i];
		}
		
		struct stat st;
		
		int rc = stat(buffer, &st);
		
		struct tm *tm;

		//Turn time since unix epoch to struct that holds specific date-time formats		
		tm = localtime(&st.st_mtime);
				
		string day;
		string month;
		
		//Determine day of the week
		switch (tm->tm_wday)
		{
			case 0:
				day = "Sun";
				break;
			case 1:
				day = "Mon";
				break;
			case 2:
				day = "Tue";
				break;
			case 3:
				day = "Wed";
				break;
			case 4:
				day = "Thu";
				break;
			case 5: 
				day = "Fri";
				break;
			case 6: 
				day = "Sat";
				break;
		}
		
		//Determine month
		switch (tm->tm_mon)
		{
			case 0:
				month = "Jan";
				break;
			case 1: 
				month = "Feb";
				break;
			case 2: 
				month = "Mar";
				break;
			case 3: 
				month = "Apr";
				break;
			case 4: 
				month = "May";
				break;
			case 5: 
				month = "Jun";
				break;
			case 6:
				month = "Jul";
				break;
			case 7: 
				month = "Aug";
				break;
			case 8:
				month = "Sep";
				break;
			case 9: 
				month = "Oct";
				break;
			case 10:
				month = "Nov";
				break;
			case 11: 
				month = "Dec";
				break;
		}
		
		//Create hour, minute, second format
		char buf[1024];		
		strftime(buf, sizeof(buf), "%H:%M:%S", tm);
		
		//Output file information
		cout << "\t";		
		cout << left << setw(50) << pDirent->d_name;		
		cout << left << setw(15) << st.st_size;		
		cout << left << setw(5) << day;		
		cout << left << setw(5) << month;		
		cout << left << setw(5) << tm->tm_mday;		
		cout << left << setw(10) << buf;		
		cout << left << setw(10) << tm->tm_year + 1900;		
		cout << endl;
		
	}
	
	//Close directory path
	closedir(pDir);
}

