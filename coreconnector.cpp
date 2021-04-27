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