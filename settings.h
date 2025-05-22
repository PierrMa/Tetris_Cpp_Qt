#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

class GameBoard;

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr, GameBoard* board = nullptr);
signals:
    void cancelClicked();
};

#endif // SETTINGS_H
