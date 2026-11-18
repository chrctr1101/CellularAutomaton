#include "mapswindow.h"
#include <QVBoxLayout>
#include <QPushButton>

MapsWindow::MapsWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("SELECT MAP");
    resize(300, 350);
    setStyleSheet(
        "QDialog { background-color: #0a0a12; color: #00ffcc; font-family: 'Consolas', 'Courier New', monospace; }"
    );

    auto layout = new QVBoxLayout(this);
    listWidget = new QListWidget();
    listWidget->addItems({
        "Gosper Glider Gun",
        "Glider",
        "Pulsar",
        "LWSS (Lightweight Spaceship)",
        "Pentadecathlon",
        "Toad"
    });
    listWidget->setStyleSheet(
        "QListWidget { background-color: #12121a; border: 2px solid #00ffcc; border-radius: 6px; padding: 4px; }"
        "QListWidget::item:selected { background-color: #ff00ff; color: #0a0a12; }"
    );
    layout->addWidget(listWidget);

    auto btn = new QPushButton("APPLY");
    btn->setStyleSheet(
        "QPushButton { background-color: #1a1a2e; border: 2px solid #00ff00; border-radius: 6px; padding: 10px; font-weight: bold; }"
        "QPushButton:hover { background-color: #00ff00; color: #0a0a12; }"
    );
    connect(btn, &QPushButton::clicked, this, [this]() {
        int idx = listWidget->currentRow();
        currentPattern.clear();

        if (idx == 0) {
            // Gosper Glider Gun — все координаты положительные
            int gun[][2] = {
                {5,1},{5,2},{6,1},{6,2},
                {3,13},{3,14},{4,12},{4,16},{5,11},{5,17},
                {6,11},{6,15},{6,17},{6,18},{7,11},{7,17},
                {8,12},{8,16},{9,13},{9,14},
                {1,25},{2,23},{2,25},{3,21},{3,22},
                {4,21},{4,22},{5,21},{5,22},{6,23},{6,25},{7,25},
                {3,35},{4,35},{3,36},{4,36}
            };
            for (auto& c : gun) currentPattern.append({c[0], c[1]});
        } 
        else if (idx == 1) {
            // Glider — сдвинем, чтобы все координаты были >= 0
            int g[][2] = {{0,2},{1,2},{2,2},{2,1},{1,0}};
            for (auto& c : g) currentPattern.append({c[0], c[1]});
        } 
        else if (idx == 2) {
            // Pulsar — сдвинем на +3, чтобы не было отрицательных
            int p[][2] = {
                {5,4},{5,5},{5,6},{5,2},{5,1},{5,0},
                {1,4},{1,5},{1,6},{1,2},{1,1},{1,0},
                {4,5},{5,5},{6,5},{2,5},{1,5},{0,5},
                {4,1},{5,1},{6,1},{2,1},{1,1},{0,1}
            };
            for (auto& c : p) currentPattern.append({c[0], c[1]});
        }
        else if (idx == 3) {
            // LWSS (10 клеток, форма космического корабля)
            int lwss[][2] = {
                {1,0}, {4,0},
                {0,1}, {4,1},
                {0,2}, {4,2},
                {0,3}, {1,3}, {2,3}, {3,3}
            };
            for (auto& c : lwss) currentPattern.append({c[0], c[1]});
        }
        else if (idx == 4) {
            // Pentadecathlon — 10-клеточная линия
            int pd[][2] = {
                {0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,8},{0,9}
            };
            for (auto& c : pd) currentPattern.append({c[0], c[1]});
        }
        else if (idx == 5) {
            // Toad
            int toad[][2] = {
                {1,0},{2,0},{3,0},
                {0,1},{1,1},{2,1}
            };
            for (auto& c : toad) currentPattern.append({c[0], c[1]});
        }
        accept();
    });
    layout->addWidget(btn);
}

QVector<QPair<int,int>> MapsWindow::selectedPattern() const {
    return currentPattern;
}