#include "coreconnector.h"

#include <boost/make_shared.hpp>

CoreConnector::CoreConnector(QObject* parent)
	: QObject(parent)
{
	p_folder_help = boost::make_shared<FolderHelp>();
}

CoreConnector::~CoreConnector()
{
}

void CoreConnector::set_task(const QString& par_dir, qint64 fi_si, DirType wh_dir, bool print_only)
{
	list_of_directory.clear();

	parent_name_dir = par_dir;
	filter_size = fi_si;
	dir_selection = wh_dir;
	print_dir_only = print_only;
}

void CoreConnector::delete_folders(const QStringList& del_folder)
{
	for (auto v : del_folder) {
		p_folder_help->delete_folder(v);
	}
}

void CoreConnector::start_task()
{
	auto allDirectoryList = p_folder_help->get_sub_dir_list(parent_name_dir, dir_selection);
	if (allDirectoryList.isEmpty()) {
		qDebug() << "Not Find Any Dir";
		emit end_task();
		return;
	}

	if (filter_size > 0) {
		for (auto vd : allDirectoryList) {
			if (p_folder_help->get_folder_size(vd) < filter_size) {
				list_of_directory.push_back(vd);
			}
		}
	}
	else {
		list_of_directory = allDirectoryList;
	}

	emit end_task();
}

void CoreConnector::delete_folders_slot(QStringList del_fldr)
{
	for (auto v : del_fldr) {
		p_folder_help->delete_folder(v);
	}
}