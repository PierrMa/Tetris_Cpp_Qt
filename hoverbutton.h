#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QPushButton>
#include <QWidget>

class QSoundEffect;
class HoverButton : public QPushButton
{
    Q_OBJECT
public:
    explicit HoverButton(const QString &text, QWidget *parent = nullptr);

protected:
    void enterEvent(QEnterEvent *event) override;

private:
    QSoundEffect* hoverSound;
};

#endif // HOVERBUTTON_H
