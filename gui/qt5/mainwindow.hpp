#pragma once
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);

protected:
	void changeEvent(QEvent *e);

private:
	Ui::MainWindow ui;
};
