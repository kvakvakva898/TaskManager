#include "core/task_service.h"

#include <algorithm>
#include <stdexcept>

TaskService::TaskService(TaskRepository& repository)
	: repository_(repository)
{}

void TaskService::loadTasks()
{
	// Загружаем задачи из репозитория
	tasks_ = repository_.load();

	// Находим максимальный существующий ID,
	// чтобы новые задачи не получили уже занятый идентификатор
	next_id_ = 1;
	
	for (const auto& task : tasks_)
	{
		next_id_ = std::max(next_id_, task.id + 1);
	}
}

void TaskService::addTask(Task task)
{
	// Проверяем, что название задачи не пустое
	if (task.title.empty())
	{
		throw std::invalid_argument("Task title cannot be empty");
	}

	// Назначаем уникальный идентификаторы
	task.id = next_id_++;

	// Добавляем задачу в список
	tasks_.push_back(std::move(task));

	// Сохраняем обновленный список
	repository_.save(tasks_);
}

void TaskService::updateTask(const Task& task)
{
	// Ищем задачу с соответствующим идентификатором
	const auto it = std::find_if(
		tasks_.begin(),
		tasks_.end(),
		[&task](const Task& current)
		{
			return current.id == task.id;
		}
	);

	// Если задача не найдена, бросаем исключение
	if (it == tasks_.end())
	{
		throw std::runtime_error("Task not found");
	}

	if (task.title.empty())
	{
		throw std::invalid_argument("Task title cannot be empty");
	}

	// Обновляем найденную задачу
	*it = task;

	repository_.save(tasks_);
}

void TaskService::revoveTask(std::uint64_t id)
{
	const auto it = std::find_if(
		tasks_.begin(),
		tasks_.end(),
		[id](const Task& task)
		{
			return task.id == id;
		}
	);

	if (it == tasks_.end())
	{
		throw std::runtime_error("Task not found");
	}

	tasks_.erase(it);

	repository_.save(tasks_);
}

const std::vector<Task>& TaskService::getTasks() const
{
	return tasks_;
}