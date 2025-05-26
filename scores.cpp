#include "scores.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QBoxLayout>

Scores::Scores(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* scoreLayout = new QVBoxLayout(this);
    scoreTable = new QTableWidget(10,4,this);
    scoreTable->setHorizontalHeaderLabels(QStringList()<<"Rank"<<"Pseudo"<<"Score"<<"Date");
    //scoreTable->setVerticalHeaderLabels(QStringList()<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10");
    //scoreTable->horizontalHeader()->hide();
    scoreTable->verticalHeader()->hide();
    scoreLayout->addWidget(scoreTable);

    for(int i=0;i<10;i++){
        scoreTable->setItem(i,0, new QTableWidgetItem(QString::number(i+1)));
    }

    scoreTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scoreTable->setSelectionMode(QAbstractItemView::NoSelection);
}

int Scores::getItem(int row, int col){
    bool* ok;
    int item = scoreTable->item(row,col)->text().toInt(ok);
    if(*ok) return item;
    else return -1;
}
