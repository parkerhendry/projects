# Parker's Password Manager
Welcome to my password manager! This is a full stack (Xaml, C#, MySQL) WPF app that allows users to make an account and store passwords. 

Security is the main priority in this app, as I use bcrypt for master password hashing and AES 256 for stored password encryption, while deriving the AES key from the user's master password. SecureStrings are used throughout the app to avoid exposing the user's passwords in plain text. Whenever exposing passwords in plain text cannot be avoided, the plain text is immediately cleared from memory after completing the necessary task. After logging in, the user is presented with a datagrid of their password entries. Passwords are not presented directly to the user here. Instead, each row in the datagrid consists of a "copy" button which allows the user to copy the password to their clipboard. This minimizes the exposure of each password, as the plain text of each is immediately cleared from memory in the same event (button click). Parameterized SQL queries and non-queries were used to prevent injections.

This app contains add, update, and delete features for password entries. 

# Credits
#### Parker Hendry
#### parkerhendry1@gmail.com
