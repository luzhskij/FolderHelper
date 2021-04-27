#pragma once

#include <QObject>
#include <QtDebug>

#include <boost/shared_ptr.hpp>

#include "core/folderhelp.h"

class CoreConnector : public QObject
{
	Q_OBJECT

public:
	CoreConnector(QObject* parent);
	~CoreConnector();

	boost::shared_ptr<FolderHelp> get_p_fl_help() {
		return p_folder_help;
	}

	void set_task(const QString& par_dir, qint64 fi_si, DirType wh_dir, bool print_only);

	QStringList get_finded_dris() {
		return list_of_directory;
	}

	void delete_folders(const QStringList& del_folder);

private:
	boost::shared_ptr<FolderHelp> p_folder_help;
	QStringList list_of_directory;

	QString parent_name_dir;
	qint64 filter_size;
	DirType dir_selection;
	bool print_dir_only;

public slots:
	void start_task();

	void delete_folders_slot(QStringList del_fldr);

signals:
	void end_task();
};
