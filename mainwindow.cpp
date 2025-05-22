#include "mainwindow.h"
#include "gameboard.h"
#include "menuwidget.h"
#include "settings.h"

#include <QBoxLayout>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent)
{    
    //Layouts and containers
    stack = new QStackedWidget(this); //contain the different pages
    QWidget *gameContainer = new QWidget(stack);
    board = new GameBoard(gameContainer);
    menu = new MenuWidget(stack);
    settings = new Settings(this,board);
    stack->addWidget(gameContainer);
    stack->addWidget(menu);
    stack->addWidget(settings);
    QHBoxLayout *gameContainerLayout = new QHBoxLayout(gameContainer); //main layout
    QVBoxLayout *scoreLayout = new QVBoxLayout(); //Layout for the score

    //Score Labels
    QWidget *scoreWidget = new QWidget(gameContainer);
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
    gameContainerLayout->addWidget(board);
    gameContainerLayout->addWidget(scoreWidget);

    //Align content into layout
    scoreLayout->setAlignment(Qt::AlignCenter);

    //Apply layout
    scoreWidget->setLayout(scoreLayout);

    //fixe the window's size
    setWindowTitle("Tetris");
    resize(400,500);

    //increment the score each time a row is completed
    connect(board, &GameBoard::rowDeleted, [=](){
        gameScore += 10;
        scoreValueLabel->setText(QString::number(gameScore));
    });

    //choose the current page to display
    //Default page
    setCentralWidget(stack);
    stack->setCurrentWidget(menu);
    stack->show();
    //page to display if backToMenu signal is emit
    connect(board,&GameBoard::backToMenu,[=](){
        stack->setCurrentWidget(menu);
        stack->show();
    });
    //page to display if goToGame signal is emit
    connect(menu,&MenuWidget::goToGame,[=](){
        stack->setCurrentWidget(gameContainer);
        stack->show();
    });
    //page to display if goToSettings signal is emit
    connect(menu,&MenuWidget::goToSettings,[=](){
        stack->setCurrentWidget(settings);
        stack->show();
    });
    //page to display if cancelClicked signal is emit
    connect(settings,&Settings::cancelClicked,[=](){
        stack->setCurrentWidget(menu);
        stack->show();
    });

    connect(menu, &MenuWidget::playBtnClicked, board, &GameBoard::startTimer);
    connect(board, &GameBoard::resetScore,[=](){
        gameScore = 0;
        scoreValueLabel->setText(QString::number(gameScore));
    });
}

MainWindow::~MainWindow(){}
