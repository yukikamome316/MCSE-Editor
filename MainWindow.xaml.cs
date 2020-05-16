using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;


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
        [DllImport("msscmp.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern int wav2binka(string wav, string binka);
        [DllImport("msscmp.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        internal static extern int binka2wav(string binka, string wav);


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

        public static void Delete(string targetDirectoryPath)
        {
            if (!Directory.Exists(targetDirectoryPath))
            {
                return;
            }

            //ディレクトリ以外の全ファイルを削除
            string[] filePaths = Directory.GetFiles(targetDirectoryPath);
            foreach (string filePath in filePaths)
            {
                File.SetAttributes(filePath, FileAttributes.Normal);
                File.Delete(filePath);
            }

            //ディレクトリの中のディレクトリも再帰的に削除
            string[] directoryPaths = Directory.GetDirectories(targetDirectoryPath);
            foreach (string directoryPath in directoryPaths)
            {
                Delete(directoryPath);
            }

            //中が空になったらディレクトリ自身も削除
            Directory.Delete(targetDirectoryPath, false);
        }

        private void window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                Delete("tmp");
            }
            catch {; }

            if (!string.IsNullOrEmpty(Variables.openFilePath))
            {
                if (extractMsscmp(Variables.openFilePath) == 1)
                {
                    MessageBox.Show("ファイルの展開に失敗しました。ファイルが破損してる可能性があります。", "MCSE Editor for Wii U"
                        , MessageBoxButton.OK, MessageBoxImage.Exclamation);

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

                    Properties.Settings.Default.recentFile = Variables.openFilePath;
                    Properties.Settings.Default.Save();
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

        private void ReplaceBinka(System.Windows.Forms.OpenFileDialog ofd)
        {
            string stBaseName = Path.GetFileNameWithoutExtension(ofd.FileName);
            string newFilePath = stBaseName + ".binka";

            if (wav2binka(ofd.FileName.Replace(@"\", "/"), newFilePath.Replace(@"\", "/")) == 1)
            {
                MessageBox.Show(".wav を .binka に変換できませんでした。ファイルが破損してる可能性があります。", "MCSE Editor for Wii U"
                        , MessageBoxButton.OK, MessageBoxImage.Exclamation);
                return;
            }

            ofd.FileName = newFilePath;
        }

        private void replaceToolButton_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(Variables.selectedFilePath))
            { 
                MessageBox.Show("置き換え先ファイルを選択してください", "MSSE Changer for Wii U", MessageBoxButton.OK, MessageBoxImage.Information);
                return;
            }
            
            var isDirectory = File
            .GetAttributes(@"tmp\" + Variables.selectedFilePath)
            .HasFlag(FileAttributes.Directory);


            if (isDirectory == true && !string.IsNullOrEmpty(Variables.selectedFilePath))
            {
                MessageBox.Show("ディレクトリが選択されています。置き換え先ファイルを選択してください", "MSSE Changer for Wii U", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            System.Windows.Forms.OpenFileDialog ofd = new System.Windows.Forms.OpenFileDialog();
            ofd.FileName = ".binka";
            ofd.Filter = "音声ファイル(*.wav;*.mp3;*.ogg)|*.wav;*.mp3;*.ogg|binkaファイル(*.binka)|*.binka|すべてのファイル(*.*)|*.*";
            ofd.Title = "置き換え元ファイルを選択してください";
            ofd.RestoreDirectory = true;

            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if (Path.GetExtension(ofd.FileName) == ".wav")
                {
                    ReplaceBinka(ofd);
                }
                else
                {
                    string path = "cache.wav";
                    string arguments = string.Format("-y -i \"{0}\" \"{1}\"", ofd.FileName, path);
                    Home.ByteArrayToFile("ffmpeg.exe", Properties.Resources.ffmpeg);
                    var process = Process.Start(new ProcessStartInfo("ffmpeg.exe", arguments)
                    {
                        CreateNoWindow = false,
                        UseShellExecute = false,
                    });
                    process.WaitForExit();
                    ofd.FileName = "cache.wav";
                    ReplaceBinka(ofd);
                }

                if (replaceEntryMsscmp(Variables.selectedFilePath.Replace(@"\", "/"), ofd.FileName) == 1)
                {
                    MessageBox.Show("ファイルの置き換えに失敗しました。ファイルが破損してる可能性があります。", "MCSE Editor for Wii U"
                    , MessageBoxButton.OK, MessageBoxImage.Exclamation);
                }
                        
                try
                {
                    File.Delete("cache.binka");
                    File.Delete("cache.wav");
                    File.Delete("ffmpeg.exe");
                }
                catch { return; }
                    
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
                result = i.Header + @"\" + result;

            return result;
        }

        private void treeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            Variables.selectedFilePath = GetFullPath((TreeViewItem)e.NewValue);
        }

        private void saveToolButton_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.SaveFileDialog sfd = new System.Windows.Forms.SaveFileDialog();
            sfd.FileName = "Minecraft.msscmp";
            sfd.Filter = "msscmpファイル(*.msscmp)|*.msscmp|すべてのファイル(*.*)|*.*";
            sfd.Title = "保存先のフォルダを選択してください";
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

        private void downloadToolButton_Click(object sender, RoutedEventArgs e)
        {
            if (!string.IsNullOrEmpty(Variables.selectedFilePath))
            {
                var isDirectory = File
                .GetAttributes(@"tmp\" + Variables.selectedFilePath)
                .HasFlag(FileAttributes.Directory);

                if (isDirectory == true)
                {
                    MessageBox.Show("ディレクトリが選択されています。ダウンロード先フォルダを選択してください", "MSSE Changer for Wii U", MessageBoxButton.OK, MessageBoxImage.Warning);
                    return;
                }
                System.Windows.Forms.SaveFileDialog sfd = new System.Windows.Forms.SaveFileDialog();
                sfd.FileName = ".binka";
                sfd.Filter = "binkaファイル(*.binka)|*.binka|すべてのファイル(*.*)|*.*";
                sfd.Title = "ダウンロード先のフォルダを選択してください";
                sfd.RestoreDirectory = true;
                if (sfd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                {
                    File.Copy(@"tmp\" + Variables.selectedFilePath, sfd.FileName);
                }
            }
            else
            {
                MessageBox.Show("ダウンロードするファイルを選択してください", "MSSE Changer for Wii U", MessageBoxButton.OK, MessageBoxImage.Information);
            }

        }

        private System.Media.SoundPlayer player = null;

        private void PlaySound(string waveFile)
        {
            if (player != null)
                StopSound();

            player = new System.Media.SoundPlayer(waveFile);

            player.Play();
        }

        private void StopSound()
        {
            if (player != null)
            {
                player.Stop();
                player.Dispose();
                player = null;
            }
        }

        private void OnItemMouseDoubleClick(object sender, RoutedEventArgs e)
        {
            var isDirectory = File
                .GetAttributes(@"tmp\" + Variables.selectedFilePath)
                .HasFlag(FileAttributes.Directory);

            if (isDirectory == true)
                return;

            binka2wav(@"tmp\" + Variables.selectedFilePath, "cache.wav");
            PlaySound("cache.wav");
        }

        private void infoToolButton_Click(object sender, RoutedEventArgs e)
        {
            Info info = new Info();
            info.ShowDialog();
        }

        private void settingToolButton_Click(object sender, RoutedEventArgs e)
        {
            Process.Start("powershell_ise");
        }
    }
    
}
