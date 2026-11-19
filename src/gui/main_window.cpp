#include "main_window.h"

#include <QLabel>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	// Устанавливаем заголовок главного окна
	setWindowTitle("task Manager");

	// Задаем начальный размер окна
	resize(900, 600);

	// Центральный виджет главного окна
	// QMainWindow самостоятельно управляет центральны виджетом
	// поэтому передавать ему parent вручную не нужно
	auto* label = new QLabel("Welcome to Task Manager!", this);

	// Устанавливаем центральный виджет
	setCentralWidget(label);
}