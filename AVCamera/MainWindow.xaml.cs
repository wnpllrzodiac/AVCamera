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
using System.Globalization;
using VideoMgr;

namespace AVCamera
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
    public class ButtonStatus : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string param = (string)parameter;
            if (param == "Start")
            {
                return MainWindow._camera_status == CameraSatus.CREATED
                || MainWindow._camera_status == CameraSatus.PAUSED
                || MainWindow._camera_status == CameraSatus.STOPPED;
            }
            else if (param == "Pause")
            {
                return MainWindow._camera_status == CameraSatus.RECORDING;
            }
            else if (param == "Stop")
            {
                return MainWindow._camera_status == CameraSatus.RECORDING
                || MainWindow._camera_status == CameraSatus.PAUSED;
            }
            return true;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
    public partial class MainWindow : Window
    {
        static public CameraSatus _camera_status = CameraSatus.CREATED;
        public MainWindow()
        {
            InitializeComponent();
        }

        private void StartButton_Click(object sender, RoutedEventArgs e)
        {
            if(_camera_status == CameraSatus.CREATED
                || _camera_status == CameraSatus.STOPPED)
            {
                Task task = new Task(() =>
                {
                    _camera.thread_task("d:\\test.mp4", 640, 480, 40000);
                });
                task.Start();
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
