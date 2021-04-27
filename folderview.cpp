#include "folderview.h"

FolderView::FolderView(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	list_folders.clear();
}

FolderView::~FolderView()
{
}

void FolderView::set_finded_folders(const QStringList& fl_list)
{
	list_folders.clear();
	list_folders = fl_list;

	if (modelColumns != nullptr) {
		modelColumns.reset();
		ui.listViewFolders->reset();
	}

	modelColumns = std::make_shared<QStringListModel>(this);

	// Populate our model
	modelColumns->setStringList(list_folders);

	// Glue model and view together
	ui.listViewFolders->setModel(modelColumns.get());
}