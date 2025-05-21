#include "settings.h"

#include <QLabel>
#include <QFormLayout>
#include <QKeySequenceEdit>
#include <QPushButton>

Settings::Settings(QWidget *parent)
    : QWidget{parent}
{
    QKeySequenceEdit* leftKey = new QKeySequenceEdit(this);
    QKeySequenceEdit* rightKey = new QKeySequenceEdit(this);
    QKeySequenceEdit* downKey = new QKeySequenceEdit(this);
    QKeySequenceEdit* rotateKey = new QKeySequenceEdit(this);
    QKeySequenceEdit* dropKey = new QKeySequenceEdit(this);
    QKeySequenceEdit* breakKey = new QKeySequenceEdit(this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow("Left",leftKey);
    formLayout->addRow("Right",rightKey);
    formLayout->addRow("Down",downKey);
    formLayout->addRow("Rotate",rotateKey);
    formLayout->addRow("Drop",dropKey);
    formLayout->addRow("Break",breakKey);
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    QPushButton* saveBtn = new QPushButton("Save",this);
    QPushButton* cancelBtn = new QPushButton("Cancel",this);
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
