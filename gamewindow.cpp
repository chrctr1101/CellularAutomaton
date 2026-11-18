#include "gamewindow.h"
#include "mapswindow.h"
#include "mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QLabel>
#include <QRandomGenerator>

// ==================== GridWidget ====================
GridWidget::GridWidget(int w, int h, QWidget *parent) 
    : QWidget(parent), current(nullptr), W(w), H(h) {
    setMinimumSize(100, 100);
    setStyleSheet("background-color: #050510; border: 2px solid #00ffcc; border-radius: 8px;");
}

void GridWidget::paintEvent(QPaintEvent *) {
    if (!current) return;
    QPainter p(this);
    
    int cellSize = qMin(width() / W, height() / H);
    if (cellSize < 1) cellSize = 1;

    int offsetX = (width() - W * cellSize) / 2;
    int offsetY = (height() - H * cellSize) / 2;

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            p.setBrush(current[y][x] ? QColor(0, 255, 140) : QColor(10, 10, 20));
            p.setPen(QPen(QColor(30, 30, 50), 1));
            p.drawRect(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize);
        }
    }
}

void GridWidget::mousePressEvent(QMouseEvent *event) {
    if (!current) return;
    
    int cellSize = qMin(width() / W, height() / H);
    if (cellSize < 1) return;
    
    int offsetX = (width() - W * cellSize) / 2;
    int offsetY = (height() - H * cellSize) / 2;

    int x = (event->pos().x() - offsetX) / cellSize;
    int y = (event->pos().y() - offsetY) / cellSize;

    if (x >= 0 && x < W && y >= 0 && y < H) {
        emit cellClicked(x, y);
    }
}

// ==================== GameWindow ====================
GameWindow::GameWindow(int w, int h, int type, unsigned int birth, unsigned int survive, QWidget *parent)
    : QWidget(parent), W(w), H(h), automaton(nullptr), timer(nullptr), 
      btnStart(nullptr), speedSpin(nullptr), gridWidget(nullptr), histCount(0) {
    
    setWindowTitle("NEON AUTOMATA");
    resize(W * 15 + 300, H * 15 + 200);
    setStyleSheet(
        "QWidget { background-color: #0a0a12; color: #00ffcc; font-family: 'Consolas', 'Courier New', monospace; font-size: 14px; }"
    );

    if (type == 0) automaton = new LifeAutomaton();
    else if (type == 1) automaton = new DropsAutomaton();
    else automaton = new CustomAutomaton(birth, survive);

    initGrid();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::stepForward);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // ==================== Top Bar ====================
    auto topLayout = new QHBoxLayout();
    
    QPushButton *btnMenu = new QPushButton("MENU");
    btnMenu->setStyleSheet(
        "QPushButton { background-color: #1a1a2e; border: 2px solid #ff00ff; border-radius: 6px; padding: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #ff00ff; color: #0a0a12; }"
    );
    connect(btnMenu, &QPushButton::clicked, this, &GameWindow::returnToMenu);
    topLayout->addWidget(btnMenu);

    QPushButton *btnMaps = new QPushButton("MAPS");
    btnMaps->setStyleSheet(
        "QPushButton { background-color: #1a1a2e; border: 2px solid #00ccff; border-radius: 6px; padding: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #00ccff; color: #0a0a12; }"
    );
    connect(btnMaps, &QPushButton::clicked, this, &GameWindow::openMaps);
    topLayout->addWidget(btnMaps);

    QPushButton *btnClear = new QPushButton("CLEAR ALL");
    btnClear->setStyleSheet(
        "QPushButton { background-color: #1a1a2e; border: 2px solid #ff3333; border-radius: 6px; padding: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #ff3333; color: #0a0a12; }"
    );
    connect(btnClear, &QPushButton::clicked, this, &GameWindow::clearAll);
    topLayout->addWidget(btnClear);

    QPushButton *btnRandom = new QPushButton("RANDOM");
    btnRandom->setStyleSheet(
        "QPushButton { background-color: #1a1a2e; border: 2px solid #ffaa00; border-radius: 6px; padding: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #ffaa00; color: #0a0a12; }"
    );
    connect(btnRandom, &QPushButton::clicked, this, &GameWindow::randomFill);
    topLayout->addWidget(btnRandom);
    
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // ==================== Speed Panel ====================
    auto speedLayout = new QHBoxLayout();
    QLabel* lblSpeed = new QLabel("SPEED (ms):");
    lblSpeed->setStyleSheet("color: #ff00ff; font-weight: bold;");
    speedLayout->addWidget(lblSpeed);
    
    speedSpin = new QSpinBox();
    speedSpin->setRange(10, 1000);
    speedSpin->setValue(150);
    speedSpin->setSingleStep(10);
    speedSpin->setStyleSheet(
        "QSpinBox { background-color: #12121a; border: 2px solid #00ffcc; border-radius: 6px; padding: 4px; color: #00ffcc; }"
    );
    connect(speedSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &GameWindow::updateSpeed);
    speedLayout->addWidget(speedSpin);
    
    mainLayout->addLayout(speedLayout);

    // ==================== Grid ====================
    gridWidget = new GridWidget(W, H, this);
    gridWidget->setCells(current);
    connect(gridWidget, &GridWidget::cellClicked, this, &GameWindow::onCellClicked);
    mainLayout->addWidget(gridWidget, 1);

    // ==================== Bottom Bar ====================
    auto btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);

    btnStart = new QPushButton("START");
    btnStart->setStyleSheet(
        "QPushButton { background-color: #1a1a2e; border: 2px solid #00ff00; border-radius: 6px; padding: 10px; font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #00ff00; color: #0a0a12; }"
    );
    
    QPushButton *btnFwd = new QPushButton("FORWARD");
    btnFwd->setStyleSheet(
        "QPushButton { background-color: #1a1a2e; border: 2px solid #ffff00; border-radius: 6px; padding: 10px; font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #ffff00; color: #0a0a12; }"
    );
    
    QPushButton *btnBack = new QPushButton("BACK");
    btnBack->setStyleSheet(
        "QPushButton { background-color: #1a1a2e; border: 2px solid #ff0000; border-radius: 6px; padding: 10px; font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #ff0000; color: #0a0a12; }"
    );

    connect(btnStart, &QPushButton::clicked, this, &GameWindow::toggleRun);
    connect(btnFwd, &QPushButton::clicked, this, &GameWindow::stepForward);
    connect(btnBack, &QPushButton::clicked, this, &GameWindow::stepBack);

    btnLayout->addWidget(btnStart);
    btnLayout->addWidget(btnFwd);
    btnLayout->addWidget(btnBack);
    mainLayout->addLayout(btnLayout);

    updateSpeed(150);
}

