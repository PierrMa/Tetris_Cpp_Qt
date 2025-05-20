#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "gameboard.h"

#include <QBoxLayout>

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    board = new GameBoard(this);
    setCentralWidget(board);

    setWindowTitle("Tetris");
    resize(400,500);

    QHBoxLayout *hScoreLayout = new QHBoxLayout(this);
    hScoreLayout->setAlignment(Qt::AlignRight);
    hScoreLayout->addLayout(board->getVScoreLayout());
}

MainWindow::~MainWindow()
{
    delete ui;
}
