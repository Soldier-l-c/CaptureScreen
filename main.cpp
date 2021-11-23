#include <Windows.h>
#include "CaptureScreenUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CaptureScreenUI w;
	w.show();
	return a.exec();
}
