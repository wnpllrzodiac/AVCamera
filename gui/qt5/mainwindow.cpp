
#include <QPainter>
#include <qevent.h>
#include <QMetaType>
#include <boost/concept_check.hpp>
#include "mainwindow.hpp"

Q_DECLARE_METATYPE(QImage);
Q_DECLARE_METATYPE(QSharedPointer<QImage>);


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

//	ui.video_frame->setAttribute(Qt::WA_PaintOnScreen);
//	ui.video_frame->setAttribute(Qt::WA_StaticContents);

	connect(this, &MainWindow::video_updated, this, &MainWindow::onvideo_updated, Qt::QueuedConnection);

	_camera.refresh_sign.connect(boost::bind(&MainWindow::video_updated, this));

}

MainWindow::~MainWindow()
{
	_camera.stop();
	_camera.exit();
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
	_camera.start("test.mp4", 720, 400, 1815484);
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
	cv::Mat mat;
	_camera.get_curr_frame(mat);
	if(mat.data == nullptr) return;

	QImage img((uchar*) mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);

	ui.video_frame->m_video_image = img.convertToFormat(QImage::Format_ARGB32);

	ui.video_frame->update();
}
