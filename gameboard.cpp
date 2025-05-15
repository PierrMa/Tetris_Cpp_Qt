#include "gameboard.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>

GameBoard::GameBoard(QWidget *parent)
    : QWidget{parent}
{
    //always keep the grid visible
    setFixedSize(cols * cellSize + 20, rows * cellSize + 20);

    //generate firt tetromino
    actuel = new tetromino;
    actuel->pos = QPoint(0,4);
    generateTetromino();

    //move the tetromino
    QTimer *myTimer = new QTimer(this);
    connect(myTimer, &QTimer::timeout, this, &GameBoard::moveDown);
    myTimer->start(500);
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
}

void GameBoard::moveDown(){
    actuel->pos.setX(actuel->pos.x()+1); //move down the referent point of the tetromino of 1 block
    update();//update the image
}
