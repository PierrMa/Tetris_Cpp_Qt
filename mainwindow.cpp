#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gameboard.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    board = new GameBoard(this);
    setCentralWidget(board);

    setWindowTitle("Tetris");
    resize(300,500);
}

MainWindow::~MainWindow()
{
    delete ui;
}
