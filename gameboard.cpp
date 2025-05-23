#include "gameboard.h"
#include "mainwindow.h"

#include <QPainter>
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QBoxLayout>
#include <QSoundEffect>

GameBoard::GameBoard(QWidget *parent, MainWindow* mainwindow)
    : QWidget{parent}, m_mainwindow(mainwindow)
{
    //always keep the grid visible
    setFixedSize(cols * cellSize + 20, rows * cellSize + 20);

    //generate first tetromino
    actuel = new tetromino;
    actuel->pos = QPoint(0,4);
    generateTetromino();

    //initialise an empty grid
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            grid[i][j]= QColor();
        }
    }

    //move down the tetromino throuh the time
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameBoard::moveDown);

    //take keyboard press into account
    setFocusPolicy(Qt::StrongFocus);
}

void GameBoard::drawBlock(QPainter &painter, int x, int y, int size, QColor color) {
    // --- Gradient (effet de profondeur intérieur) ---
    QLinearGradient gradient(x, y, x + size, y + size);
    gradient.setColorAt(0, color.lighter(130));  // plus clair en haut à gauche
    gradient.setColorAt(1, color.darker(130));   // plus foncé en bas à droite

    painter.setBrush(QBrush(gradient));
    painter.setPen(Qt::NoPen);
    painter.drawRect(x, y, size, size);

    // Couleurs pour l'effet 3D
    QColor light = color.lighter(150);  // Bord éclairé
    QColor dark  = color.darker(150);   // Bord ombré

    // Dessiner les bords
    painter.setPen(light);
    painter.drawLine(x, y, x + size - 1, y);            // haut
    painter.drawLine(x, y, x, y + size - 1);            // gauche

    painter.setPen(dark);
    painter.drawLine(x + 1, y + size - 1, x + size - 1, y + size - 1); // bas
    painter.drawLine(x + size - 1, y + 1, x + size - 1, y + size - 1); // droite
}

void GameBoard::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    for(int y=0;y<rows;y++){
        for(int x=0;x<cols;x++){
            painter.drawRect(x*cellSize+offSetX, y*cellSize+offSetY, cellSize, cellSize);
        }
    }

    if(actuel!=nullptr){
        for(int i=0;i<4;i++){
            int x = (actuel->forme[i].y()+actuel->pos.y())*cellSize+offSetX;
            int y = (actuel->forme[i].x()+actuel->pos.x())*cellSize+offSetY;
            drawBlock(painter,x,y,cellSize,actuel->color);
        }
    }

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(grid[i][j].isValid()){
                int x = j*cellSize+offSetX;
                int y = i*cellSize+offSetY;
                drawBlock(painter,x,y,cellSize,grid[i][j]);
            }
        }
    }
}

void GameBoard::generateTetromino(){
    int r = QRandomGenerator::securelySeeded().generate()%256;
    int g = QRandomGenerator::securelySeeded().generate()%256;
    int b = QRandomGenerator::securelySeeded().generate()%256;
    actuel->color = QColor(r,g,b);
    actuel->pos = QPoint(0,4);

    int actuelFormeEnum = QRandomGenerator::securelySeeded().generate()%7;
    forme myForme;
    switch(actuelFormeEnum){
        case formeEnum::l :
            std::copy(std::begin(myForme.l),std::end(myForme.l),actuel->forme);
            break;
        case formeEnum::t :
            std::copy(std::begin(myForme.t),std::end(myForme.t),actuel->forme);
            break;
        case formeEnum::z :
            std::copy(std::begin(myForme.z),std::end(myForme.z),actuel->forme);
            break;
        case formeEnum::s :
            std::copy(std::begin(myForme.s),std::end(myForme.s),actuel->forme);
            break;
        case formeEnum::L :
            std::copy(std::begin(myForme.L),std::end(myForme.L),actuel->forme);
            break;
        case formeEnum::J :
            std::copy(std::begin(myForme.J),std::end(myForme.J),actuel->forme);
            break;
        case formeEnum::o :
            std::copy(std::begin(myForme.o),std::end(myForme.o),actuel->forme);
            break;
        default:
            std::copy(std::begin(myForme.o),std::end(myForme.o),actuel->forme);
    }

    gameOverCheck();
}

