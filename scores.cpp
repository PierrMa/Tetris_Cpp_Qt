#include "scores.h"
#include "mainwindow.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QBoxLayout>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QPushButton>

Scores::Scores(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* scoreLayout = new QVBoxLayout(this);
    scoreTable = new QTableWidget(10,4,this);
    scoreTable->setHorizontalHeaderLabels(QStringList()<<"Rank"<<"Pseudo"<<"Score"<<"Date");
    scoreTable->verticalHeader()->hide();
    scoreTable->setFixedHeight(327);
    scoreTable->setColumnWidth(0, 80);
    scoreLayout->addWidget(scoreTable);
    scoreLayout->setAlignment(Qt::AlignCenter);

    //rank column
    for(int i=0;i<10;i++){
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(i+1));
        scoreTable->setItem(i,0, item);
        item->setTextAlignment(Qt::AlignCenter);
    }
    //initiate an empty table
    for(int i=0;i<10;i++){
        for(int j=1;j<4;j++){
            scoreTable->setItem(i,j, new QTableWidgetItem());
        }
    }

    //forbid edition and selection
    scoreTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scoreTable->setSelectionMode(QAbstractItemView::NoSelection);

    loadScoresFromFile();//load previous scores

    QPushButton* backBtn = new QPushButton("Back",this);
    connect(backBtn,&QPushButton::clicked,[=](){
        emit backToMenu();
    });
    backBtn->setFixedWidth(100);
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(backBtn);
    btnLayout->addStretch();
    scoreLayout->addLayout(btnLayout);
}

int Scores::getItem(int row, int col){
    QTableWidgetItem* item = scoreTable->item(row,col);
    bool empty = item->text().isEmpty();
    if(!empty) return item->text().toInt();
    else return -1;
}

void Scores::loadScoresFromFile() {
    QFile file("scores.json");
    if (!file.exists()) {
        //qDebug() << "scores.json not found. Starting with an empty score table.";
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //qWarning() << "Failed to open scores.json for reading.";
        return;
    }

    QByteArray data = file.readAll();
    file.close();
    if (data.trimmed().isEmpty()) {
        //qDebug() << "scores.json is empty.";
        return;
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
        //qWarning() << "Invalid JSON format in scores.json:" << parseError.errorString();
        return;
    }

    QJsonArray array = doc.array();

    for (int i = 0; i < array.size() && i < 10; ++i) {
        QJsonObject obj = array[i].toObject();
        QString pseudo = obj["Pseudo"].toString();
        int score = obj["Score"].toInt();
        QString date = obj["Date"].toString();

        QTableWidgetItem* pseudoItem = new QTableWidgetItem(pseudo);
        QTableWidgetItem* scoreItem = new QTableWidgetItem(QString::number(score));
        QTableWidgetItem* dateItem = new QTableWidgetItem(date);
        pseudoItem->setTextAlignment(Qt::AlignCenter);
        scoreItem->setTextAlignment(Qt::AlignCenter);
        dateItem->setTextAlignment(Qt::AlignCenter);

        scoreTable->setItem(i, 1, pseudoItem); // column 1 : pseudo
        scoreTable->setItem(i, 2, scoreItem);  // column 2 : score
        scoreTable->setItem(i, 3, dateItem);   // column 3 : date
    }
}

void Scores::saveScoresToFile() {
    QJsonArray array;

    for (int i = 0; i < 10; ++i) {
        QTableWidgetItem* dateItem = scoreTable->item(i, 3);
        QTableWidgetItem* pseudoItem = scoreTable->item(i, 1);
        QTableWidgetItem* scoreItem = scoreTable->item(i, 2);
        if (pseudoItem && scoreItem && dateItem) {
            QJsonObject obj;
            obj["Pseudo"] = pseudoItem->text();
            obj["Score"] = scoreItem->text().toInt();
            obj["Date"] = dateItem->text();
            array.append(obj);
        }
    }

    QJsonDocument doc(array);
    QFile file("scores.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void Scores::insertNewScore(const QString& pseudo, int score) {
    QList<QJsonObject> scoreList;

    for (int i = 0; i < 10; ++i) {
        QTableWidgetItem* dateItem = scoreTable->item(i, 3);
        QTableWidgetItem* pseudoItem = scoreTable->item(i, 1);
        QTableWidgetItem* scoreItem = scoreTable->item(i, 2);
        if (pseudoItem && scoreItem && dateItem) {
            QJsonObject obj;
            obj["Pseudo"] = pseudoItem->text();
            obj["Score"] = scoreItem->text().toInt();
            obj["Date"] = dateItem->text();
            scoreList.append(obj);
        }
    }

    // Ajouter le nouveau score avec la date actuelle
    QJsonObject newScore;
    newScore["Pseudo"] = pseudo;
    newScore["Score"] = score;
    newScore["Date"] = QDate::currentDate().toString("yyyy-MM-dd");
    scoreList.append(newScore);

    // Trier par score décroissant
    std::sort(scoreList.begin(), scoreList.end(), [](const QJsonObject& a, const QJsonObject& b){
        return a["Score"].toInt() > b["Score"].toInt();
    });

    // Garder les 10 meilleurs
    while (scoreList.size() > 10)
        scoreList.removeLast();

    // Mettre à jour le tableau
    for (int i = 0; i < scoreList.size(); ++i) {
        QJsonObject obj = scoreList[i];
        QTableWidgetItem* dateItem = new QTableWidgetItem(obj["Date"].toString());
        QTableWidgetItem* pseudoItem = new QTableWidgetItem(obj["Pseudo"].toString());
        QTableWidgetItem* scoreItem = new QTableWidgetItem(QString::number(obj["Score"].toInt()));
        pseudoItem->setTextAlignment(Qt::AlignCenter);
        scoreItem->setTextAlignment(Qt::AlignCenter);
        dateItem->setTextAlignment(Qt::AlignCenter);

        scoreTable->setItem(i, 3, dateItem);
        scoreTable->setItem(i, 1, pseudoItem);
        scoreTable->setItem(i, 2, scoreItem);
    }

    // Sauvegarde
    saveScoresToFile();
}
