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


namespace MCSE_Editor_for_Wii_U
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("msscmp.dll", EntryPoint = "extractMsscmp")]
        public static extern void extractMsscmp([In()] [MarshalAs(UnmanagedType.LPStr)] string path);


        public MainWindow()
        {
            InitializeComponent();
            MouseLeftButtonDown += (sender, e) => this.DragMove();

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
            Window home = new Home();
            home.WindowStartupLocation = WindowStartupLocation.CenterScreen;
            home.ShowDialog();

            Home h = new Home();
            h.Close();

            if (h.openFilePath != null)
            {
                extractMsscmp(h.openFilePath);
            }

        }
    }
}
