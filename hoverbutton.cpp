#include "hoverbutton.h"
#include <QEvent>

HoverButton::HoverButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    hoverSound.setSource(QUrl::fromLocalFile("C:/Users/fpier/Documents/QtCreator/tetris/sound/hover_sound.wav"));
    hoverSound.setVolume(0.5); // volume from 0.0 to 1.0
}

void HoverButton::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    if (hoverSound.isLoaded()) {
        hoverSound.play();
    }
}
