
#include <QApplication>
#include <QIcon>
#include <QMainWindow>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	app.setWindowIcon(QIcon(":/icon/AVCamera.ico"));

	(new QMainWindow)->show();

	return app.exec();
}
