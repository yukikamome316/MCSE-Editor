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
        [DllImport("msscmp.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern int extractMsscmp(string path);
        [DllImport("msscmp.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern int replaceEntryMsscmp(string path, string replacePath);
        [DllImport("msscmp.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern int saveMsscmp(string path);



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
            if (!string.IsNullOrEmpty(Variables.openFilePath))
            {
                if (extractMsscmp(Variables.openFilePath) == 1)
                {
                    MessageBox.Show("ファイルの展開に失敗しました。ファイルが破損してる可能性があります。", "MCSE Editor for Wii U"
                        , MessageBoxButton.OK ,MessageBoxImage.Exclamation);

                    treeView.Visibility = Visibility.Hidden;
                    noOpenText.Visibility = Visibility.Visible;
                    homeButton.Visibility = Visibility.Visible;
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
            Application.Current.Shutdown();
        }

        private void homeToolButton_Click(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show("現在の編集データは破棄されますがよろしいですか？", "MCSE Editor for Wii U", MessageBoxButton.YesNo,
                 MessageBoxImage.Information) == MessageBoxResult.No)
            { return; }
            else
            {
                System.Windows.Forms.Application.Restart();
                Application.Current.Shutdown();
            }
            
        }

        private void replaceToolButton_Click(object sender, RoutedEventArgs e)
        {
            


            if (!string.IsNullOrEmpty(Variables.replaceFilePath))
            {
                var isDirectory = File
                .GetAttributes(@"tmp\" + Variables.replaceFilePath)
                .HasFlag(FileAttributes.Directory);

                if (isDirectory == true)
                {
                    MessageBox.Show("ディレクトリが選択されています。置き換え先ファイルを選択してください", "MSSE Changer for Wii U", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return;
                }

                System.Windows.Forms.OpenFileDialog ofd = new System.Windows.Forms.OpenFileDialog();
                ofd.FileName = ".binka";
                ofd.Filter = "binkaファイル(*.binka)|*.binka|すべてのファイル(*.*)|*.*";
                ofd.Title = "置き換え元ファイルを選択してください";
                ofd.RestoreDirectory = true;

                if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    if (replaceEntryMsscmp(Variables.replaceFilePath, ofd.FileName) == 1)
                    {
                        MessageBox.Show("ファイルの置き換えに失敗しました。ファイルが破損してる可能性があります。", "MCSE Editor for Wii U"
                        , MessageBoxButton.OK, MessageBoxImage.Exclamation);
                    }
                }
            }
            else
            {
                MessageBox.Show("置き換え先ファイルを選択してください", "MSSE Changer for Wii U", MessageBoxButton.OK, MessageBoxImage.Information);
            }

        }


        //Referd to https://stackoverflow.com/questions/4309827/get-fullpath-in-wpf-treeview
        static TreeViewItem GetParentItem(TreeViewItem item)
        {
            for (var i = VisualTreeHelper.GetParent(item); i != null; i = VisualTreeHelper.GetParent(i))
                if (i is TreeViewItem)
                    return (TreeViewItem)i;

            return null;
        }

        public string GetFullPath(TreeViewItem node)
        {
            if (node == null)
                throw new ArgumentNullException();

            var result = Convert.ToString(node.Header);

            for (var i = GetParentItem(node); i != null; i = GetParentItem(i))
                result = i.Header + "\\" + result;

            return result;
        }

        private void treeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            Variables.replaceFilePath = GetFullPath((TreeViewItem)e.NewValue);
        }

        private void saveToolButton_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.SaveFileDialog sfd = new System.Windows.Forms.SaveFileDialog();
            sfd.FileName = "Minecraft.msscmp";
            sfd.Filter = "msscmpファイル(*.msscmp)|*.msscmp|すべてのファイル(*.*)|*.*";
            sfd.Title = "保存先のファイルを選択してください";
            sfd.RestoreDirectory = true;
            if (sfd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if (saveMsscmp(sfd.FileName) == 1)
                {
                    MessageBox.Show("ファイルの保存に失敗しました。ファイルが破損してる可能性があります。", "MCSE Editor for Wii U"
                        , MessageBoxButton.OK, MessageBoxImage.Exclamation);
                }
            }

        }
    }
}
