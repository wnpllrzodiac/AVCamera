#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_camera_thread = std::thread([this]()
	{
		_camera.thread_task();
	});

	connect(this, &MainWindow::video_updated, this, &MainWindow::onvideo_updated, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
	_camera.stop();
	_camera.exit();
	m_camera_thread.join();
}

void MainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type())
	{
	case QEvent::LanguageChange:
		ui.retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::on_startButton_clicked()
{
	_camera.start("test.mp4", 640, 480, 1815484);
}

void MainWindow::on_stopButton_clicked()
{
	_camera.stop();
}

void MainWindow::on_pauseButton_clicked()
{
	_camera.pause();
}

void MainWindow::onvideo_updated()
{

}
