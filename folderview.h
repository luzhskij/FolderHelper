#pragma once

#include <QWidget>
#include "ui_folderview.h"

class FolderView : public QWidget
{
	Q_OBJECT

public:
	FolderView(QWidget *parent = Q_NULLPTR);
	~FolderView();

private:
	Ui::FolderView ui;
};
