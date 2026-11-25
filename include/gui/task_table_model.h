#pragma once

#include <QAbstractTableModel>

#include "core/task.h"

#include <vector>

class TaskTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	// Колонки таблицы
	enum Column
	{
		Title = 0,
		Status,
		Priority,
		Deadline,
		ColumnCount
	};

	explicit TaskTableModel(QObject* parent = nullptr);

	// Количество строк
	int rowCount(
		const QModelIndex& parent = QModelIndex()
	) const override;

	// Количество колонок
	int columnCount(
		const QModelIndex& parent = QModelIndex()
	) const override;

	// Возвращает данные для конкретной ячейки
	QVariant data(
		const QModelIndex& index,
		int role = Qt::DisplayRole
	) const override;

	// Возвращает заголовки колонок
	QVariant headerData(
		int section,
		Qt::Orientation orientation,
		int role = Qt::DisplayRole
	) const override;

	// Обновляет список задач в модели
	void setTasks(std::vector<Task> tasks);

	// Возвращает задачу по строке
	const Task* taskAt(int row) const;

private:
	std::vector<Task> tasks_;
};