void GameBoard::moveDown(){
    int newPosX = actuel->pos.x()+1;
    bool collision = false;
    //detect collision
    for(int i=0;i<4;i++){
        int x = newPosX+actuel->forme[i].x();
        int y = actuel->pos.y()+actuel->forme[i].y();
        if((x>rows-1)||grid[x][y].isValid())
        {
            collision = true;
            break;
        }
    }
    if(collision){
        for(int i=0;i<4;i++){
            int x = actuel->pos.x()+actuel->forme[i].x();
            int y = actuel->pos.y()+actuel->forme[i].y();
            grid[x][y] = actuel->color;
        }
        generateTetromino(); //next tetromino
    }else{
        actuel->pos.setX(newPosX); //move down the referent point of the tetromino of 1 block
        deleteRow(); //check if there is a row to delete
        update(); //update the image
    }
}

void GameBoard::moveRight(){
    int newY = actuel->pos.y()+1;
    bool collision = false;
    //detect collision
    for(int i=0;i<4;i++){
        int x = actuel->pos.x()+actuel->forme[i].x();
        int y = newY+actuel->forme[i].y();
        if((y>cols-1)||grid[x][y].isValid())
        {
            collision = true;
            break;
        }
    }
    if(!collision){
        actuel->pos.setY(newY); //move right the referent point of the tetromino of 1 block
        deleteRow(); //check if there is a row to delete
        update();//update the image
    }

}

void GameBoard::moveLeft(){
    int newY = actuel->pos.y()-1;
    bool collision = false;
    //detect collision
    for(int i=0;i<4;i++){
        int x = actuel->pos.x()+actuel->forme[i].x();
        int y = newY+actuel->forme[i].y();
        if((y<0)||grid[x][y].isValid())
        {
            collision = true;
            break;
        }
    }
    if(!collision){
        actuel->pos.setY(newY); //move left the referent point of the tetromino of 1 block
        deleteRow(); //check if there is a row to delete
        update();//update the image
    }
}

void GameBoard::turn(){
    bool collision = false;
    //detect collision
    for(int i=0;i<4;i++){
        int x = actuel->pos.x()+actuel->forme[i].y();
        int y = actuel->pos.y()-(actuel->forme[i].x());
        if((y<0)||(y>cols-1)||(x<0)||(x>rows-1)||grid[x][y].isValid())
        {
            collision = true;
            break;
        }
    }
    if(!collision){
        for(int i=0;i<4;i++){
            int tempX = actuel->forme[i].x();
            int tempY = actuel->forme[i].y();
            actuel->forme[i].setX(tempY);
            actuel->forme[i].setY(-tempX);
        }
        update();//update the image
    }
}

void GameBoard::drop(){
    bool collision = false;
    int tempRow = actuel->pos.x();
    while(!collision){
        for(int i=0;i<4;i++){
            int x = tempRow + actuel->forme[i].x();
            int y = actuel->pos.y() + actuel->forme[i].y();
            if(grid[x][y].isValid()||x>rows-1){
                collision = true;
                break;
            }
        }
        if(!collision) tempRow++;
    }

    actuel->pos.setX(tempRow-1);
    for(int i =0;i<4;i++){
        int x = actuel->pos.x()+actuel->forme[i].x();
        int y = actuel->pos.y()+actuel->forme[i].y();
        grid[x][y] = actuel->color;
    }
    deleteRow(); //check if there is a row to delete
    generateTetromino();
}

void GameBoard::keyPressEvent(QKeyEvent *event){
    if(event->key() == rightKey[0]){
        moveRight();
    }else if(event->key() == leftKey[0]){
        moveLeft();
    }else if(event->key() == downKey[0]){
        moveDown();
    }else if(event->key() == rotateKey[0]){
        turn();
    }else if(event->key() == dropKey[0]){
        drop();
    }else if(event->key() == breakKey[0]){
        pause();
    }
}

void GameBoard::deleteRow(){
    bool isEmpty = false;

    for(int i=0;i<rows;i++){
        isEmpty = false;
        for(int j=0;j<cols;j++){
            if(!grid[i][j].isValid()){
                isEmpty = true;
                break;
            }
        }
        if(!isEmpty){ //if there is que complete row, delete it
            for(int k=i;k>0;k--){ //shift above rows of one row down
                std::copy(std::begin(grid[k-1]), std::end(grid[k-1]), grid[k]);
            }
            for(int k=0;k<cols;k++){ //the upper row is empty
                grid[0][k] = QColor();
            }
            emit rowDeleted(); //signal to increment the score

            //add crash sound effect
            QSoundEffect* crashSound = new QSoundEffect(this);
            crashSound->setSource(QUrl("qrc:/sound/crash.wav"));
            crashSound->setVolume(1);
            crashSound->play();
        }
    }
}

