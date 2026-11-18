#pragma once
#include <QWidget>
#include <QTimer>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QVector>
#include <QPair>
#include "automaton.h"

class GridWidget : public QWidget {
    Q_OBJECT
public:
    GridWidget(int w, int h, QWidget *parent = nullptr);
    bool** current;
    int W, H;
    void setCells(bool** cells) { current = cells; update(); }

signals:
    void cellClicked(int x, int y);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

class GameWindow : public QWidget {
    Q_OBJECT
public:
    GameWindow(int w, int h, int type, unsigned int birth, unsigned int survive, QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void toggleRun();
    void stepForward();
    void stepBack();
    void openMaps();
    void returnToMenu();
    void updateSpeed(int value);
    void onCellClicked(int x, int y);
    void clearAll();
    void randomFill(); // New slot

private:
    int W, H;
    bool** current;
    bool** next;
    Automaton* automaton;
    QTimer* timer;
    QPushButton* btnStart;
    QSpinBox* speedSpin;
    GridWidget* gridWidget;

    struct Snapshot { bool** data; };
    Snapshot history[50];
    int histCount = 0;

    void initGrid();
    void saveSnapshot();
    void applyPattern(const QVector<QPair<int,int>>& cells);
};