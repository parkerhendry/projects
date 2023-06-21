# P2P
Welcome to my P2P application! This app uses socket programming to create a P2P node (it acts as a server and a client using multithreading) which can connect to other nodes using their IP address, and vice-versa. This app uses pthreads to create two different threads for server and client. When connected to a node, the client can invoke shell commands, share files, and change directories. Most commands are supported. This app also introduces a new command:
```
download file.txt
````
In this case, file.txt represents a file from a node that you are currently connected to and would like to download it to your own machine.

PS: IF you would like your correct IP address to be shown, please change line 245 to your correct network interface. The application will work fine without this, but your IP address will be printed as 0.0.0.0.
# Instructions for use
````
make
````
# Credits
#### Parker Hendry
#### hendryp2@winthrop.edu
