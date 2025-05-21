#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class GameBoard;
class MenuWidget;
class Settings;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();

private:
    QStackedWidget* stack;
    GameBoard* board;
    MenuWidget* menu;
    Settings* settings;
    int gameScore = 0;
};
#endif // MAINWINDOW_H
