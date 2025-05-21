#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);

signals:
    void playBtnClicked();
    void goToGame();
    void goToSettings();
};

#endif // MENUWIDGET_H
