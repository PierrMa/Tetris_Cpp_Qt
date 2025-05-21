#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class GameBoard;
class MenuWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget *stack;
    GameBoard *board;
    MenuWidget *menu;
    int gameScore = 0;
};
#endif // MAINWINDOW_H
