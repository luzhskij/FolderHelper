#pragma once

#include <QObject>

#include "core/folderhelp.h"

class CoreConnector : public QObject
{
	Q_OBJECT

public:
	CoreConnector(QObject* parent);
	~CoreConnector();
};
