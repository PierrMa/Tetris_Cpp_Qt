#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QKeySequence>

enum formeEnum{
    l, t, z, s, L, J, o
};

typedef struct forme{
    const QPoint l[4] = {{0,0},{1,0},{2,0},{3,0}};
    const QPoint t[4] = {{0,0},{1,0},{1,1},{2,0}};
    const QPoint z[4] = {{0,1},{1,0},{1,1},{2,0}};
    const QPoint s[4] = {{0,0},{1,0},{1,1},{2,1}};
    const QPoint L[4] = {{0,0},{1,0},{2,0},{2,1}};
    const QPoint J[4] = {{0,1},{1,1},{2,0},{2,1}};
    const QPoint o[4] = {{0,0},{1,0},{0,1},{1,1}};
}forme;

typedef struct tetromino{
    QPoint forme[4];
    QColor color;
    QPoint pos;
}tetromino;

class GameBoard : public QWidget
{
    Q_OBJECT
public:
    explicit GameBoard(QWidget *parent = nullptr);
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

private:
    static const int rows = 20;
    static const int cols = 10;
    static const int cellSize = 25;
    static const int offSetX = 10;
    static const int offSetY = 10;
    tetromino* actuel = nullptr;
    QColor grid[rows][cols];
    QTimer *gameTimer;
    static const int timerPeriod = 500;
    QKeySequence leftKey = Qt::Key_Left;
    QKeySequence rightKey = Qt::Key_Right;
    QKeySequence downKey = Qt::Key_Down;
    QKeySequence rotateKey = Qt::Key_Up;
    QKeySequence dropKey = Qt::Key_Space;
    QKeySequence breakKey = Qt::Key_Escape;

signals:
    void rowDeleted();
    void backToMenu();
    void resetScore();

public slots:
    void startTimer();
};

#endif // GAMEBOARD_H
