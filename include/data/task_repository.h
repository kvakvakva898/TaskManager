#pragma once

#include "core/task.h"

#include <vector>

// Абстракция для хранения задач
//
// Core знает только об этом интерфейсе
// Он не знает, будут ли данные хранится в JSON,
// SQLite или каком-либо удаленном сервисе
class TaskRepository
{
public:
	virtual ~TaskRepository() = default;

	// Загружает все задачи
	virtual std::vector<Task> load() = 0;

	// Сохраняет все задачи
	virtual void save(const std::vector<Task>& tasks) = 0;
};