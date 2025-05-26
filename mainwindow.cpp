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
    /********************
     * Background music *
     ********************/
    bgMusicPlayer = new QMediaPlayer(this);
    bgAudioOutput = new QAudioOutput(this);

    bgMusicPlayer->setAudioOutput(bgAudioOutput);
    bgMusicPlayer->setSource(QUrl("qrc:/sound/background_music.mp3"));
    bgAudioOutput->setVolume(50); // volume from 0 to 100
    bgAudioOutput->setMuted(false); //Unmute by default


    /********************
     *      Pages       *
     ********************/
    //Layouts and containers
    stack = new QStackedWidget(this); //contain the different pages
    QWidget *gameContainer = new QWidget(stack);
    board = new GameBoard(gameContainer,this);
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
    setCentralWidget(stack);
    stack->setCurrentWidget(menu);//default page to display
    stack->show();

    /********************
     *    Connexions    *
     ********************/
    //to do when Menu button is clicked
    connect(board,&GameBoard::backToMenu,[=](){
        //page to display if backToMenu signal is emit
        stack->setCurrentWidget(menu);
        stack->show();
    });

    //to do when Play button is clicked
    connect(menu,&MenuWidget::goToGame,[=](){
        playMusic();
        //page to display if goToGame signal is emit
        stack->setCurrentWidget(gameContainer);
        stack->show();
    });

    //to do when Settings button is clicked
    connect(menu,&MenuWidget::goToSettings,[=](){
        //update of keyfields with the actual commands
        settings->setLeftKeyField(board->getLeftCmd());
        settings->setRightKeyField(board->getRightCmd());
        settings->setDownKeyField(board->getDownCmd());
        settings->setRotateKeyField(board->getRotateCmd());
        settings->setDropKeyField(board->getDropCmd());
        settings->setBreakKeyField(board->getBreakCmd());

        //page to display if goToSettings signal is emit
        stack->setCurrentWidget(settings);
        stack->show();
    });

    //to do when Cancel button (from Settings page) is clicked
    connect(settings,&Settings::backToMenu,[=](){
        //page to display if cancelClicked signal is emit
        stack->setCurrentWidget(menu);
        stack->show();
    });

    //to do when Play button is clicked
    connect(menu, &MenuWidget::playBtnClicked, board, &GameBoard::startTimer);

    //to do when resetScore signal is emitted (after a clik on Restart or Try Again or Menu)
    connect(board, &GameBoard::resetScore,[=](){
        gameScore = 0;
        scoreValueLabel->setText(QString::number(gameScore));
    });

    //to do when Mute button is clicked
    connect(settings, &Settings::muteBtnCliked,[=](){
        if(bgAudioOutput->isMuted()){
            bgAudioOutput->setMuted(false);
            settings->setTxtMuteBtn("Mute");
        }else{
            bgAudioOutput->setMuted(true);
            settings->setTxtMuteBtn("Unmute");
        }
    });

    /**********************
     * Load configuration
     *********************/
    settings->loadSettings(board);
}

MainWindow::~MainWindow(){}

void MainWindow::playMusic(){
    bgMusicPlayer->play();

    //Loop the background music
    connect(bgMusicPlayer, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            bgMusicPlayer->setPosition(0);
            bgMusicPlayer->play();
        }
    });
}

void MainWindow::pauseMusic(){
    bgMusicPlayer->pause();
}

void MainWindow::stopMusic(){
    bgMusicPlayer->stop();
}

void MainWindow::playMusicFromTheStart(){
    bgMusicPlayer->setPosition(0);
    playMusic();
}
