#pragma once
#include "ui_mainwindow.h"
#include "camera.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);

protected:
	void changeEvent(QEvent *e);

private Q_SLOTS:
	void on_startButton_clicked();

private:
	Ui::MainWindow ui;

	VideoMgr::Camera _camera;
};
