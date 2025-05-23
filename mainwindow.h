#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMediaPlayer>
#include <QAudioOutput>

class GameBoard;
class MenuWidget;
class Settings;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();
    void playMusic();
    void playMusicFromTheStart();
    void pauseMusic();
    void stopMusic();
private:
    QStackedWidget* stack;
    GameBoard* board;
    MenuWidget* menu;
    Settings* settings;
    int gameScore = 0;
    QMediaPlayer* bgMusicPlayer;
    QAudioOutput* bgAudioOutput;
};
#endif // MAINWINDOW_H
