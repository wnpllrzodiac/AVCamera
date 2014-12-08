#pragma once
#include <thread>
#include "ui_mainwindow.h"
#include "camera.hpp"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void changeEvent(QEvent *e);

private Q_SLOTS:
	void on_startButton_clicked();

	void on_stopButton_clicked();

	void on_pauseButton_clicked();

	void onvideo_updated();

Q_SIGNALS:
	void video_updated();

private:
	Ui::MainWindow ui;
	VideoMgr::Camera _camera;
};
