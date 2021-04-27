#include "folderhelper.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	FolderHelper w;
	w.show();
	return a.exec();
}