#pragma once

#include <QWidget>
#include <QStringListModel>

#include "ui_folderview.h"

class FolderView : public QWidget
{
	Q_OBJECT

public:
	FolderView(QWidget* parent = Q_NULLPTR);
	~FolderView();

	void set_finded_folders(const QStringList& fl_list);

private:
	Ui::FolderView ui;

	QStringList list_folders;
	std::shared_ptr<QStringListModel> modelColumns;

signals:
	void want_delete_folders(QStringList fl_list);
};
