#include "settings.h"
#include "gameboard.h"

#include <QLabel>
#include <QFormLayout>
#include <QKeySequenceEdit>
#include <QPushButton>
#include <QSizePolicy>

Settings::Settings(QWidget *parent, GameBoard* board)
    : QWidget{parent}
{
    int formFieldHeight = 25;
    int formFieldWidth = 50;
    QKeySequenceEdit* leftKey = new QKeySequenceEdit(board->getLeftCmd(),this);
    leftKey->setFixedHeight(formFieldHeight);
    leftKey->setFixedWidth(formFieldWidth);
    QKeySequenceEdit* rightKey = new QKeySequenceEdit(board->getRightCmd(),this);
    rightKey->setFixedHeight(formFieldHeight);
    rightKey->setFixedWidth(formFieldWidth);
    QKeySequenceEdit* downKey = new QKeySequenceEdit(board->getDownCmd(),this);
    downKey->setFixedHeight(formFieldHeight);
    downKey->setFixedWidth(formFieldWidth);
    QKeySequenceEdit* rotateKey = new QKeySequenceEdit(board->getRotateCmd(),this);
    rotateKey->setFixedHeight(formFieldHeight);
    rotateKey->setFixedWidth(formFieldWidth);
    QKeySequenceEdit* dropKey = new QKeySequenceEdit(board->getDropCmd(),this);
    dropKey->setFixedHeight(formFieldHeight);
    dropKey->setFixedWidth(formFieldWidth);
    QKeySequenceEdit* breakKey = new QKeySequenceEdit(board->getBreakCmd(),this);
    breakKey->setFixedHeight(formFieldHeight);
    breakKey->setFixedWidth(formFieldWidth);

    QFont labelFont("Segoe", 12, QFont::Bold, false);
    QLabel* leftLabel = new QLabel("Left",this);
    leftLabel->setFont(labelFont);
    QLabel* rightLabel = new QLabel("Right",this);
    rightLabel->setFont(labelFont);
    QLabel* downLabel = new QLabel("Down",this);
    downLabel->setFont(labelFont);
    QLabel* rotateLabel = new QLabel("Rotate",this);
    rotateLabel->setFont(labelFont);
    QLabel* dropLabel = new QLabel("Drop",this);
    dropLabel->setFont(labelFont);
    QLabel* breakLabel = new QLabel("Break",this);
    breakLabel->setFont(labelFont);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(leftLabel,leftKey);
    formLayout->addRow(rightLabel,rightKey);
    formLayout->addRow(downLabel,downKey);
    formLayout->addRow(rotateLabel,rotateKey);
    formLayout->addRow(dropLabel,dropKey);
    formLayout->addRow(breakLabel,breakKey);
    formLayout->setFormAlignment(Qt::AlignCenter);
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    int btnHeight = 30;
    int btnWidth = 50;
    QPushButton* saveBtn = new QPushButton("Save",this);
    saveBtn->setFixedHeight(btnHeight);
    saveBtn->setFixedWidth(btnWidth);
    QPushButton* cancelBtn = new QPushButton("Cancel",this);
    cancelBtn->setFixedHeight(btnHeight);
    cancelBtn->setFixedWidth(btnWidth);
    connect(cancelBtn,&QPushButton::clicked,[=](){
        emit cancelClicked();
    });

    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setAlignment(Qt::AlignCenter);
}
