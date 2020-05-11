using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
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
using System.Diagnostics;


namespace MCSE_Editor_for_Wii_U
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("msscmp.dll")]
        extern static void extractMsscmp([In()] [MarshalAs(UnmanagedType.LPStr)] string path);


        public MainWindow()
        {
            InitializeComponent();
            MouseLeftButtonDown += (sender, e) => this.DragMove();
            Window home = new Home();
            home.WindowStartupLocation = WindowStartupLocation.CenterScreen;
            home.ShowDialog();

        }
    

        private void Rectangle_DragEnter(object sender, DragEventArgs e)
        {

        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
        }

        private void window_Loaded(object sender, RoutedEventArgs e)
        {
            variables variables = new variables();

            Debug.Print(variables.openFilePath);

            if (variables.openFilePath != "")
            {
                extractMsscmp(variables.openFilePath);
            }

        }
    }
}
