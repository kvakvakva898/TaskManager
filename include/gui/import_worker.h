#pragma once

#include <QObject>
#include <QString>

#include <vector>

#include "core/task.h"

class ImportWorker : public QObject
{
	Q_OBJECT

public:
	explicit ImportWorker(QString file_path);

public slots:
	// Метод будет выполняться в работчем потоке
	void process();

signals:
	// Результат успешного импорта
	void finished(std::vector<Task> tasks);

	// Ошибка импорта
	void error(QString message);

	// Информация о ходе выполнения
	void progress(int percentage);

private:
	QString file_path_;
};