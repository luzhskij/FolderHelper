#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include <QTimer>

#include "ui_folderhelper.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/scoped_ptr.hpp>

#include "coreconnector.h"
#include "folderview.h"

enum FSize
{
	BYTES,
	KBYTES,
	MBYTES
};

class FolderHelper : public QMainWindow
{
	Q_OBJECT

public:
	FolderHelper(QWidget* parent = Q_NULLPTR);

private:
	Ui::FolderHelperClass ui;

	boost::shared_ptr<CoreConnector> p_core_conn;
	boost::shared_ptr<FolderView> p_fl_view;

	QString parent_folder;

	qint64 filter_size_byte;

	void connect_slots();
	void con_discon_fsize(bool isSetCon = true);

public slots:
	void select_parent_folder();

	void set_selected_size();
	void set_unselected_size();

	void set_byte_size();
	void set_kbyte_size();
	void set_mbyte_size();

	void edit_filter_size(double);

	void start_operation();

	void end_operation();

	void print_finded_directory();
};
