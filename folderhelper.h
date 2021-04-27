#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets>

#include "ui_folderhelper.h"

#include <boost/shared_ptr.hpp>

#include "coreconnector.h"

class FolderHelper : public QMainWindow
{
	Q_OBJECT

public:
	FolderHelper(QWidget* parent = Q_NULLPTR);

private:
	Ui::FolderHelperClass ui;

	boost::shared_ptr<CoreConnector> p_core_conn;

	QString parent_folder;

	void connect_slots();

public slots:
	void select_parent_folder();

	void set_selected_size();
	void set_unselected_size();

	void set_byte_size();
	void set_kbyte_size();
	void set_mbyte_size();
};
