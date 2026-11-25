#pragma once

#include <QMainWindow>

#include "core/task_service.h"

class QTableView;
class TaskTableModel;
class QListWidget;
class QLineEdit;
class QPushbutton;
class QSortFilterProxyModel;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public: 
	explicit MainWindow(
		TaskService& task_service,
		QWidget* parent = nullptr);

private:
	// Создает центральную часть интерфейса
	void createCentralWidget();

	// Создает главное меню
	void createMenuBar();

	// Создает панель инструментов
	void createToolBar();

	// Создает строку состояния
	void createStatusBar();

private slots:
	void createTask();

private:
	// Обновляет содержимое таблицы
	void refreshTasks();

private:
	// Сервис не принадлежит MainWindow
	// Его время жизни контролирутеся в main.cpp
	TaskService& task_service_;

	// Элементы интерфейса
	QTableView* tasks_table_ = nullptr;
	TaskTableModel* task_model_ = nullptr;
	QSortFilterProxyModel* proxy_model_ = nullptr;
	QListWidget* filters_list_ = nullptr;
	QLineEdit* search_edit_ = nullptr;
	QPushbutton* add_button_ = nullptr;
};