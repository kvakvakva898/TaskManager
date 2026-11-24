#pragma once

#include <QMainWindow>

class QTableWidget;
class QListWidget;
class QLineEdit;
class QPushbutton;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public: 
	explicit MainWindow(QWidget* parent = nullptr);

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
	// Элументы интерфейса
	QTableWidget* tasks_table_ = nullptr;
	QListWidget* filters_list_ = nullptr;
	QLineEdit* search_edit_ = nullptr;
	QPushbutton* add_button_ = nullptr;
};