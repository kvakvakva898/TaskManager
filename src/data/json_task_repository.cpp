#include "data/json_task_repository.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <filesystem>
#include <stdexcept>

using json = nlohmann::json;

namespace
{
	std::string statusToString(TaskStatus status)
	{
		switch (status)
		{
		case TaskStatus::todo:
			return "Todo";

		case TaskStatus::inProgress:
			return "InProgress";

		case TaskStatus::done:
			return "Done";
		}

		throw std::runtime_error("Unknown task status");
	}

	TaskStatus stringToStatus(const std::string& value)
	{
		if (value == "Todo")
			return TaskStatus::todo;

		if (value == "InProgress")
			return TaskStatus::inProgress;

		if (value == "Done")
			return TaskStatus::done;

		throw std::runtime_error("Invalid task status");
	}

	std::string priorityToString(TaskPriority priority)
	{
		switch (priority)
		{
		case TaskPriority::low:
			return "Low";

		case TaskPriority::medium:
			return "Medium";

		case TaskPriority::high:
			return "High";
		}

		throw std::runtime_error("Unknown task priority");
	}

	TaskPriority stringToPriority(const std::string value)
	{
		if (value == "Low")
			return TaskPriority::low;

		if (value == "Medium")
			return TaskPriority::medium;

		if (value == "High")
			return TaskPriority::high;

		throw std::runtime_error("Invalid task priority");
	}
}

JsonTaskRepository::JsonTaskRepository(
	std::filesystem::path file_path)
	: file_path_(file_path)
{}

std::vector<Task> JsonTaskRepository::load()
{
	if (!std::filesystem::exists(file_path_))
		return {};

	std::ifstream file(file_path_);

	if (!file.is_open())
	{
		throw std::runtime_error("Cannot open tasks file");
	}

	// Читаем Json из файла
	const json document = json::parse(file);

	std::vector<Task> tasks;

	for (const auto& item : document)
	{
		Task task;

		task.id = item.at("id").get<std::uint64_t>();
		task.title = item.at("title").get<std::string>();

		task.description =
			item.at("description").get<std::string>();

		task.status = stringToStatus(
			item.at("status").get<std::string>());

		task.priority = stringToPriority(
			item.at("priority").get<std::string>());

		task.deadline = item.at("deadline").get<std::string>();

		tasks.push_back(std::move(task));
	}

	return tasks;
}

void JsonTaskRepository::save(const std::vector<Task>& tasks)
{
	json document = json::array();

	for (const auto& task : tasks)
	{
		document.push_back({
			{"id", task.id},
			{"title", task.title},
			{"description", task.description},
			{"status", statusToString(task.status)},
			{"priority", priorityToString(task.priority)},
			{"deadline", task.deadline}
			});
	}

	// Создаем директорию, если она отсутствует
	const auto parent = file_path_.parent_path();

	if (!parent.empty())
	{
		std::filesystem::create_directories(parent);
	}

	// Сохраняем Json в файл
	std::ofstream file(file_path_);

	if (!file.is_open())
	{
		throw std::runtime_error("Cannot save tasks file");
	}

	file << document.dump(4);
}