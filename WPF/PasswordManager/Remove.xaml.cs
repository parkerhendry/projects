using System;
using System.Collections.Generic;
using System.Linq;
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
    /// Interaction logic for Remove.xaml
    /// </summary>
    public partial class Remove : Page
    {
        private DatabaseHelper dbHelper;
        private SecureString userPassword;
        private string email;

        public Remove(SecureString bar, string baz)
        {
            InitializeComponent();

            userPassword = bar;

            email = baz;
        }

        private void RemoveButton_Click(object sender, RoutedEventArgs e)
        {
            //Input validation
            if (string.IsNullOrWhiteSpace(txtWebsite.Text))
            {
                MessageBox.Show("You did not enter a website.",
                    "Error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);

                return;
            }

            //Ensure user wants to delete
            MessageBoxResult result = MessageBox.Show($"Are you sure you want to delete the entry for {txtWebsite.Text}?", "Confirmation", MessageBoxButton.YesNo, MessageBoxImage.Question);

            if (result == MessageBoxResult.No)
            {
                return;
            }

            dbHelper = new DatabaseHelper();

            //Delete entry from table
            string query = "DELETE FROM entries WHERE email = @Email AND website = @Website";

            var parameters = new Dictionary<string, object>
            {
                { "@Email", email },
                { "@Website", txtWebsite.Text }
            };

            int output = dbHelper.ExecuteNonQuery(query, parameters);

            if (output > 0)
            {
                MessageBox.Show($"{txtWebsite.Text} was successfully deleted!", "Congrats!", MessageBoxButton.OK, MessageBoxImage.Information);

                var nextPage = new Start(userPassword, email);
                this.NavigationService.Navigate(nextPage);
            }
            else
            {
                MessageBox.Show($"{txtWebsite.Text} does not exist.",
                    "Error",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error);
            }
        }

        //Go back to start
        private void Back_Click(object sender, RoutedEventArgs e)
        {
            if (NavigationService.CanGoBack)
            {
                NavigationService.GoBack();
            }
        }
    }
}
