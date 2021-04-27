#include "folderhelp.h"

#include <boost/scoped_ptr.hpp>

static bool copy_uno_file(const QString& fi_from, const QString& fi_to)
{
	QDir dir;
	bool ismovefail(false);
	if (!dir.rename(fi_from, fi_to)) {
		qDebug() << "move failed";
		qDebug() << "old:" << fi_from;
		qDebug() << "new=" << fi_to;
		ismovefail = true;
	}

	if (!ismovefail) {
		return true;
	}

	boost::scoped_ptr< QFile > f_src(new QFile(fi_from));
	if (!f_src->open(QIODevice::ReadOnly)) {
		qDebug() << "copy_data: Cannot read file: " << f_src->errorString();
		return false;
	}
	if (!f_src->isReadable()) {
		qDebug() << "copy_data: File is not readable: " << fi_from;
		return false;
	}

	qint64 	m_srcSize = f_src->size();

	boost::scoped_ptr< QFile > t_src(new QFile(fi_to));
	if (!t_src->open(QIODevice::WriteOnly)) {
		qDebug() << "copy_data: Cannot open to write file: " << t_src->errorString();
		return false;
	}
	if (!t_src->isWritable()) {
		qDebug() << "copy_data: File is not writable: " << fi_to;
		return false;
	}

	QByteArray m_buf = QByteArray(8192, Qt::Uninitialized);
	qint64 m_hasRead = 0;
	qint64 m_hasWritten = 0;

	bool isFileReadOk(true);

	while (!f_src->atEnd()) {
		qint64 read = f_src->read(m_buf.data(), m_buf.size());
		if (read == -1) {
			qDebug() << "copy_data: Error while reading file(read == -1). File: " << fi_from;
			isFileReadOk = false;
			break;
		}
		m_hasRead += read;
		t_src->write(m_buf.constData(), read);
		t_src->flush();
		m_hasWritten += read;
	}
	t_src->flush();
	f_src->close();
	t_src->close();

	f_src.reset();
	t_src.reset();

	if (m_srcSize && m_hasWritten == m_srcSize) {
		//		qDebug() << "copy_data: Good copy file: " << fi_to;
	}
	else {
		qDebug() << "copy_data: !-!-ERROR-!-! copy file: " << fi_to;
	}

	return isFileReadOk;
}

static void copyPath(const QString& oldPath, const QString& newPath)
{
	if (QFileInfo(oldPath).isDir()) {
		QDir().mkdir(newPath);
		foreach(const QString & entry, QDir(oldPath).entryList(QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot))
			copyPath(oldPath + QDir::separator() + entry, newPath + QDir::separator() + entry);
	}
	else {
		copy_uno_file(oldPath, newPath);
	}
	QDir(newPath).refresh();
}

FolderHelp::FolderHelp()
	: slash_cor(new slash_correcter),
	minimumSize(0),
	allfilesize(0),
	result_find_list(QStringList()),
	list_suffixs(QStringList())
{
	toshpcollect.push_back(".shp");
	toshpcollect.push_back(".dbf");
	toshpcollect.push_back(".prj");
	toshpcollect.push_back(".cpg");
	toshpcollect.push_back(".shx");
}

FolderHelp::~FolderHelp()
{
}

bool FolderHelp::check_create_folder(const QString& folder_name)
{
	QDir dir(folder_name);
	if (!dir.exists()) {
		dir.mkpath(".");
		if (!dir.exists()) {
			std::cout << "FolderHelp: Error create folder: " << folder_name.toStdString() << std::endl;
			return false;
		}
		return true;
	}
	return true;
}

void FolderHelp::slash_cor_fixend(QString& inp_str, bool need_correct /*= true*/, bool need_fix /*= true*/)
{
	slash_cor->check_slash(inp_str, need_correct, need_fix);
}

QStringList FolderHelp::find_file_in_folder(const QString& interestFolder, const QStringList& fileSuf, qint64 minSize)
{
	QDir searchDir(interestFolder);
	searchDir.refresh();
	list_suffixs = fileSuf;
	result_find_list.clear();
	allfilesize = 0;
	minimumSize = minSize;
	scan_directory(searchDir);
	return result_find_list;
}

