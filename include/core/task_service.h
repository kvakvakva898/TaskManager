#pragma once

#include "core/task.h"
#include "data/task_repository.h"

#include <vector>
#include <cstdint>

class TaskService
{
public:
	explicit TaskService(TaskRepository& repository);

	// Загружает задачи из репозитория
	void loadTasks();

	// Добавляет новую задачу
	void addTask(Task task);

	// Обновляет существующую задачи
	void updateTask(const Task& task);

	// Удаляет задачу по индентификатору
	void revoveTask(std::uint64_t id);

	// Возвращает все задачи
	const std::vector<Task>& getTasks() const;

private:
	// Репозиторий не принадлежит сервису
	// Сервис только использует переданные объект
	TaskRepository& repository_;

	// Список задач в оперативной памяти
	std::vector<Task> tasks_;

	// Генератор идентификаторов
	std::uint64_t next_id_ = 1;
};