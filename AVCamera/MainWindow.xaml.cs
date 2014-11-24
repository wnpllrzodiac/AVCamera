using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using VideoMgr;

namespace AutoCamera
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    /// 
    public enum CameraSatus
    {
        CREATED   = 0,
        RECORDING = 1,
        PAUSED    = 2,
        STOPPED   = 4,
    };
    public partial class MainWindow : Window
    {
        public CameraSatus _camera_status = CameraSatus.CREATED;
        public MainWindow()
        {
            InitializeComponent();
        }

        private void StartButton_Click(object sender, RoutedEventArgs e)
        {
            _camera = new Camera("d:\\test.mp4");
        }
        private void PauseButton_Click(object sender, RoutedEventArgs e)
        {
            _camera_status = (CameraSatus)_camera.pause();
        }
        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            _camera_status = (CameraSatus)_camera.stop();
        }

        private Camera _camera = null;
    }
}
