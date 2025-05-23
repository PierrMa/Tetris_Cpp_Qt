#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

class GameBoard;
class QKeySequenceEdit;
class QPushButton;

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
    void setTxtMuteBtn(QString);

private:
    QKeySequenceEdit* leftKeyField;
    QKeySequenceEdit* rightKeyField;
    QKeySequenceEdit* downKeyField;
    QKeySequenceEdit* rotateKeyField;
    QKeySequenceEdit* dropKeyField;
    QKeySequenceEdit* breakKeyField;
    QPushButton* muteBtn;

signals:
    void cancelClicked();
    void muteBtnCliked();
};

#endif // SETTINGS_H