bool FolderHelp::copy_files_to(const QStringList& lFiles, const QString& toFolder)
{
	bool iKak(true);
	for (auto oncp : lFiles) {
		QString oldyf = oncp;
		if (oldyf.endsWith(".shp")) {
			iKak = copy_shape_file(oldyf, toFolder);
			if (!iKak) {
				qDebug() << "Error copy shape complect: " << oldyf << " to: " << toFolder;
			}
		}
		else {
			QString yangfile = toFolder + oncp.remove(0, oncp.lastIndexOf("/"));
			iKak = copy_uno_file(oldyf, yangfile);
			if (!iKak) {
				qDebug() << "Error copy: " << oldyf << " to: " << yangfile;
			}
		}
	}
	return true;
}

QStringList FolderHelp::get_sub_dir_list(const QString& parent_dir, DirType drTp)
{
	QString prntDir = parent_dir;
	slash_cor_fixend(prntDir);

	qint64 dirLenght = prntDir.length();

	auto tmpList = get_directories(prntDir.toStdString());

	QStringList dirList;
	for (auto vp : tmpList) {
		QString vpS = QString::fromStdString(vp);
		slash_cor_fixend(vpS);

		if (vpS.length() <= dirLenght) {
			continue;
		}

		switch (drTp)
		{
		case ONLY_EMPTY:
			if (QDir(vpS).isEmpty()) {
				dirList.push_back(vpS);
			}
			break;
		case NOT_EMPTY:
			if (!QDir(vpS).isEmpty()) {
				dirList.push_back(vpS);
			}
			break;
		case ALL_DIRS:
		default:
			dirList.push_back(vpS);
			break;
		}
	}
	return dirList;
}

bool FolderHelp::copy_shape_file(const QString& shpFile, const QString& endFolder)
{
	bool iKak(true);
	for (const QString& unsuf : toshpcollect) {
		QString orgnl = shpFile;
		orgnl.remove(".shp");
		orgnl.append(unsuf);

		QString yangfile = endFolder + orgnl.mid(orgnl.lastIndexOf("/") + 1, orgnl.size());
		iKak = copy_uno_file(orgnl, yangfile);
		if (!iKak) {
			qDebug() << "Error copy: " << orgnl << " to: " << yangfile;
		}
	}
	return true;
}

void FolderHelp::scan_directory(QDir dir)
{
	dir.setNameFilters(list_suffixs);
	dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	QStringList fileList = dir.entryList();
	for (int i = 0; i < fileList.count(); i++) {
		QString f_res_name = dir.path() + "/" + fileList[i];
		auto sizefile = QFileInfo(f_res_name).size();
		if (sizefile > minimumSize) {
			result_find_list.push_back(f_res_name);
			allfilesize += sizefile;
		}
	}
	dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	QStringList dirList = dir.entryList();
	for (int i = 0; i < dirList.size(); ++i) {
		QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i));
		scan_directory(QDir(newPath));
	}
}

std::vector<std::string> FolderHelp::get_directories(const std::string& s)
{
	std::vector<std::string> r;
	for (auto& p : std::filesystem::recursive_directory_iterator(s)) {
		if (p.is_directory()) {
			r.push_back(p.path().string());
		}
	}
	return r;
}

qint64 FolderHelp::get_folder_size(const QString& folderName)
{
	namespace bf = boost::filesystem;
	size_t size = 0;
	for (bf::recursive_directory_iterator it(folderName.toStdString());
		it != bf::recursive_directory_iterator();
		++it)
	{
		if (!bf::is_directory(*it))
			size += bf::file_size(*it);
	}

	return static_cast<qint64>(size);

	// 	QDirIterator it(folderName, QDirIterator::Subdirectories);
	// 	qint64 total(0);
	// 	while (it.hasNext()) {
	// 		total += it.fileInfo().size();
	// 	}
	// 	return total;
}

bool FolderHelp::delete_folder(const QString& fol_name)
{
	try {
		boost::filesystem::remove_all(fol_name.toStdString());
		return true;
	}
	catch (boost::filesystem::filesystem_error const& e) {
		std::string messg = e.what();

		QByteArray msg2;
		msg2 = QByteArray(messg.data(), int(messg.size()));
		qDebug() << "Error del folder: " << fol_name << " |reason: " << msg2;
		return false;
	}
	catch (...) {
		QString messg = tr("_Error cleaning directory:\n%1");
		qDebug() << messg.arg(fol_name);
		return false;
	}
}