GameWindow::~GameWindow() {
    for (int i = 0; i < H; ++i) { delete[] current[i]; delete[] next[i]; }
    delete[] current; delete[] next;
    delete automaton;
}

void GameWindow::initGrid() {
    current = new bool*[H];
    next = new bool*[H];
    for (int i = 0; i < H; ++i) {
        current[i] = new bool[W]{false};
        next[i] = new bool[W]{false};
    }
}

void GameWindow::onCellClicked(int x, int y) {
    if (current[y][x]) {
        current[y][x] = false;
        automaton->onCellRemoved(x, y);
    } else {
        current[y][x] = true;
        automaton->onCellAdded(x, y);
    }
    gridWidget->update();
}

void GameWindow::clearAll() {
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            current[y][x] = false;
    automaton->reset();
    gridWidget->update();
}

void GameWindow::randomFill() {
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            // 20% chance to be alive
            bool alive = (QRandomGenerator::global()->bounded(100) < 20);
            current[y][x] = alive;
            if (alive) automaton->onCellAdded(x, y);
        }
    }
    gridWidget->update();
}

void GameWindow::toggleRun() {
    if (timer->isActive()) {
        timer->stop();
        btnStart->setText("START");
        btnStart->setStyleSheet(
            "QPushButton { background-color: #1a1a2e; border: 2px solid #00ff00; border-radius: 6px; padding: 10px; font-size: 14px; font-weight: bold; }"
            "QPushButton:hover { background-color: #00ff00; color: #0a0a12; }"
        );
    } else {
        timer->start();
        btnStart->setText("STOP");
        btnStart->setStyleSheet(
            "QPushButton { background-color: #1a1a2e; border: 2px solid #ff0000; border-radius: 6px; padding: 10px; font-size: 14px; font-weight: bold; }"
            "QPushButton:hover { background-color: #ff0000; color: #0a0a12; }"
        );
    }
}

void GameWindow::updateSpeed(int value) {
    timer->setInterval(value);
}

void GameWindow::saveSnapshot() {
    if (histCount == 50) {
        for (int i = 0; i < H; ++i) delete[] history[0].data[i];
        delete[] history[0].data;
        for (int i = 1; i < 50; ++i) history[i-1] = history[i];
        histCount--;
    }
    bool** snap = new bool*[H];
    for (int i = 0; i < H; ++i) {
        snap[i] = new bool[W];
        for (int j = 0; j < W; ++j) snap[i][j] = current[i][j];
    }
    history[histCount++].data = snap;
}

void GameWindow::stepForward() {
    saveSnapshot();
    automaton->step(current, next, W, H);
    bool** temp = current; current = next; next = temp;
    gridWidget->setCells(current);
}

void GameWindow::stepBack() {
    if (histCount > 0) {
        histCount--;
        bool** snap = history[histCount].data;
        for (int i = 0; i < H; ++i)
            for (int j = 0; j < W; ++j)
                current[i][j] = snap[i][j];
        for (int i = 0; i < H; ++i) delete[] snap[i];
        delete[] snap;
        gridWidget->setCells(current);
    }
}

void GameWindow::openMaps() {
    MapsWindow dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        applyPattern(dlg.selectedPattern());
    }
}

void GameWindow::returnToMenu() {
    for (QWidget *widget : QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow*>(widget);
        if (mainWin) {
            mainWin->show();
            break;
        }
    }
    this->close();
}

void GameWindow::applyPattern(const QVector<QPair<int,int>>& cells) {
    automaton->reset();

    // 1. Очищаем поле
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            current[y][x] = false;

    if (cells.isEmpty()) return;

    // 2. Ищем габариты карты (bounding box)
    int minX = cells[0].first, maxX = cells[0].first;
    int minY = cells[0].second, maxY = cells[0].second;
    for (auto& p : cells) {
        if (p.first < minX) minX = p.first;
        if (p.first > maxX) maxX = p.first;
        if (p.second < minY) minY = p.second;
        if (p.second > maxY) maxY = p.second;
    }

    // 3. Размеры карты
    int mapW = maxX - minX + 1;
    int mapH = maxY - minY + 1;

    // 4. Точный сдвиг, чтобы центр карты совпал с центром поля
    int offsetX = (W - mapW) / 2 - minX;
    int offsetY = (H - mapH) / 2 - minY;

    // 5. Ставим клетки
    for (auto& p : cells) {
        int nx = p.first + offsetX;
        int ny = p.second + offsetY;
        if (nx >= 0 && nx < W && ny >= 0 && ny < H) {
            current[ny][nx] = true;
            automaton->onCellAdded(nx, ny);
        }
    }
    gridWidget->setCells(current);
}