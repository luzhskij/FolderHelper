#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_folderhelper.h"

class FolderHelper : public QMainWindow
{
	Q_OBJECT

public:
	FolderHelper(QWidget *parent = Q_NULLPTR);

private:
	Ui::FolderHelperClass ui;
};
