﻿using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Printing.IndexedProperties;
using System.Runtime.InteropServices;
using System.Security;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Foo
{
    /// <summary>
    /// Interaction logic for Add.xaml
    /// </summary>
    public partial class Add : Page
    {
        private DatabaseHelper dbHelper;
        private SecureString userPassword;
        private string email;

        public Add(SecureString bar, string baz)
        {
            InitializeComponent();

            userPassword = bar;

            email = baz;

            dbHelper = new DatabaseHelper();
        }

        //Go back to start
        private void Back_Click(object sender, RoutedEventArgs e)
        {
            if (NavigationService.CanGoBack)
            {
                NavigationService.GoBack();
            }
        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            //Validate input
            if (string.IsNullOrWhiteSpace(txtWebsite.Text))
            {
                MessageBox.Show("You did not enter a website.",
                    "Error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);

                return;
            }
            else if (string.IsNullOrWhiteSpace(txtUsername.Text))
            {
                MessageBox.Show("You did not enter a username.",
                    "Error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);

                return;
            }
            else if (txtPassword.SecurePassword.Length == 0)
            {
                MessageBox.Show("You did not enter a password.",
                    "Error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);

                return;
            }

            //Generate salt, derive key from master password, use key to encrypt password
            byte[] salt = GenerateSalt();

            byte[] key = DeriveKey(userPassword, salt);

            (byte[] encryptedPass, byte[] IV) = EncryptData(txtPassword.SecurePassword, key);

            //Insert entry into database along with salt and iv for decryption
            string query = $"INSERT INTO entries (email, website, password, salt, username, iv) VALUES (@Email, @Website, @EncryptedPass, @Salt, @Username, @IV)";

            var parameters = new Dictionary<string, object>
            {
                { "@Email", email },
                { "@Website", txtWebsite.Text },
                { "@EncryptedPass", encryptedPass },
                { "@Salt", salt },
                { "@Username", txtUsername.Text },
                { "@IV", IV}
            };

            int output = dbHelper.ExecuteNonQuery(query, parameters);

            if (output > 0)
            {
                MessageBox.Show($"{txtWebsite.Text} was successfully added!", "Congrats!", MessageBoxButton.OK, MessageBoxImage.Information);

                var nextPage = new Start(userPassword, email);
                this.NavigationService.Navigate(nextPage);
            }
            else
            {
                MessageBox.Show($"{txtWebsite.Text} was not successfully added.",
                    "Error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);
            }
        }

        //RNG salt 
        private byte[] GenerateSalt(int size = 16) 
        {
            byte[] salt = new byte[size];
            RandomNumberGenerator.Fill(salt);
            return salt;
        }

        //Derive key from master password while limiting exposure
        private byte[] DeriveKey(SecureString masterPassword, byte[] salt, int iterations = 100000, int keyLength = 32)
        {
            IntPtr passwordPtr = IntPtr.Zero;
            byte[] passwordBytes = null;

            try
            {
                passwordPtr = Marshal.SecureStringToBSTR(masterPassword);

                passwordBytes = Encoding.UTF8.GetBytes(Marshal.PtrToStringBSTR(passwordPtr));

                using (var pbkdf2 = new Rfc2898DeriveBytes(passwordBytes, salt, iterations, HashAlgorithmName.SHA256))
                {
                    return pbkdf2.GetBytes(keyLength);
                }
            }
            finally
            {
                Marshal.ZeroFreeBSTR(passwordPtr);
                if (passwordBytes != null)
                    Array.Clear(passwordBytes, 0, passwordBytes.Length); 
            }
        }

        //Encrypt password while limiting exposure
        private (byte[] encryptedData, byte[] iv) EncryptData(SecureString plainTextSecure, byte[] key)
        {
            IntPtr plainTextPtr = IntPtr.Zero;
            byte[] plainTextBytes = null;

            try
            {
                plainTextPtr = Marshal.SecureStringToBSTR(plainTextSecure);
                plainTextBytes = Encoding.UTF8.GetBytes(Marshal.PtrToStringBSTR(plainTextPtr));

                using (Aes aes = Aes.Create())
                {
                    aes.Key = key;
                    aes.GenerateIV(); 

                    using (MemoryStream ms = new MemoryStream())
                    {
                        using (CryptoStream cs = new CryptoStream(ms, aes.CreateEncryptor(), CryptoStreamMode.Write))
                        {
                            cs.Write(plainTextBytes, 0, plainTextBytes.Length);
                        }
                        return (ms.ToArray(), aes.IV);
                    }
                }
            }
            finally
            {
                Marshal.ZeroFreeBSTR(plainTextPtr);

                if (plainTextBytes != null)
                {
                    Array.Clear(plainTextBytes, 0, plainTextBytes.Length);
                }
            }
        }
    }
}
