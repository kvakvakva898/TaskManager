#include "gui/main_window.h"
#include "gui/task_dialog.h"
#include "gui/task_table_model.h"

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
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>

MainWindow::MainWindow(
	TaskService& task_service,
	QWidget* parent)
	: task_service_(task_service),
	  QMainWindow(parent)
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

	connect(
		add_action,
		&QAction::triggered,
		this,
		&MainWindow::createTask);
}

void MainWindow::createCentralWidget()
{
	// QMainWindow требует центральный виджет,
	// внутри которого размещается основное содержимое
	auto* central_widget = new QWidget(this);

	// Главный layout располагает sidebar и таблицу горизонтально
	auto* main_layout = new QHBoxLayout();

	// Создаем разделитель, позволяющий менять размеры областей
	auto* splitter = new QSplitter(Qt::Horizontal, central_widget);

	// Левая область: фильтры
	filters_list_ = new QListWidget(splitter);

	filters_list_->addItem("All tasks");
	filters_list_->addItem("Todo");
	filters_list_->addItem("In progress");
	filters_list_->addItem("Done");

	task_model_ = new TaskTableModel(this);

	// Правая область: таблица задач
	tasks_table_ = new QTableView(this);

	// Устанавливаем модель для отображения
	tasks_table_->setModel(task_model_);

	// Запрещаем редактирование ячеек непосредственно в таблице
	// ДЛя редактирования будем использовать TaskDialog
	tasks_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	// Разрешаем выбирать целые строки
	tasks_table_->setSelectionBehavior(
		QAbstractItemView::SelectRows);

	// Разрешаем выбирать только одну задачу
	tasks_table_->setSelectionMode(
		QAbstractItemView::SingleSelection);

	// Разрешаем пользователю изменять ширину колонок
	tasks_table_->horizontalHeader()->setStretchLastSection(true);

	proxy_model_ = new QSortFilterProxyModel(this);

	// Устанавливаем исходную модель
	proxy_model_->setSourceModel(task_model_);

	// Поиск будет выполнятся по всем колонкам
	proxy_model_->setFilterKeyColumn(-1);

	// По умолчанию поиск нечувствителен к регистру
	proxy_model_->setFilterCaseSensitivity(Qt::CaseInsensitive);

	// Подключаем proxy model к таблице
	tasks_table_->setModel(proxy_model_);

	refreshTasks();

	// Разрешаем сортировку по заголовкам колонок
	tasks_table_->setSortingEnabled(true);

	// Добавляем обе области в splitter
	splitter->addWidget(filters_list_);
	splitter->addWidget(tasks_table_);

	// Начальные размеры областей
	splitter->setSizes({ 200, 800 });

	main_layout->addWidget(splitter);

	//Создаем строку поиска
	search_edit_ = new QLineEdit(central_widget);
	search_edit_->setPlaceholderText("Search tasks...");

	// Подключаем поле поиск
	connect(
		search_edit_,
		&QLineEdit::textChanged,
		proxy_model_,
		&QSortFilterProxyModel::setFilterFixedString);

	// Добавляем поиск над основным содержимым
	auto* root_layout = new QVBoxLayout(central_widget);

	root_layout->addWidget(search_edit_);
	root_layout->addLayout(main_layout);
	
	setCentralWidget(central_widget);
}

void MainWindow::createStatusBar()
{
	// QMainWindow также прдоставляет готовую строку состояния
	statusBar()->showMessage("Ready");
}

void MainWindow::createTask()
{
	TaskDialog dialog(nullptr, this);

	// Открываем модальный диалог
	if (dialog.exec() != QDialog::Accepted)
	{
		return;
	}

	// Получаем задачу из полей диалога
	const Task task = dialog.getTask();

	try
	{
		// Передаем задачу сервису
		task_service_.addTask(std::move(task));

		// Обновляем отображение таблицы
		refreshTasks();

		// Показываем сообщение об успешном добавлении
		statusBar()->showMessage("Task added successfully", 3000);
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(
			this,
			"Error",
			QString::fromUtf8(e.what()));
	}
}

void MainWindow::refreshTasks()
{
	// Получаем актуальный список задач из сервиса
	const auto& tasks = task_service_.getTasks();

	// Передаем его модели
	task_model_->setTasks(tasks);
}

void MainWindow::loadSettings()
{
	// QSettings автоматически определяет хранилище
	// настроект по организации и названию приложения
	QSettings settings(
		"MyCompany",
		"TaskManager");

	// Восстанавливаем размер окна
	resize(
		settings.value("window/size", QSize(1000, 650)).toSize());

	// Восстанавливаем положение окна
	if (settings.contains("window/position"))
	{
		move(settings.value("window/position").toPoint());
	}
}

void MainWindow::saveSettings()
{
	QSettings settings(
		"MyCompany",
		"TaskManager");
	
	settings.setValue("window/size", size());

	settings.setValue("window/position", pos());
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	saveSettings();

	// Передаем событие базовому классу
	QMainWindow::closeEvent(event);
}