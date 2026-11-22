#include "main_window.h"

#include <QLabel>
#include <QTableWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QHeaderView>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	// Устанавливаем заголовок главного окна
	setWindowTitle("Task Manager");

	// Задаем начальный размер окна
	resize(900, 600);

	createMenuBar();
	createToolBar();
	createCentralWidget();
	createStatusBar();
}

void MainWindow::createMenuBar()
{
	// QMenuBar уже существует внутри QMainWindow
	auto* file_menu = menuBar()->addMenu("&File");

	// QAction представляет действие, которое можно использовать
	// одновременно в меню, на панели инструментов и в других местах
	auto* exit_action = new QAction("Exit", this);

	// Подключаем сигнал triggered к слоту закрытия окна
	connect(
		exit_action,
		&QAction::triggered,
		this,
		&QWidget::close);

	file_menu->addAction(exit_action);
}

void MainWindow::createToolBar()
{
	auto* toolbar = addToolBar("Main Toolbar");

	// Создаем QAction для добавления задачи
	auto* add_action = new QAction("Add task", this);

	toolbar->addAction(add_action);
}

void MainWindow::createCentralWidget()
{
	// QMainWindow требует центральный виджет,
	// внутри которого размещается основное содержимое
	auto* central_widget = new QWidget(this);

	// Главный layout располагает sidebar и таблицу горизонтально
	auto* main_layout = new QHBoxLayout(central_widget);

	// Создаем разделитель, позволяющий менять размеры областей
	auto* splitter = new QSplitter(Qt::Horizontal, central_widget);

	// Левая область: фильтры
	filters_list_ = new QListWidget(splitter);

	filters_list_->addItem("All tasks");
	filters_list_->addItem("Todo");
	filters_list_->addItem("In progress");
	filters_list_->addItem("Done");

	// Правая область: таблица задач
	tasks_table_ = new QTableWidget(splitter);

	// Временная структура из 3 колонок
	tasks_table_->setColumnCount(3);

	tasks_table_->setHorizontalHeaderLabels({
		"Title",
		"Status",
		"Priority"});

	// Разрешаем пользователю изменять ширину колонок
	tasks_table_->horizontalHeader()->setStretchLastSection(true);

	// Добавляем обе области в splitter
	splitter->addWidget(filters_list_);
	splitter->addWidget(tasks_table_);

	// Начальные размеры областей
	splitter->setSizes({ 200, 800 });

	main_layout->addWidget(splitter);

	//Создаем строку поиска
	search_edit_ = new QLineEdit(central_widget);
	search_edit_->setPlaceholderText("Search tasks...");

	// Добавляем поиск над основным содержимым
	auto* root_layout = new QVBoxLayout();

	root_layout->addWidget(search_edit_);
	root_layout->addLayout(main_layout);

	central_widget->setLayout(root_layout);
	
	setCentralWidget(central_widget);
}

void MainWindow::createStatusBar()
{
	// QMainWindow также прдоставляет готовую строку состояния
	statusBar()->showMessage("Ready");
}