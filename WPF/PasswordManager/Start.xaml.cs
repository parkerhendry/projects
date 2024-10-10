using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.IO;
using System.Linq;
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
    /// Interaction logic for Start.xaml
    /// </summary>
    /// 

    public class GridData
    {
        public string Website { get; set; }
        public string Username { get; set; }
        public SecureString Password { get; set; }
    }

    public partial class Start : Page
    {
        private DatabaseHelper dbHelper;
        private SecureString userPassword;
        private string email;

        public Start(SecureString bar, string baz)
        {
            InitializeComponent();

            userPassword = bar;

            email = baz;

            dbHelper = new DatabaseHelper();

            //Query database for user's passwords
            string query = "SELECT * FROM entries WHERE email = @Email";

            var parameters = new Dictionary<string, object>
            {
                { "@Email", email }
            };

            DataTable output = dbHelper.ExecuteQuery(query, parameters);

            ObservableCollection<GridData> list = new ObservableCollection<GridData>();

            //Decrypt each password for the user
            foreach (DataRow row in output.Rows)
            {
                byte[] password = (byte[])row["password"];
                byte[] salt = (byte[])row["salt"];
                byte[] iv = (byte[])row["iv"];

                byte[] key = DeriveKey(userPassword, salt);

                string username = row["username"].ToString();
                string website = row["website"].ToString();

                SecureString securePassword = DecryptDataToSecureString(password, iv, key);

                list.Add(new GridData { Website = website, Username = username, Password = securePassword });
            }

            //Display user entries in a datagrid
            dgPasswords.ItemsSource = list;
        }

        //Convert passwords to plain text when user presses copy button, while limiting exposure
        private void CopyPasswordButton_Click(object sender, RoutedEventArgs e)
        {
            Button button = sender as Button;

            if (button != null && button.Tag is SecureString securePassword)
            {
                string plainPassword = ConvertSecureStringToString(securePassword);
                try
                {
                    Clipboard.SetText(plainPassword); 
                    MessageBox.Show("Password copied to clipboard!", "Info", MessageBoxButton.OK, MessageBoxImage.Information);
                }
                finally
                {
                    plainPassword = null;
                }
            }
        }

        //Convert to plain string while limiting exposure
        private string ConvertSecureStringToString(SecureString secureString)
        {
            IntPtr unmanagedString = IntPtr.Zero;
            try
            {
                unmanagedString = Marshal.SecureStringToBSTR(secureString);
                return Marshal.PtrToStringBSTR(unmanagedString); 
            }
            finally
            {
                Marshal.ZeroFreeBSTR(unmanagedString);
            }
        }

        //Decrypt user's passwords while limiting exposure
        private SecureString DecryptDataToSecureString(byte[] encryptedData, byte[] iv, byte[] key)
        {
            using (Aes aes = Aes.Create())
            {
                aes.Key = key;
                aes.IV = iv;

                using (MemoryStream ms = new MemoryStream(encryptedData))
                {
                    using (CryptoStream cs = new CryptoStream(ms, aes.CreateDecryptor(), CryptoStreamMode.Read))
                    {
                        SecureString secureString = new SecureString();
                        int decryptedChar;

                        while ((decryptedChar = cs.ReadByte()) != -1)
                        {
                            secureString.AppendChar((char)decryptedChar);
                        }
                        secureString.MakeReadOnly();
                        return secureString;
                    }
                }
            }
        }

        //Derive key without converting master password to plain string, while also limiting exposure
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

        //Go to add page
        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            var nextPage = new Add(userPassword, email);
            this.NavigationService.Navigate(nextPage);
        }

        //Return to home
        private void LogoutButton_Click(object sender, RoutedEventArgs e)
        {

            MessageBox.Show("Thanks for using my password manager!", "Thank You!", MessageBoxButton.OK, MessageBoxImage.Information);

            var nextPage = new Home();
            this.NavigationService.Navigate(nextPage);
        }

        //Go to remove page
        private void RemoveButton_Click(object sender, RoutedEventArgs e)
        {
            var nextPage = new Remove(userPassword, email);
            this.NavigationService.Navigate(nextPage);
        }

        //Go to update page
        private void UpdateButton_Click(object sender, RoutedEventArgs e)
        {
            var nextPage = new Update(userPassword, email);
            this.NavigationService.Navigate(nextPage);
        }
    }
}
