#pragma once
#include <QDialog>
#include <QListWidget>
#include <QVector>
#include <QPair>

class MapsWindow : public QDialog {
    Q_OBJECT
public:
    explicit MapsWindow(QWidget *parent = nullptr);
    QVector<QPair<int,int>> selectedPattern() const;

private:
    QListWidget *listWidget;
    QVector<QPair<int,int>> currentPattern;
};