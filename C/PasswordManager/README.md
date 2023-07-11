# Password Manager
This is a password manager that utilizes SHA512, AES256, and sqlite3 to encrypt information, store that information into a database, and decrypt the information in the database. In order to access your password manager, you need to first create a master key. This key will be padded with a random salt and stored as a SHA512 hash. After successfully entering your master key, you can now access your password manager through commands. The following commands are supported: 
````
help : View available commands
create : Store a new entry
view [ENTRY] : View username and password for an entry
list : Lists all entries
update [ENTRY] : Update existing username and password
delete [ENTRY] : Delete existing username and password
exit : Exit password manager
````
Each entry is stored using AES256 encryption by encrypting with the master key. 
# Instructions for use
````
make
````
# Credits
#### Parker Hendry
#### hendryp2@winthrop.edu

