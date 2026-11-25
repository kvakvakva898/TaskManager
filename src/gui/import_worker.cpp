#include "gui/import_worker.h"
#include "core/task.h"

#include <vector>
#include <stdexcept>

void ImportWorker::process()
{
	try
	{
		// Читаем и разбираем JSON
		std::vector<Task> tasks;

		// Логика импорта

		emit finished(std::move(tasks));
	}
	catch (const std::exception& e)
	{
		emit error(
			QString::fromUtf8(e.what()));
	}
}