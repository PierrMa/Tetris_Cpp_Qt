#include "mainwindow.h"
#include "gameboard.h"

#include <QBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent)
{

    //Layouts and container
    QWidget *container = new QWidget(this);
    QHBoxLayout *containerLayout = new QHBoxLayout(container); //main layout
    QVBoxLayout *scoreLayout = new QVBoxLayout(); //Layout for the score

    //Gameboard and container
    board = new GameBoard(container);

    //Score Labels
    QWidget *scoreWidget = new QWidget(container);
    QLabel *scoreTxtLabel = new QLabel("Score", scoreWidget);
    scoreTxtLabel->setFixedHeight(30);
    scoreTxtLabel->setStyleSheet("font-weight: bold; font-size: 26px;");
    QLabel *scoreValueLabel = new QLabel("0", scoreWidget);
    scoreValueLabel->setFixedHeight(30);
    scoreValueLabel->setStyleSheet("font-weight: bold; font-size: 26px;");
    scoreValueLabel->setAlignment(Qt::AlignCenter);

    //add content to layouts
    scoreLayout->addWidget(scoreTxtLabel);
    scoreLayout->addWidget(scoreValueLabel);
    containerLayout->addWidget(board);
    containerLayout->addWidget(scoreWidget);

    //Align content into layout
    scoreLayout->setAlignment(Qt::AlignCenter);

    //Apply layout
    scoreWidget->setLayout(scoreLayout);

    //define central widget
    setCentralWidget(container);

    setWindowTitle("Tetris");
    resize(400,500);
}

MainWindow::~MainWindow(){}
