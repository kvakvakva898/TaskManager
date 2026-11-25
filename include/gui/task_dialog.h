#pragma once

#include <QDialog>

#include "core/task.h"

class QLineEdit;
class QTextEdit;
class QComboBox;
class QDateEdit;

class TaskDialog : public QDialog
{
	Q_OBJECT

public:
	// Если task == nullptr, то создаем новую задачу
	// Если task передан, то редактируем существующую
	explicit TaskDialog(
		const Task* Task = nullptr,
		QWidget* parent = nullptr);

	// Возвращает задачу, сформированную из полей диалога
	Task getTask() const;

private:
	// Заполняем поля существующими данными
	void loadTask(const Task& task);

	// Проверяет корректность введенных данных
	bool validateInput();

private:
	QLineEdit* title_edit_ = nullptr;
	QTextEdit* description_edit_ = nullptr;

	QComboBox* status_combo_ = nullptr;
	QComboBox* priority_combo_ = nullptr;

	QDateEdit* deadline_edit_ = nullptr;

	// ID нужен для сохранения идентификатора при редактировании
	std::uint64_t task_id_ = 0;
};