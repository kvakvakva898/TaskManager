#pragma once

#include "data/task_repository.h"

#include <filesystem>

class JsonTaskRepository : public TaskRepository
{
public:
	explicit JsonTaskRepository(std::filesystem::path file_path);

	std::vector<Task> load() override;

	void save(const std::vector<Task>& tasks) override;

private:
	std::filesystem::path file_path_;
};