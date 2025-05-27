#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);

signals:
    void playBtnClicked(); //to start the timer
    void goToGame(); //to do all necessary setup to go to the game page
    void goToSettings();
    void goToScores();
    void loadBackUp();
};

#endif // MENUWIDGET_H
