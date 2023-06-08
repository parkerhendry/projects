# Custom Shell
This program is a implementation of a custom shell. This shell supports a list of custom commands and also supports BASH commands. This shell also catches interrupt signals and records command history. When the shell is quit, the command history is printed. When the command typed is not supported by the shell, a BASH command will be attempted. The current working directory is always recorded in the command prompt. The "hiMom" command is unique in that it creates a child process and communicates to it via a pipe. 
# Supported Commands 
`````
myprocess: Print current process ID
allprocesses: List all processes
chgd <directory>: Change directory
clr: Clear the screen
dir <directory>: List directory contents
environ: List environment strings
quit: Quit the shell
help: Display help
repeat <string>: Echo's string (also supports file redirection via > symbol followed by file)
hiMom: Creates a child process using fork and communicates to the parent via pipe.
`````
# Instructions for use
`````
make
./shell
`````
# Credits
#### Parker Hendry
#### hendryp2@winthrop.edu
