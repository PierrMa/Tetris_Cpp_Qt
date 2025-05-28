#include "gameboard.h"

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
#include <QLineEdit>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>

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

    //move down the tetromino through the time
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameBoard::moveDown);

    //take keyboard press into account
    setFocusPolicy(Qt::StrongFocus);

    QTimer* speedTimer = new QTimer(this);
    connect(speedTimer, &QTimer::timeout, [=](){
        if(timerPeriod>100){
            timerPeriod -= 100;
            gameTimer->start(timerPeriod);
        }
    });
    speedTimer->start(180000);
}

void GameBoard::drawBlock(QPainter &painter, int x, int y, int size, QColor color) {
    // --- Gradient (inner depth effect) ---
    QLinearGradient gradient(x, y, x + size, y + size);
    gradient.setColorAt(0, color.lighter(130));  // brighter up left
    gradient.setColorAt(1, color.darker(130));   // darker bottom right

    painter.setBrush(QBrush(gradient));
    painter.setPen(Qt::NoPen);
    painter.drawRect(x, y, size, size);

    // Colors for 3D effect
    QColor light = color.lighter(150); // illuminated edge
    QColor dark  = color.darker(150); // shaded edge

    // Draw edges
    painter.setPen(light);
    painter.drawLine(x, y, x + size - 1, y); // up
    painter.drawLine(x, y, x, y + size - 1); // left

    painter.setPen(dark);
    painter.drawLine(x + 1, y + size - 1, x + size - 1, y + size - 1); // bottom
    painter.drawLine(x + size - 1, y + 1, x + size - 1, y + size - 1); // right
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
        deleteRow(); //check if there is a row to delete
        generateTetromino(); //next tetromino
    }else{
        actuel->pos.setX(newPosX); //move down the referent point of the tetromino of 1 block
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
        update();//update the image
    }
}

void GameBoard::turn(){
    bool isO = true;
    for(int i=0;i<4;i++){
        if(actuel->forme[i]!=forme::o[i]){
            isO = false;
            break;
        }
    }
    if(!isO){
        QPoint rotated[4]; //present tetromino with a -90° rotation
        //Process the coordinates of each point of the tetromino
        for(int i=0;i<4;i++){
            int relX = actuel->forme[i].x() - actuel->forme[4].x();
            int relY = actuel->forme[i].y() - actuel->forme[4].y();
            rotated[i].setX(actuel->forme[4].x() + relY);
            rotated[i].setY(actuel->forme[4].y() - relX);
        }

        bool collision = false;
        //detect collision
        for(int i=0;i<4;i++){
            int x = actuel->pos.x()+rotated[i].x();
            int y = actuel->pos.y()+rotated[i].y();
            if((y<0)||(y>cols-1)||(x<0)||(x>rows-1)||grid[x][y].isValid())
            {
                collision = true;
                break;
            }
        }
        if(!collision){
            for(int i=0;i<4;i++){ //apply rotation
                actuel->forme[i]=rotated[i];
            }
            update();//update the image
        }
    }
}

