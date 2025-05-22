#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QKeySequenceEdit>

class GameBoard;

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = nullptr, GameBoard* board = nullptr);
    void setLeftKeyField(QKeySequence val);
    void setRightKeyField(QKeySequence val);
    void setDownKeyField(QKeySequence val);
    void setRotateKeyField(QKeySequence val);
    void setDropKeyField(QKeySequence val);
    void setBreakKeyField(QKeySequence val);

private:
    QKeySequenceEdit* leftKeyField;
    QKeySequenceEdit* rightKeyField;
    QKeySequenceEdit* downKeyField;
    QKeySequenceEdit* rotateKeyField;
    QKeySequenceEdit* dropKeyField;
    QKeySequenceEdit* breakKeyField;

signals:
    void cancelClicked();
};

#endif // SETTINGS_H
