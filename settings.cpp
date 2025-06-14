#include "settings.h"
#include "gameboard.h"

#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QSizePolicy>
#include <QKeySequenceEdit>
#include <QSettings>

Settings::Settings(QWidget *parent, GameBoard* board)
    : QWidget{parent}
{
    int formFieldHeight = 25;
    int formFieldWidth = 50;
    leftKeyField = new QKeySequenceEdit(this);
    leftKeyField->setFixedHeight(formFieldHeight);
    leftKeyField->setFixedWidth(formFieldWidth);
    rightKeyField = new QKeySequenceEdit(this);
    rightKeyField->setFixedHeight(formFieldHeight);
    rightKeyField->setFixedWidth(formFieldWidth);
    downKeyField = new QKeySequenceEdit(this);
    downKeyField->setFixedHeight(formFieldHeight);
    downKeyField->setFixedWidth(formFieldWidth);
    rotateKeyField = new QKeySequenceEdit(this);
    rotateKeyField->setFixedHeight(formFieldHeight);
    rotateKeyField->setFixedWidth(formFieldWidth);
    dropKeyField = new QKeySequenceEdit(this);
    dropKeyField->setFixedHeight(formFieldHeight);
    dropKeyField->setFixedWidth(formFieldWidth);
    breakKeyField = new QKeySequenceEdit(this);
    breakKeyField->setFixedHeight(formFieldHeight);
    breakKeyField->setFixedWidth(formFieldWidth);

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
    formLayout->addRow(leftLabel,leftKeyField);
    formLayout->addRow(rightLabel,rightKeyField);
    formLayout->addRow(downLabel,downKeyField);
    formLayout->addRow(rotateLabel,rotateKeyField);
    formLayout->addRow(dropLabel,dropKeyField);
    formLayout->addRow(breakLabel,breakKeyField);
    formLayout->setFormAlignment(Qt::AlignCenter);
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    int btnHeight = 30;
    int btnWidth = 50;
    QPushButton* saveBtn = new QPushButton("Save",this);
    saveBtn->setFixedHeight(btnHeight);
    saveBtn->setFixedWidth(btnWidth);
    connect(saveBtn, &QPushButton::clicked,[=](){
        QKeySequence val = leftKeyField->keySequence();
        if(val!=board->getRightCmd() &&val!=board->getDownCmd()
            &&val!=board->getRotateCmd() &&val!=board->getDropCmd()
            &&val!=board->getBreakCmd())
            board->setLeftCmd(val);

        val = rightKeyField->keySequence();
        if(val!=board->getLeftCmd() && val!=board->getDownCmd()
            &&val!=board->getRotateCmd() &&val!=board->getDropCmd()
            &&val!=board->getBreakCmd())
            board->setRightCmd(val);

        val = downKeyField->keySequence();
        if(val!=board->getLeftCmd() && val!=board->getRightCmd()
            &&val!=board->getRotateCmd() &&val!=board->getDropCmd()
            &&val!=board->getBreakCmd())
            board->setDownCmd(val);

        val = rotateKeyField->keySequence();
        if(val!=board->getLeftCmd() && val!=board->getRightCmd()
            &&val!=board->getDownCmd() &&val!=board->getDropCmd()
            &&val!=board->getBreakCmd())
            board->setRotateCmd(val);

        val = dropKeyField->keySequence();
        if(val!=board->getLeftCmd() && val!=board->getRightCmd()
            &&val!=board->getDownCmd() &&val!=board->getRotateCmd()
            &&val!=board->getBreakCmd())
            board->setDropCmd(val);

        val = breakKeyField->keySequence();
        if(val!=board->getLeftCmd() && val!=board->getRightCmd()
            &&val!=board->getDownCmd() &&val!=board->getRotateCmd()
            &&val!=board->getDropCmd())
            board->setBreakCmd(val);

        QSettings settings("PierrElec", "Tetris",this);

        //Saving settings
        settings.setValue("leftKey", leftKeyField->keySequence().toString());
        settings.setValue("rightKey", rightKeyField->keySequence().toString());
        settings.setValue("downKey", downKeyField->keySequence().toString());
        settings.setValue("rotateKey", rotateKeyField->keySequence().toString());
        settings.setValue("dropKey", dropKeyField->keySequence().toString());
        settings.setValue("breakKey", breakKeyField->keySequence().toString());
        settings.setValue("isMuted", (muteBtn->text() == "Unmute"));

        emit backToMenu();
    });
    QPushButton* cancelBtn = new QPushButton("Cancel",this);
    cancelBtn->setFixedHeight(btnHeight);
    cancelBtn->setFixedWidth(btnWidth);
    connect(cancelBtn,&QPushButton::clicked,[=](){
        emit backToMenu();
    });

    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->setAlignment(Qt::AlignCenter);

    muteBtn = new QPushButton("Mute",this);
    connect(muteBtn,&QPushButton::clicked,[=](){
        emit muteBtnCliked();
    });
    muteBtn->setFixedHeight(btnHeight);
    muteBtn->setFixedWidth(btnWidth*2);
    QHBoxLayout* muteLayout = new QHBoxLayout;
    muteLayout->addWidget(muteBtn);
    muteLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(muteLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setAlignment(Qt::AlignCenter);
}

void Settings::setLeftKeyField(QKeySequence val){
    leftKeyField->setKeySequence(val);
}
void Settings::setRightKeyField(QKeySequence val){
    rightKeyField->setKeySequence(val);
}
void Settings::setDownKeyField(QKeySequence val){
    downKeyField->setKeySequence(val);
}
void Settings::setRotateKeyField(QKeySequence val){
    rotateKeyField->setKeySequence(val);
}
void Settings::setDropKeyField(QKeySequence val){
    dropKeyField->setKeySequence(val);
}
void Settings::setBreakKeyField(QKeySequence val){
    breakKeyField->setKeySequence(val);
}

void Settings::setTxtMuteBtn(QString val){
    muteBtn->setText(val);
}

void Settings::loadSettings(GameBoard* board) {
    QSettings settings("PierrElec", "Tetris");

    QKeySequence val;
    val = QKeySequence(settings.value("leftKey", "Left").toString());
    setLeftKeyField(val); board->setLeftCmd(val);

    val = QKeySequence(settings.value("rightKey", "Right").toString());
    setRightKeyField(val); board->setRightCmd(val);

    val = QKeySequence(settings.value("downKey", "Down").toString());
    setDownKeyField(val); board->setDownCmd(val);

    val = QKeySequence(settings.value("rotateKey", "Up").toString());
    setRotateKeyField(val); board->setRotateCmd(val);

    val = QKeySequence(settings.value("dropKey", "Space").toString());
    setDropKeyField(val); board->setDropCmd(val);

    val = QKeySequence(settings.value("breakKey", "Escape").toString());
    setBreakKeyField(val); board->setBreakCmd(val);

    bool muted = settings.value("isMuted", false).toBool();
    if(muted) emit muteBtnCliked(); //change configuration (Unmute to Mute) because default status is Unmute
}
