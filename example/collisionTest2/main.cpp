#include "stable.h"

#include "mainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWidget w;
	w.canvas()->setInterval(40);
	w.canvas()->start();
	w.show();

	return a.exec();
}
