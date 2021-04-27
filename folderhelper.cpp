#include "folderhelper.h"

#include <boost/make_shared.hpp>

#include <QFile>
#include <QFileDialog>

FolderHelper::FolderHelper(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	p_core_conn = boost::make_shared<CoreConnector>(this);

	connect_slots();
}

void FolderHelper::connect_slots()
{
	connect(ui.pushButtonSelectFolder, SIGNAL(clicked()), SLOT(select_parent_folder()));
}

void FolderHelper::select_parent_folder()
{
	QString saveFileFolder = QFileDialog::getExistingDirectory(this, tr("_Select Parent Folder"));
	if (saveFileFolder.isEmpty()) {
		return;
	}

	parent_folder.clear();
	ui.lineEditSelectFolder->clear();

	p_core_conn->get_p_fl_help()->slash_cor_fixend(saveFileFolder);

	parent_folder = saveFileFolder;
	ui.lineEditSelectFolder->setText(parent_folder);
}