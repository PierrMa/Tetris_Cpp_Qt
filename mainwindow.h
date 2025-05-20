#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GameBoard;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();

private:
    GameBoard *board;

};
#endif // MAINWINDOW_H
