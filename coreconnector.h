#pragma once

#include <QObject>

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

private:
	boost::shared_ptr<FolderHelp> p_folder_help;
};
