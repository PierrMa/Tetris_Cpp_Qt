#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "mainwindow.h"
#include <QWidget>
#include <QKeySequence>

enum formeEnum{
    l, t, z, s, L, J, o
};

typedef struct forme{
    //the fifth QPoint of each QPoint array is the center of rotation
    static constexpr QPoint l[5] = {{0,0},{1,0},{2,0},{3,0},{1,0}};
    static constexpr QPoint t[5] = {{0,0},{1,0},{1,1},{2,0},{1,0}};
    static constexpr QPoint z[5] = {{0,1},{1,0},{1,1},{2,0},{1,1}};
    static constexpr QPoint s[5] = {{0,0},{1,0},{1,1},{2,1},{1,0}};
    static constexpr QPoint L[5] = {{0,0},{1,0},{2,0},{2,1},{1,0}};
    static constexpr QPoint J[5] = {{0,1},{1,1},{2,0},{2,1},{1,1}};
    static constexpr QPoint o[5] = {{0,0},{1,0},{0,1},{1,1},{0,0}};
}forme;

typedef struct tetromino{
    QPoint forme[5];
    QColor color;
    QPoint pos;
}tetromino;

class GameBoard : public QWidget
{
    Q_OBJECT
public:
    explicit GameBoard(QWidget *parent = nullptr, MainWindow* mainwindow = nullptr);
    ~GameBoard(){delete actuel;}
    void drawBlock(QPainter &painter, int x, int y, int size, QColor color) ;
    void paintEvent(QPaintEvent *) override;
    void generateTetromino();
    void moveDown();
    void moveRight();
    void moveLeft();
    void turn();
    void drop();
    void keyPressEvent(QKeyEvent *) override;
    void deleteRow();
    void gameOverCheck();
    void tryAgain();
    void pause();
    void clearBoard();
    QKeySequence getLeftCmd()const{return leftKey;}
    QKeySequence getRightCmd()const{return rightKey;}
    QKeySequence getDownCmd()const{return downKey;}
    QKeySequence getRotateCmd()const{return rotateKey;}
    QKeySequence getDropCmd()const{return dropKey;}
    QKeySequence getBreakCmd()const{return breakKey;}
    void setLeftCmd(QKeySequence newLeftCmd){leftKey = newLeftCmd;}
    void setRightCmd(QKeySequence newRightCmd){rightKey = newRightCmd;}
    void setDownCmd(QKeySequence newDownCmd){downKey = newDownCmd;}
    void setRotateCmd(QKeySequence newRotateCmd){rotateKey = newRotateCmd;}
    void setDropCmd(QKeySequence newDropCmd){dropKey = newDropCmd;}
    void setBreakCmd(QKeySequence newBreakCmd){breakKey = newBreakCmd;}
    void resume();
    void displayGameOverPopUp();
    void displayWinnerPopUp();
    void saveSession();
    bool loadSession();

private:
    static const int rows = 20;
    static const int cols = 10;
    static const int cellSize = 25;
    static const int offSetX = 10;
    static const int offSetY = 10;
    tetromino* actuel = nullptr;
    QColor grid[rows][cols];
    QTimer *gameTimer;
    int timerPeriod = 500;
    QKeySequence leftKey = Qt::Key_Left;
    QKeySequence rightKey = Qt::Key_Right;
    QKeySequence downKey = Qt::Key_Down;
    QKeySequence rotateKey = Qt::Key_Up;
    QKeySequence dropKey = Qt::Key_Space;
    QKeySequence breakKey = Qt::Key_Escape;
    MainWindow* m_mainwindow;

signals:
    void rowDeleted();
    void backToMenu();
    void resetScore();
    void gameOver();

public slots:
    void startTimer();
};

#endif // GAMEBOARD_H