void GameBoard::gameOverCheck(){
    bool collision = false;
    for(int i=0;i<4;i++){
        int x = actuel->pos.x()+actuel->forme[i].x();
        int y = actuel->pos.y()+actuel->forme[i].y();
        if(grid[x][y].isValid()){
            collision = true;
            break;
        }
    }
    if(collision){
        gameTimer->stop();
        m_mainwindow->stopMusic();
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Game Over");
        dialog->setModal(true);  // bloque les autres interactions

        QLabel *label = new QLabel("Game Over");
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("color: rgb(255,0,0); font-size: 26px; font-weight: bold");

        QPushButton *tryAgainBtn = new QPushButton("Try Again");
        QPushButton *menuBtn     = new QPushButton("Menu");
        QPushButton *quitBtn     = new QPushButton("Quit");

        // Connexions aux actions
        connect(tryAgainBtn, &QPushButton::clicked, [=](){
            dialog->accept(); //close the pop up
            tryAgain(); //restart the game
        });

        connect(menuBtn, &QPushButton::clicked, [=]() {
            dialog->accept();
            clearBoard();
            emit backToMenu();
        });

        connect(quitBtn, &QPushButton::clicked, [=]() {
            qApp->quit();
        });

        //Add vertical layout
        QVBoxLayout *layout = new QVBoxLayout(dialog);
        layout->addWidget(label);

        //Add horizontal Layout for each button and add it to the vertical layout
        auto addCenteredButton = [&](QPushButton *button) {
            button->setFixedWidth(120); // fixe width for each button
            QHBoxLayout *hLayout = new QHBoxLayout;
            hLayout->addStretch(); //left space
            hLayout->addWidget(button);  //the button in the middle
            hLayout->addStretch(); // right space
            layout->addLayout(hLayout); //add the horizontal layout to the vertical layout
        };
        addCenteredButton(tryAgainBtn);
        addCenteredButton(menuBtn);
        addCenteredButton(quitBtn);

        dialog->setLayout(layout); //apply the main layout
        dialog->show(); //display the pop up
        connect(dialog,&QDialog::rejected,this,&GameBoard::tryAgain);

        //play game over sound effect
        QSoundEffect* gameOverSound = new QSoundEffect(this);
        gameOverSound->setSource(QUrl("qrc:/sound/game_over_sound.wav"));
        gameOverSound->setVolume(1);
        gameOverSound->play();
    }else update();
}

void GameBoard::clearBoard(){
    //clear the grid
    for(int x=0;x<rows;x++){
        for(int y=0;y<cols;y++){
            grid[x][y] = QColor();
        }
    }

    emit resetScore(); //reset the score
}

void GameBoard::tryAgain(){
    clearBoard();//clear the grid and reset the score

    generateTetromino(); //generate a new tetromino

    gameTimer->start(timerPeriod); //restart timer

    m_mainwindow->playMusicFromTheStart();
}

void GameBoard::pause(){
    gameTimer->stop();
    m_mainwindow->pauseMusic();
    QDialog *pauseMenu = new QDialog(this);
    pauseMenu->setWindowTitle("Pause");
    pauseMenu->setModal(true);

    QPushButton *resumeBtn = new QPushButton("Resume", pauseMenu);
    resumeBtn->setFixedWidth(120);
    connect(resumeBtn, &QPushButton::clicked, [=](){
        pauseMenu->accept();
        resume();
    });
    QPushButton *restartBtn = new QPushButton("Restart", pauseMenu);
    restartBtn->setFixedWidth(120);
    connect(restartBtn, &QPushButton::clicked, [=](){
        pauseMenu->accept();
        tryAgain();
    });
    QPushButton *menuBtn = new QPushButton("Menu", pauseMenu);
    menuBtn->setFixedWidth(120);
    connect(menuBtn, &QPushButton::clicked, [=]() {
        pauseMenu->accept();
        emit backToMenu();
    });
    QPushButton *quitBtn = new QPushButton("Quit", pauseMenu);
    quitBtn->setFixedWidth(120);
    connect(quitBtn, &QPushButton::clicked, [=]() {
        qApp->quit();
    });

    QVBoxLayout *menuLayout = new QVBoxLayout();
    menuLayout->addWidget(resumeBtn);
    menuLayout->addWidget(restartBtn);
    menuLayout->addWidget(menuBtn);
    menuLayout->addWidget(quitBtn);
    pauseMenu->setLayout(menuLayout);
    pauseMenu->show();
    connect(pauseMenu,&QDialog::rejected,this,&GameBoard::resume);
}

void GameBoard::startTimer(){
    gameTimer->start(timerPeriod); //choose the period to move the tetromino (ms)
}

void GameBoard::resume(){
    gameTimer->start(timerPeriod);
    m_mainwindow->playMusic();
}
