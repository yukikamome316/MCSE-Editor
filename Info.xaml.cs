using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace MCSE_Editor_for_Wii_U
{
    /// <summary>
    /// Info.xaml の相互作用ロジック
    /// </summary>
    public partial class Info : Window
    {
        public Info()
        {
            InitializeComponent();
            MouseLeftButtonDown += (sender, e) => this.DragMove();

            // 第3引数をtrueにすることにより、処理済みのルーティングイベントにも反応します。
            yt.AddHandler(MouseUpEvent, new MouseButtonEventHandler(TextBlock_MouseLeftButtonUp), true);
            gt.AddHandler(MouseUpEvent, new MouseButtonEventHandler(TextBlock_MouseLeftButtonUp_1), true);
            rp.AddHandler(MouseUpEvent, new MouseButtonEventHandler(TextBlock_MouseLeftButtonUp_2), true);
        }

        private void TextBlock_MouseLeftButtonUp_2(object sender, MouseButtonEventArgs e)
        {
            Process.Start(rp.Text);
        }

        private void TextBlock_MouseLeftButtonUp_1(object sender, MouseButtonEventArgs e)
        {
            Process.Start(gt.Text);
        }

        private void TextBlock_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            Process.Start(yt.Text);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }


    }
}
