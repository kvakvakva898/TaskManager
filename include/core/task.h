#pragma once

#include <cstdint>
#include <string>

// Статус задачи
enum class TaskStatus
{
	toDo,
	inProgress,
	done
};

// Приоритет задачи
enum class TaskPriority
{
	low,
	medium,
	hight
};

// Основная модель задачи
struct Task
{
	// Уникальный идентификатор задачи
	std::uint64_t id = 0;

	// Название задачи
	std::string title;

	// Подробное описание
	std::string description;

	// Текущий статус
	TaskStatus status = TaskStatus::toDo;

	// Приоритет
	TaskPriority priority = TaskPriority::medium;

	// Дата выполнения YYYY-MM-DD
	// На первом этапе храним в виде строки
	std::string deadline;
};