
#ifndef FILEENTRY_H
#define FILEENTRY_H

#include <string>
#include <QFileIconProvider>

struct file_entry{
	qint64 size;
	std::string path;
	int offset;
	int index;
};

#endif