void GameBoard::drop(){
    bool collision = false;
    int tempRow = actuel->pos.x();
    while(!collision){
        for(int i=0;i<4;i++){
            int x = tempRow + actuel->forme[i].x();
            int y = actuel->pos.y() + actuel->forme[i].y();
            if(x>rows-1||grid[x][y].isValid()){
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

        //manage game over task (check if the scor is among the 10 best scores and save it if so, ask pseudo, display the pop up)
        emit gameOver();
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

    timerPeriod = 1000; //re-initialize timer
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
    QPushButton *saveBtn = new QPushButton("Save", pauseMenu);
    saveBtn->setFixedWidth(120);
    connect(saveBtn, &QPushButton::clicked, this, &GameBoard::saveSession);
    QPushButton *quitBtn = new QPushButton("Quit", pauseMenu);
    quitBtn->setFixedWidth(120);
    connect(quitBtn, &QPushButton::clicked, [=]() {
        qApp->quit();
    });

    QVBoxLayout *menuLayout = new QVBoxLayout();
    menuLayout->addWidget(resumeBtn);
    menuLayout->addWidget(restartBtn);
    menuLayout->addWidget(menuBtn);
    menuLayout->addWidget(saveBtn);
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

void GameBoard::displayGameOverPopUp(){
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
}

void GameBoard::displayWinnerPopUp(){
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Congratulations");
    QLabel* label = new QLabel("Congratulations, you are in the top 10!\nEnter your pseudo:",this);
    label->setAlignment(Qt::AlignCenter);
    QLineEdit* pseudoLineEdit = new QLineEdit(this);
    QPushButton* acceptBtn = new QPushButton("OK",this);
    acceptBtn->setFixedWidth(100);
    connect(acceptBtn,&QPushButton::clicked,dialog,&QDialog::accept);
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(label);
    layout->addWidget(pseudoLineEdit);
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(acceptBtn);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);

    dialog->exec(); //display the pop up

    m_mainwindow->setPseudo(pseudoLineEdit->text());//change the pseudo in mainwindow
}

void GameBoard::saveSession(){
    //save the state of the grid
    QJsonArray array;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            QColor caseColor = grid[i][j];
            QPoint caseCoordinates = QPoint(i,j);
            bool isValid = grid[i][j].isValid();
            QJsonObject obj;
            obj["valid"] = isValid;
            qDebug()<<"grid["<<i<<"]["<<j<<"] valid?: "<<isValid;
            if(isValid){
                obj["r"] = caseColor.red();
                obj["g"] = caseColor.green();
                obj["b"] = caseColor.blue();
            }
            obj["row"] = caseCoordinates.x();
            obj["col"] = caseCoordinates.y();
            array.append(obj);
        }
    }

    //save the current tetromino and its position
    QJsonObject tetrominoObj;
    for(int i=0;i<5;i++){
        QString pointAxis = "p"+QString::number(i)+"x";
        QString pointOrdinate ="p"+QString::number(i)+"y";
        tetrominoObj[pointAxis] = actuel->forme[i].x();
        tetrominoObj[pointOrdinate] = actuel->forme[i].y();
    }
    tetrominoObj["r"] = actuel->color.red();
    tetrominoObj["g"] = actuel->color.green();
    tetrominoObj["b"] = actuel->color.blue();
    tetrominoObj["posX"] = actuel->pos.x();
    tetrominoObj["posY"] = actuel->pos.y();
    array.append(tetrominoObj);

    //save the score
    QJsonObject scoreObj;
    scoreObj["score"] = m_mainwindow->getScore();
    array.append(scoreObj);

    //save timer period
    QJsonObject periodObj;
    periodObj["period"] = timerPeriod;
    array.append(periodObj);

    //save data into json
    QJsonDocument doc(array);
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save JSON File",
        "",
        "JSON Files (*.json);;All Files (*)"
        );

    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

bool GameBoard::loadSession(){
    //select the backup to open
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Choose your back up",
        "",
        "JSON Files (*.json);;All Files (*)"
        );

    if (fileName.isEmpty()) {
        // Clicked on cancel
        return false;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file for reading.");
        return false;
    }

    QByteArray data = file.readAll();
    file.close();
    if (data.trimmed().isEmpty())return false;

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
        qWarning() << "Invalid JSON format in scores.json:" << parseError.errorString();
        return false;
    }

    QJsonArray array = doc.array();
    int nbCase = rows*cols;
    for(int i=0;i<nbCase;i++){
        QJsonObject obj = array[i].toObject();
        if(obj["valid"]==true)
            grid[obj["row"].toInt()][obj["col"].toInt()]=QColor(obj["r"].toInt(),obj["g"].toInt(),obj["b"].toInt());
        else
            grid[obj["row"].toInt()][obj["col"].toInt()]=QColor();
    }

    QJsonObject obj = array[nbCase].toObject();
    for(int i=0;i<5;i++){
        actuel->forme[i] = QPoint(obj["p"+QString::number(i)+"x"].toInt(),obj["p"+QString::number(i)+"y"].toInt());
    }
    actuel->color = QColor(obj["r"].toInt(),obj["g"].toInt(),obj["b"].toInt());
    actuel->pos = QPoint(obj["posX"].toInt(),obj["posY"].toInt());

    obj = array[nbCase+1].toObject();
    m_mainwindow->setScore(obj["score"].toInt());

    obj = array[nbCase+2].toObject();
    timerPeriod = obj["period"].toInt();
    return true;
}
