
#include <QApplication>
#include <QIcon>
#include <QMainWindow>

#include <mainwindow.hpp>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	app.setWindowIcon(QIcon(":/icon/AVCamera.ico"));

	(new MainWindow)->show();

	return app.exec();
}
