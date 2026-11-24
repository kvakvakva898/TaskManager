#include "task_dialog.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QDate>

// Преобразуем enum в индекс ComboBox
int statusToIndex(TaskStatus status)
{
	return static_cast<int>(status);
}

int priorityToIndex(TaskPriority priority)
{
	return static_cast<int>(priority);
}

TaskDialog::TaskDialog(const Task* task, QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle(task ? "Edit task" : "Create task");
	setMinimumWidth(400);

	// Создаем поля ввода
	title_edit_ = new QLineEdit(this);

	description_edit_ = new QTextEdit(this);

	status_combo_ = new QComboBox(this);
	status_combo_->addItems({
		"Todo",
		"In progress",
		"Done" });

	priority_combo_ = new QComboBox(this);
	priority_combo_->addItems({
		"Low",
		"Medium",
		"High" });

	deadline_edit_ = new QDateEdit(QDate::currentDate(), this);
	deadline_edit_->setCalendarPopup(true);
	deadline_edit_->setDisplayFormat("yyyy-MM-dd");

	// Размещаем поля в форме
	auto* form_layout = new QFormLayout();

	form_layout->addRow("Title:", title_edit_);
	form_layout->addRow("Description:", description_edit_);
	form_layout->addRow("Status:", status_combo_);
	form_layout->addRow("Priority:", priority_combo_);
	form_layout->addRow("Deadline:", deadline_edit_);

	// Создаем кнопки OK и Cancel
	auto* buttons = new QDialogButtonBox(
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		this);

	// При нажатии OK вызываем собственную проверку данных
	connect(
		buttons,
		&QDialogButtonBox::accepted,
		this,
		[this]()
		{
			if (validateInput())
			{
				accept();
			}
		});

	// Cancel закрывает диалог без сохранения
	connect(
		buttons,
		&QDialogButtonBox::rejected,
		this,
		&QDialog::reject);

	auto* main_layout = new QVBoxLayout(this);

	main_layout->addLayout(form_layout);
	main_layout->addWidget(buttons);

	// Если передана задача, то заполняем поля ее значениями
	if (task)
	{
		loadTask(*task);
	}

	adjustSize();
}

void TaskDialog::loadTask(const Task& task)
{
	task_id_ = task.id;

	title_edit_->setText(
		QString::fromStdString(task.title));

	description_edit_->setPlainText(
		QString::fromStdString(task.description));

	status_combo_->setCurrentIndex(
		statusToIndex(task.status));

	priority_combo_->setCurrentIndex(
		priorityToIndex(task.priority));

	const QDate deadline = QDate::fromString(
		QString::fromStdString(task.deadline),
		"yyyy-MM-dd");

	if (deadline.isValid())
	{
		deadline_edit_->setDate(deadline);
	}
}

bool TaskDialog::validateInput()
{
	if (title_edit_->text().trimmed().isEmpty())
	{
		QMessageBox::warning(
			this,
			"Validation error",
			"Task title cannot be empty");

		return false;
	}

	return true;
}

Task TaskDialog::getTask() const
{
	Task task;

	task.id = task_id_;

	// QString и std::string не взаимозаменяемы,
	// поэтому явно преобразуем значения
	task.title = title_edit_->text().trimmed().toStdString();

	task.description = description_edit_->toPlainText().toStdString();

	task.status = static_cast<TaskStatus>(
		status_combo_->currentIndex());

	task.priority = static_cast<TaskPriority>(
		priority_combo_->currentIndex());

	task.deadline = deadline_edit_->date()
		.toString("yyyy-MM-dd")
		.toStdString();

	return task;
}