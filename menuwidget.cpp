#include "menuwidget.h"
#include "hoverbutton.h"

#include <QCoreApplication>
#include <QBoxLayout>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget{parent}
{
    //buttons
    HoverButton* playBtn = new HoverButton("Play", this);
    playBtn->setFixedHeight(100);
    playBtn->setFixedWidth(300);
    HoverButton* settingBtn = new HoverButton("Settings", this);
    settingBtn->setFixedHeight(100);
    settingBtn->setFixedWidth(300);
    HoverButton* quitBtn = new HoverButton("Quit", this);
    quitBtn->setFixedHeight(100);
    quitBtn->setFixedWidth(300);

    //connexions
    connect(playBtn, &QPushButton::clicked, [=](){
        emit playBtnClicked();
        emit goToGame();
    });
    connect(settingBtn, &QPushButton::clicked, [=](){
        emit goToSettings();
    });
    connect(quitBtn, &QPushButton::clicked, [=](){
        qApp->quit();
    });

    //Layout
    QVBoxLayout* btnLayout = new QVBoxLayout(this);
    btnLayout->setAlignment(Qt::AlignCenter);
    btnLayout->addWidget(playBtn);
    btnLayout->addWidget(settingBtn);
    btnLayout->addWidget(quitBtn);

    //style
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(Qt::green));
    this->setPalette(palette);
    this->setStyleSheet("font-weight: Bold; font-size: 22px; color: grey");
}
