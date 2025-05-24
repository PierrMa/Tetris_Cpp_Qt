#include "hoverbutton.h"
#include <QEvent>
#include <QSoundEffect>

HoverButton::HoverButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    hoverSound = new QSoundEffect(this);
    hoverSound->setSource(QUrl("qrc:/sound/hover_sound.wav"));
    hoverSound->setVolume(0.5); // volume from 0.0 to 1.0
}

void HoverButton::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    if (hoverSound->isLoaded()) {
        hoverSound->play();
    }
}
