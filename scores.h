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

private:
    QTableWidget* scoreTable;
signals:
};

#endif // SCORES_H
