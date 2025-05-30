#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QLabel>

class GameBoard;
class MenuWidget;
class Settings;
class Scores;

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
    void setPseudo(QString pseudo){ m_pseudo = pseudo;}
    int getScore()const{return gameScore;}
    void setScore(int val);
private:
    QStackedWidget* stack;
    GameBoard* board;
    MenuWidget* menu;
    Settings* settings;
    int gameScore = 0;
    QMediaPlayer* bgMusicPlayer;
    QAudioOutput* bgAudioOutput;
    Scores* score;
    QString m_pseudo;
    QLabel* scoreValueLabel;
public slots:
    void displayMenu();
};
#endif // MAINWINDOW_H
