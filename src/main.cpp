#include <QApplication>

#include "gui/main_window.h"
#include "data/json_task_repository.h"
#include "core/task_service.h"

int main(int argc, char* argv[])
{
	QApplication application(argc, argv);
	
	// Создаем репозиторий
	JsonTaskRepository repository("tasks.json");

	// Создаем сервис, передавая ему репозиторий
	TaskService service(repository);

	// Загружаем задачи
	service.loadTasks();
	
	// Передаем сервис главному окну
	MainWindow main_window(service);

	main_window.show();

	return application.exec();
}