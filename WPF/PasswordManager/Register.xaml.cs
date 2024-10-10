using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security;
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
    /// Interaction logic for Register.xaml
    /// </summary>
    public partial class Register : Page
    {
        private DatabaseHelper dbHelper;

        public Register()
        {
            InitializeComponent();

            dbHelper = new DatabaseHelper();
        }

        //Go back a page
        private void Back_Click(object sender, RoutedEventArgs e)
        {
            if (NavigationService.CanGoBack)
            {
                NavigationService.GoBack();
            }
        }
        private void SubmitButton_Click(object sender, RoutedEventArgs e)
        {
            //Verify user's password confirmation matches
            if (!SecureStringCompare(txtPass.SecurePassword, txtPassConfirm.SecurePassword))
            {
                MessageBox.Show("Passwords do not match, try again.",
                "Error",
                MessageBoxButton.OK,
                MessageBoxImage.Error);

                txtPass.Clear();
                txtPassConfirm.Clear();

                return;
            }

            //Query database to ensure no user has the email already
            string query = "SELECT * FROM users WHERE email = @Email";

            var parameters = new Dictionary<string, object>
            {
                { "@Email", txtEmail.Text } 
            };

            DataTable output = dbHelper.ExecuteQuery(query, parameters);

            if (output.Rows.Count == 0)
            {
                string password = HashPassword(txtPass.SecurePassword);

                //Insert user into database
                query = "INSERT INTO users (email, pass) VALUES (@Email, @Password)";

                parameters = new Dictionary<string, object>
                {
                    { "@Email", txtEmail.Text },
                    { "@Password", password}
                };

                dbHelper.ExecuteQuery(query, parameters);

                MessageBox.Show("You have successfully registered!", "Congrats!", MessageBoxButton.OK, MessageBoxImage.Information);

                var nextPage = new Login();
                this.NavigationService.Navigate(nextPage);
            }
            else
            {
                MessageBox.Show("A user with this email already exists, try again.",
                "Error",
                MessageBoxButton.OK,
                MessageBoxImage.Error);

                txtEmail.Clear();
                txtPass.Clear();
                txtPassConfirm.Clear();
            }

        }

        // Convert secure to plain while limiting exposure
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

        // Hash password while limiting exposure through memory
        public string HashPassword(SecureString securePassword)
        {
            string plainPassword = ConvertSecureStringToString(securePassword);

            try
            {
                return BCrypt.Net.BCrypt.HashPassword(plainPassword);
            }
            finally
            {
                plainPassword = null;
            }
        }

        //Compare secure strings without converting to a plain string
        private bool SecureStringCompare(SecureString s1, SecureString s2)
        {
            if (s1 == null || s2 == null)
                return false;

            if (s1.Length != s2.Length)
                return false;

            IntPtr ptr1 = IntPtr.Zero;
            IntPtr ptr2 = IntPtr.Zero;

            try
            {
                ptr1 = Marshal.SecureStringToBSTR(s1);
                ptr2 = Marshal.SecureStringToBSTR(s2);

                int length1 = Marshal.ReadInt32(ptr1, -4); 
                int length2 = Marshal.ReadInt32(ptr2, -4);

                if (length1 != length2)
                    return false;

                for (int i = 0; i < length1; i++)
                {
                    byte b1 = Marshal.ReadByte(ptr1, i);
                    byte b2 = Marshal.ReadByte(ptr2, i);

                    if (b1 != b2)
                        return false;
                }

                return true;
            }
            finally
            {
                if (ptr1 != IntPtr.Zero)
                    Marshal.ZeroFreeBSTR(ptr1);

                if (ptr2 != IntPtr.Zero)
                    Marshal.ZeroFreeBSTR(ptr2);
            }
        }
    }
}
