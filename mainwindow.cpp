#include "mainwindow.h"
#include "gameboard.h"
#include "menuwidget.h"
#include "settings.h"
#include "scores.h"

#include <QBoxLayout>
#include <QDebug>
#include <QSoundEffect>

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
    settings = new Settings(stack,board);
    score = new Scores(stack);
    stack->addWidget(gameContainer);
    stack->addWidget(menu);
    stack->addWidget(settings);
    stack->addWidget(score);
    QHBoxLayout *gameContainerLayout = new QHBoxLayout(gameContainer); //main layout
    QVBoxLayout *scoreLayout = new QVBoxLayout(); //Layout for the score

    //Score Labels
    QWidget *scoreWidget = new QWidget(gameContainer);
    QLabel *scoreTxtLabel = new QLabel("Score", scoreWidget);
    scoreTxtLabel->setFixedHeight(30);
    scoreTxtLabel->setStyleSheet("font-weight: bold; font-size: 26px;");
    scoreValueLabel = new QLabel("0", scoreWidget);
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
        setScore(gameScore+10);
    });

    //choose the current page to display
    setCentralWidget(stack);
    stack->setCurrentWidget(menu);//default page to display
    stack->show();

    /********************
     *    Connexions    *
     ********************/
    //to do when Menu button is clicked
    connect(board,&GameBoard::backToMenu,this,&MainWindow::displayMenu);
    connect(score,&Scores::backToMenu,this,&MainWindow::displayMenu);
    //to do when Cancel button (from Settings page) is clicked
    connect(settings,&Settings::backToMenu,this, &MainWindow::displayMenu);

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

    //to do when Play button is clicked
    connect(menu, &MenuWidget::playBtnClicked, board, &GameBoard::startTimer);

    //to do when resetScore signal is emitted (after a clik on Restart or Try Again or Menu)
    connect(board, &GameBoard::resetScore,[=](){
        setScore(0);
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

    //to do if the game is over
    connect(board,&GameBoard::gameOver, [=](){
        int worstScore;
        int rank=10;
        do{ //looking for the worst registered score from rank 10 to rank 0
            worstScore = score->getItem(rank-1,2); //2 is the column of the score
            rank--;
        }while((rank!=0)&&(worstScore==-1));

        if((worstScore!=-1 && gameScore>worstScore) //if the table is not empty and the score is among the 10 best score
            ||(rank==0 && worstScore==-1)){ //if the table is empty
            //play congrats sound effect
            QSoundEffect* winnerSound = new QSoundEffect(this);
            winnerSound->setSource(QUrl("qrc:/sound/winner_sound.wav"));
            winnerSound->setVolume(1);
            winnerSound->play();

            //ask to enter a pseudo
            board->displayWinnerPopUp();

            //save the new score with the pseudo and the name
            score->insertNewScore(m_pseudo,gameScore);

        }else{
            //play game over sound
            QSoundEffect* gameOverSound = new QSoundEffect(this);
            gameOverSound->setSource(QUrl("qrc:/sound/game_over_sound.wav"));
            gameOverSound->setVolume(1);
            gameOverSound->play();
        }
        //display game over pop up
        board->displayGameOverPopUp();
    });

    //to do when Scores button is clicked
    connect(menu,&MenuWidget::goToScores,[=](){
        stack->setCurrentWidget(score);
        stack->show();
    });

    //to do when Record button is clicked
    connect(menu,&MenuWidget::loadBackUp,[=](){
        bool isLoaded = board->loadSession();
        if(isLoaded){
            playMusic();
            //display game page
            stack->setCurrentWidget(gameContainer);
            stack->show();
            //launch timer
            board->startTimer();
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

void MainWindow::displayMenu(){
    stack->setCurrentWidget(menu);
    stack->show();
}

void MainWindow::setScore(int val){
    gameScore = val;
    scoreValueLabel->setText(QString::number(gameScore));
}
