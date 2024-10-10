using System;
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
    /// Interaction logic for Login.xaml
    /// </summary>
    public partial class Login : Page
    {
        private DatabaseHelper dbHelper;
        private SecureString userPassword;

        public Login()
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

            //Get password hash for the given email
            string query = "SELECT pass FROM users WHERE email = @Email";

            var parameters = new Dictionary<string, object>
            {
                { "@Email", txtEmail.Text }
            };

            DataTable output = dbHelper.ExecuteQuery(query, parameters);

            userPassword = txtPass.SecurePassword;

            //Verify user's password while also validating input
            if (output.Rows.Count > 0)
            {
                string password = output.Rows[0][0].ToString();

                if (VerifyPassword(userPassword, password))
                {    
                    var nextPage = new Start(userPassword, txtEmail.Text);
                    this.NavigationService.Navigate(nextPage);

                }
                else
                {
                    MessageBox.Show("Incorrect password, try again.",
                        "Error",
                        MessageBoxButton.OK,
                        MessageBoxImage.Error);

                    txtPass.Clear();
                }
            }
            else
            {
                MessageBox.Show("A user with this email does not exist, try again.",
                    "Error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);

                txtEmail.Clear();
                txtPass.Clear();
            }


        }

        //Convert secure to plain string while limiting exposure
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

        //Verify while limiting exposure through memory
        public bool VerifyPassword(SecureString enteredPassword, string hashedPassword)
        {
            string plainPassword = ConvertSecureStringToString(enteredPassword);

            try
            {
                return BCrypt.Net.BCrypt.Verify(plainPassword, hashedPassword);
            }
            finally
            {
                plainPassword = null;
            }
        }
    }
}
