#ifndef SCORES_H
#define SCORES_H

#include <QWidget>
#include <QTableWidget>

class Scores : public QWidget
{
    Q_OBJECT
public:
    explicit Scores(QWidget *parent = nullptr);
    int getItem(int row, int col);
    void loadScoresFromFile();
    void saveScoresToFile();
    void insertNewScore(const QString& pseudo, int score);

private:
    QTableWidget* scoreTable;
signals:
    void backToMenu();
};

#endif // SCORES_H
