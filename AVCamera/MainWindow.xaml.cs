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
    /*public class ButtonStatus : IValueConverter
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
    }*/
    public partial class MainWindow : Window
    {
        static public CameraSatus _camera_status = CameraSatus.CREATED;
        public MainWindow()
        {
            InitializeComponent();
            UpdateUI(_camera_status);
            /*BitmapImage bi3 = new BitmapImage();
            bi3.BeginInit();
            bi3.SourceRect = new System.Windows.Int32Rect(0,0,640,480);
            //bi3.UriSource = new Uri("", UriKind.Relative);
            //bi3.EndInit();
            VideoImage.Stretch = Stretch.Fill;
            VideoImage.Source = bi3;*/
            
        }
        private void UpdateUI(CameraSatus status)
        {
            switch (status)
            {
                case CameraSatus.CREATED:
                    StartButton.Visibility = Visibility.Visible;
                    PauseButton.Visibility = Visibility.Hidden;
                    StopButton.Visibility = Visibility.Visible;

                    StartButton.IsEnabled = true;
                    PauseButton.IsEnabled = false;
                    StopButton.IsEnabled = false;
                    break;
                case CameraSatus.PAUSED:
                    StartButton.Visibility = Visibility.Visible;
                    PauseButton.Visibility = Visibility.Hidden;
                    StopButton.Visibility = Visibility.Visible;

                    StartButton.IsEnabled = true;
                    PauseButton.IsEnabled = false;
                    StopButton.IsEnabled = true;
                    break;
                case CameraSatus.RECORDING:
                    StartButton.Visibility = Visibility.Hidden;
                    PauseButton.Visibility = Visibility.Visible;
                    StopButton.Visibility = Visibility.Visible;

                    StartButton.IsEnabled = false;
                    PauseButton.IsEnabled = true;
                    StopButton.IsEnabled = true;
                    break;
                case CameraSatus.STOPPED:
                    StartButton.Visibility = Visibility.Visible;
                    PauseButton.Visibility = Visibility.Hidden;
                    StopButton.Visibility = Visibility.Visible;

                    StartButton.IsEnabled = true;
                    PauseButton.IsEnabled = false;
                    StopButton.IsEnabled = false;
                    break;
            }
        }

        private void StartButton_Click(object sender, RoutedEventArgs e)
        {
            if(_camera_status == CameraSatus.CREATED
                )
            {
                Task task = new Task(() =>
                {
                    _camera.thread_task("d:\\test.mp4", 640, 480, 20000);
                });
                task.Start();
                _camera_status = (CameraSatus)_camera.start();
            }
            else if (_camera_status == CameraSatus.STOPPED)
            {
                Task task = new Task(() =>
                {
                    _camera = new Camera();
                    _camera.thread_task("d:\\test.mp4", 640, 480, 20000);
                });
                task.Start();
                _camera_status = (CameraSatus)_camera.start();
            }
            else if (_camera_status == CameraSatus.PAUSED)
            {
                _camera_status = (CameraSatus)_camera.start();
            }
            UpdateUI(_camera_status);
        }
        private void PauseButton_Click(object sender, RoutedEventArgs e)
        {
            _camera_status = (CameraSatus)_camera.pause();
            UpdateUI(_camera_status);
        }
        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            _camera_status = (CameraSatus)_camera.stop();
            UpdateUI(_camera_status);
        }

        private Camera _camera = new Camera();

        private void Window_Closed(object sender, EventArgs e)
        {
            _camera.stop();
            Application.Current.Shutdown();
        }

        private void ExportPathButton_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.SaveFileDialog save = new Microsoft.Win32.SaveFileDialog();
            save.Title = "选择录像保存路径";
            save.Filter = "视频文件（.MP4）|*.MP4|所有文件|*.*";
            if ((bool)save.ShowDialog().GetValueOrDefault())
            {
                ExportPathTextBox.Text = save.FileName;
            }
        }
    }
}
