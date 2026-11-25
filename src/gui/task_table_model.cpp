#include "gui/task_table_model.h"

TaskTableModel::TaskTableModel(QObject* parent)
	: QAbstractTableModel(parent)
{}

int TaskTableModel::rowCount(const QModelIndex& parent) const
{
	// У модели нет вложенных элементов
	// Поэтому для дочерних индеков возвращаем 0
	if (parent.isValid())
	{
		return 0;
	}

	return static_cast<int>(tasks_.size());
}

int TaskTableModel::columnCount(const QModelIndex& parent) const
{
	if (parent.isValid())
	{
		return 0;
	}

	return ColumnCount;
}

QVariant TaskTableModel::data(
	const QModelIndex& index,
	int role
) const
{
	// Проверяем корректность индекса
	if (!index.isValid())
	{
		return {};
	}

	if (index.row() < 0 ||
		index.row() >= static_cast<int>(tasks_.size()))
	{
		return {};
	}

	const Task& task = tasks_[index.row()];

	// DisplayRole отвечает за отобраэаемый пользователю текст
	if (role != Qt::DisplayRole)
	{
		return {};
	}

	switch (index.column())
	{
	case Title:
		return QString::fromStdString(task.title);

	case Status:
		switch (task.status)
		{
		case TaskStatus::todo:
			return "Todo";

		case TaskStatus::inProgress:
			return "In progress";

		case TaskStatus::done:
			return "Done";
		}
		break;

	case Priority:
		switch (task.priority)
		{
		case TaskPriority::low:
			return "Low";

		case TaskPriority::medium:
			return "Medium";

		case TaskPriority::high:
			return "High";
		}
		break;

	case Deadline:
		return QString::fromStdString(task.deadline);
	}

	return {};
}

QVariant TaskTableModel::headerData(
	int section,
	Qt::Orientation orientation,
	int role
) const
{
	if (role != Qt::DisplayRole)
	{
		return {};
	}

	if (orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case Title:
			return "Title";
			
		case Status:
			return "Status";

		case Priority:
			return "Priority";

		case Deadline:
			return "Deadline";
		}
	}

	return QAbstractItemModel::headerData(
		section,
		orientation,
		role);
}

void TaskTableModel::setTasks(std::vector<Task> tasks)
{
	// Сообщаем Qt, что содержимое модели полностью заменяется
	beginResetModel();

	tasks_ = tasks;

	endResetModel();
}

const Task* TaskTableModel::taskAt(int row) const
{
	if (row < 0 || row >= static_cast<int>(tasks_.size()))
	{
		return nullptr;
	}

	return &tasks_[row];
}