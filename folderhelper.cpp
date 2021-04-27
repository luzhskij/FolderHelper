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

	connect(ui.radioButtonAllDir, SIGNAL(clicked()), this, SLOT(set_selected_size()));
	connect(ui.radioButtonEmpty, SIGNAL(clicked()), this, SLOT(set_unselected_size()));
	connect(ui.radioButtonNotEmpty, SIGNAL(clicked()), this, SLOT(set_selected_size()));

	connect(ui.radioButtonByte, SIGNAL(clicked()), this, SLOT(set_byte_size()));
	connect(ui.radioButtonKiloByte, SIGNAL(clicked()), this, SLOT(set_kbyte_size()));
	connect(ui.radioButtonMegaByte, SIGNAL(clicked()), this, SLOT(set_mbyte_size()));
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

void FolderHelper::set_selected_size()
{
	ui.groupBoxSizeFilter->setEnabled(true);
	ui.groupBoxSizeFilter->setChecked(true);
}

void FolderHelper::set_unselected_size()
{
	ui.groupBoxSizeFilter->setDisabled(true);
	ui.groupBoxSizeFilter->setChecked(false);
}

void FolderHelper::set_byte_size()
{
	auto nowdata = ui.doubleSpinBoxSize->value();
	if (nowdata <= 0) {
		ui.doubleSpinBoxSize->setValue(0);
		return;
	}



}

void FolderHelper::set_kbyte_size()
{
	auto nowdata = ui.doubleSpinBoxSize->value();
	if (nowdata <= 0) {
		ui.doubleSpinBoxSize->setValue(0);
		return;
	}



}

void FolderHelper::set_mbyte_size()
{
	auto nowdata = ui.doubleSpinBoxSize->value();
	if (nowdata <= 0) {
		ui.doubleSpinBoxSize->setValue(0);
		return;
	}



}