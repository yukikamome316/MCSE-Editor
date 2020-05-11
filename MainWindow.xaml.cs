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
using System.Collections.ObjectModel;
using System.IO;

namespace MCSE_Editor_for_Wii_U
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("msscmp.dll")]
        extern static int extractMsscmp([In()] [MarshalAs(UnmanagedType.LPStr)] string path);


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

        //Refered to https://stackoverflow.com/questions/21954669/treeview-directories-in-c-sharp-wpf and Syoch
        //Special thanks!
        private void ListDirectory(TreeView treeView, string path)
        {
            treeView.Items.Clear();
            var rootDirectoryInfo = new DirectoryInfo(path);
            treeView.Items.Add(CreateDirectoryNode(rootDirectoryInfo));
        }

        private static TreeViewItem CreateDirectoryNode(DirectoryInfo directoryInfo)
        {
            var directoryNode = new TreeViewItem { Header = directoryInfo.Name };
            foreach (var directory in directoryInfo.GetDirectories())
                directoryNode.Items.Add(CreateDirectoryNode(directory));

            foreach (var file in directoryInfo.GetFiles())
                directoryNode.Items.Add(new TreeViewItem { Header = file.Name });

            return directoryNode;

        }

        private void window_Loaded(object sender, RoutedEventArgs e)
        {

            if (Variables.openFilePath != "")
            {
                if (extractMsscmp(Variables.openFilePath) == 1)
                {
                    MessageBox.Show("ファイルの展開に失敗しました。ファイルが破損してる可能性があります。");
                }
                else //TreeView
                {
                    treeView.Visibility = Visibility.Visible;
                    noOpenText.Visibility = Visibility.Hidden;
                    homeButton.Visibility = Visibility.Hidden;
                    ListDirectory(treeView, @"tmp\Minecraft");
                }

            }
            else
            {
                treeView.Visibility = Visibility.Hidden;
                noOpenText.Visibility = Visibility.Visible;
                homeButton.Visibility = Visibility.Visible;
            }

        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.Application.Restart();
            System.Windows.Application.Current.Shutdown();
        }
    }
}
