#include "folderhelper.h"

#include <QFile>
#include <QFileDialog>

FolderHelper::FolderHelper(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	p_core_conn = boost::make_shared<CoreConnector>(this);
	filter_size_byte = 0;

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

	con_discon_fsize();

	connect(ui.pushButtonStart, SIGNAL(clicked()), this, SLOT(start_operation()));

	connect(p_core_conn.get(), SIGNAL(end_task()), this, SLOT(end_operation()));

	connect(ui.pushButtonPrintDirs, SIGNAL(clicked()), this, SLOT(print_finded_directory()));
}

void FolderHelper::con_discon_fsize(bool isSetCon /*= true*/)
{
	if (isSetCon) {
		connect(ui.doubleSpinBoxSize, SIGNAL(valueChanged(double)), this, SLOT(edit_filter_size(double)));
	}
	else {
		disconnect(ui.doubleSpinBoxSize, SIGNAL(valueChanged(double)), this, SLOT(edit_filter_size(double)));
	}
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
	con_discon_fsize(false);
	ui.doubleSpinBoxSize->setDecimals(0);
	ui.doubleSpinBoxSize->setValue(filter_size_byte);
	con_discon_fsize();
}

void FolderHelper::set_kbyte_size()
{
	con_discon_fsize(false);
	double kbs = filter_size_byte / 1024.0;
	ui.doubleSpinBoxSize->setDecimals(2);
	ui.doubleSpinBoxSize->setValue(kbs);
	con_discon_fsize();
}

void FolderHelper::set_mbyte_size()
{
	con_discon_fsize(false);
	double mbs = filter_size_byte / (1024.0 * 1024.0);
	ui.doubleSpinBoxSize->setDecimals(1);
	ui.doubleSpinBoxSize->setValue(mbs);
	con_discon_fsize();
}

void FolderHelper::edit_filter_size(double fs_d)
{
	if (ui.radioButtonByte->isChecked()) {
		ui.doubleSpinBoxSize->setDecimals(0);
		filter_size_byte = static_cast<qint64>(fs_d);
	}
	else if (ui.radioButtonKiloByte->isChecked()) {
		ui.doubleSpinBoxSize->setDecimals(2);
		auto f_s_tn = fs_d * 1024.0;
		filter_size_byte = static_cast<qint64>(f_s_tn);
	}
	else {
		ui.doubleSpinBoxSize->setDecimals(1);
		auto f_s_tn = fs_d * 1024.0 * 1024.0;
		filter_size_byte = static_cast<qint64>(f_s_tn);
	}
}

void FolderHelper::start_operation()
{
	QString pr_dr = parent_folder;

	qint64 filter_size(filter_size_byte);
	if (!ui.groupBoxSizeFilter->isChecked()) {
		filter_size = 0;
	}

	DirType whatWant = ALL_DIRS;
	if (ui.radioButtonEmpty->isChecked()) {
		whatWant = ONLY_EMPTY;
	}
	else if (ui.radioButtonNotEmpty->isChecked()) {
		whatWant = NOT_EMPTY;
	}

	bool isPrintOnly(true);
	if (ui.radioButtonDelete->isChecked()) {
		isPrintOnly = false;
	}

	p_core_conn->set_task(pr_dr, filter_size, whatWant, isPrintOnly);

	QTimer::singleShot(150, p_core_conn.get(), SLOT(start_task()));
}

void FolderHelper::end_operation()
{
	auto what_find = p_core_conn->get_finded_dris();
	if (what_find.isEmpty()) {
		QMessageBox msgBox;
		msgBox.setWindowTitle("Result");
		msgBox.setText("Folders Not Found");
		msgBox.exec();
		return;
	}
	else {
		if (ui.radioButtonDelete->isChecked()) {
			QMessageBox msgBox;
			msgBox.setWindowTitle("Result");
			msgBox.setText("Attention!");

			QString txt = QString("Found %1 folders\nDo you want delete it?").arg(what_find.size());
			msgBox.setInformativeText(txt);

			msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setDefaultButton(QMessageBox::Cancel);
			int res = msgBox.exec();
			if (res == QMessageBox::Ok) {
				//нажата кнопка Ok
				p_core_conn->delete_folders(what_find);
				return;
			}
			else {
				return;
			}
		}
		else {
			print_finded_directory();
		}
	}
}

void FolderHelper::print_finded_directory()
{
	if (p_fl_view != nullptr) {
		p_fl_view.reset();
	}
	p_fl_view = boost::make_shared<FolderView>();

	connect(p_fl_view.get(), SIGNAL(want_delete_folders(QStringList)), p_core_conn.get(), SLOT(delete_folders_slot(QStringList)));

	auto what_find = p_core_conn->get_finded_dris();
	p_fl_view->set_finded_folders(what_find);

	p_fl_view->show();
}