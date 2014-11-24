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
using System.Threading.Tasks;
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
            if(_camera_status == CameraSatus.CREATED)
            {
                Task task = new Task(() =>
                {
                    _camera.thread_task("d:\\test.mp4", 640, 480, 40000);
                });
                //task.Start();
                _camera_status = (CameraSatus)_camera.start();
            }
            else if (_camera_status == CameraSatus.PAUSED)
            {
                _camera_status = (CameraSatus)_camera.start();
            }
        }
        private void PauseButton_Click(object sender, RoutedEventArgs e)
        {
            _camera_status = (CameraSatus)_camera.pause();
        }
        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            _camera_status = (CameraSatus)_camera.stop();
        }

        private Camera _camera = new Camera();
    }
}
