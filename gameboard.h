#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QBoxLayout>

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
    QVBoxLayout* getVScoreLayout(){return vScoreLayout;}
    void pause();

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
    QVBoxLayout *vScoreLayout;

signals:
};

#endif // GAMEBOARD_H
