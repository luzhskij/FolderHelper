#pragma once

#include <QObject>
#include <QtCore>
#include <QDir>
#include <QFileInfo>

#include <filesystem>
#include <iostream>

#include <boost/filesystem.hpp>

class slash_correcter
{
private:
	void correct_slash(QString& inp_str)
	{
		if (inp_str.contains("\\")) {
			inp_str.replace("\\", "/");
		}
		// 		if (inp_str.contains("//")) {
		// 			inp_str.replace("//", "/");
		// 		}
		if (inp_str.contains("//")) {
			inp_str.replace(QRegExp("[/]{2,}"), "/");
			inp_str = inp_str.trimmed();
		}
	}
	void fix_end_slash(QString& inp_str)
	{
		if (inp_str.at(inp_str.size() - 1) != '/') {
			inp_str.append("/");
		}
	}
public:
	slash_correcter() {}
	void check_slash(QString& inp_str, bool need_correct = true, bool need_fix = true)
	{
		if (inp_str.isEmpty()) {
			return;
		}
		if (need_correct) {
			correct_slash(inp_str);
		}
		if (need_fix) {
			fix_end_slash(inp_str);
		}
	}
};

enum DirType
{
	ALL_DIRS,
	ONLY_EMPTY,
	NOT_EMPTY
};

class FolderHelp : public QObject
{
	Q_OBJECT

public:
	FolderHelp();
	~FolderHelp();

	bool check_create_folder(const QString& folder_name);
	void slash_cor_fixend(QString& inp_str, bool need_correct = true, bool need_fix = true);

	QStringList find_file_in_folder(const QString& interestFolder, const QStringList& fileSuf, qint64 minSize = 0);
	qint64		get_all_file_size() { return allfilesize; }

	bool copy_files_to(const QStringList& lFiles, const QString& toFolder);

	QStringList get_sub_dir_list(const QString& parent_dir, DirType drTp = ALL_DIRS);

	qint64 get_folder_size(const QString& folderName);

	bool delete_folder(const QString& fol_name);

private:
	slash_correcter* slash_cor;

	void scan_directory(QDir dir);
	std::vector<std::string> get_directories(const std::string& s);
	qint64		minimumSize;
	qint64		allfilesize;
	QStringList	result_find_list;
	QStringList	list_suffixs;

	QStringList toshpcollect;
	bool copy_shape_file(const QString& shpFile, const QString& endFolder);
};
