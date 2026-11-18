#include "mainwindow.h"
#include "gamewindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolTip>
#include <QApplication> // Для qApp->quit()

MainWindow::MainWindow(QWidget *parent) 
    : QWidget(parent), customBirth(1u << 3), customSurvive((1u << 2) | (1u << 3)) {
    setWindowTitle("NEON AUTOMATA");
    resize(450, 500); // Чуть увеличили высоту под новую кнопку
    setStyleSheet(
        "QWidget { background-color: #0a0a12; color: #00ffcc; font-family: 'Consolas', 'Courier New', monospace; font-size: 14px; }"
    );

    auto layout = new QVBoxLayout(this);
    layout->setSpacing(12);
    layout->setContentsMargins(25, 20, 25, 25);

    // ==================== TITLE ====================
    QLabel* title = new QLabel("CELLULAR AUTOMATON");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel {"
        "  font-size: 26px;"
        "  font-weight: bold;"
        "  color: #00ffcc;"
        "  border: none;"
        "  padding: 10px;"
        "  letter-spacing: 2px;"
        "}"
    );
    layout->addWidget(title);

    // Разделительная линия
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #ff00ff; background-color: #ff00ff; max-height: 2px;");
    layout->addWidget(line);

    // ==================== SETTINGS ====================
    // Width
    auto l1 = new QHBoxLayout();
    QLabel* lblW = new QLabel("Width (X):");
    lblW->setStyleSheet("color: #ff00ff; font-weight: bold;");
    l1->addWidget(lblW);
    spinW = new QSpinBox();
    spinW->setRange(10, 150);
    spinW->setValue(40);
    spinW->setStyleSheet(
        "QSpinBox { background-color: #12121a; border: 2px solid #00ffcc; border-radius: 6px; padding: 6px; color: #00ffcc; }"
        "QSpinBox::up-button, QSpinBox::down-button { background: #1a1a2e; }"
    );
    l1->addWidget(spinW);
    layout->addLayout(l1);

    // Height
    auto l2 = new QHBoxLayout();
    QLabel* lblH = new QLabel("Height (Y):");
    lblH->setStyleSheet("color: #ff00ff; font-weight: bold;");
    l2->addWidget(lblH);
    spinH = new QSpinBox();
    spinH->setRange(10, 150);
    spinH->setValue(40);
    spinH->setStyleSheet(
        "QSpinBox { background-color: #12121a; border: 2px solid #00ffcc; border-radius: 6px; padding: 6px; color: #00ffcc; }"
        "QSpinBox::up-button, QSpinBox::down-button { background: #1a1a2e; }"
    );
    l2->addWidget(spinH);
    layout->addLayout(l2);

    // Type
    auto l3 = new QHBoxLayout();
    QLabel* lblT = new QLabel("Automaton Type:");
    lblT->setStyleSheet("color: #ff00ff; font-weight: bold;");
    l3->addWidget(lblT);
    comboType = new QComboBox();
    comboType->addItems({"Classic Life", "Drops", "Custom"});
    comboType->setStyleSheet(
        "QComboBox { background-color: #12121a; border: 2px solid #00ffcc; border-radius: 6px; padding: 6px; color: #00ffcc; }"
        "QComboBox QAbstractItemView { background-color: #0a0a12; color: #00ffcc; selection-background-color: #ff00ff; }"
    );
    l3->addWidget(comboType);
    layout->addLayout(l3);

    // Custom fields container
    customWidget = new QWidget(this);
    customWidget->setFixedHeight(90);
    customWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    auto customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(0, 10, 0, 0);
    customLayout->setSpacing(8);

    // Birth
    auto customL1 = new QHBoxLayout();
    QLabel* lblB = new QLabel("Birth (B):");
    lblB->setStyleSheet("color: #ff00ff; font-weight: bold;");
    customL1->addWidget(lblB);
    
    birthEdit = new QLineEdit("3");
    birthEdit->setStyleSheet(
        "QLineEdit { background-color: #12121a; border: 2px solid #00ffcc; border-radius: 6px; padding: 6px; color: #00ffcc; }"
    );
    birthEdit->setToolTip(
        "Birth (B): Number of live neighbors required for a dead cell to become alive.\n"
        "Example: '3' means a dead cell with 3 neighbors becomes alive.\n"
        "Multiple digits allowed: '36' (3 or 6 neighbors)."
    );
    customL1->addWidget(birthEdit);
    customLayout->addLayout(customL1);

    // Survival
    auto customL2 = new QHBoxLayout();
    QLabel* lblS = new QLabel("Survival (S):");
    lblS->setStyleSheet("color: #ff00ff; font-weight: bold;");
    customL2->addWidget(lblS);
    
    surviveEdit = new QLineEdit("23");
    surviveEdit->setStyleSheet(
        "QLineEdit { background-color: #12121a; border: 2px solid #00ffcc; border-radius: 6px; padding: 6px; color: #00ffcc; }"
    );
    surviveEdit->setToolTip(
        "Survival (S): Number of live neighbors required for a live cell to survive.\n"
        "Example: '23' means a live cell with 2 or 3 neighbors survives.\n"
        "Classic Life: B3/S23."
    );
    customL2->addWidget(surviveEdit);
    customLayout->addLayout(customL2);

    // Скрываем содержимое
    for (int i = 0; i < customLayout->count(); ++i) {
        QLayoutItem *item = customLayout->itemAt(i);
        if (item->layout()) {
            for (int j = 0; j < item->layout()->count(); ++j) {
                if (QWidget *w = item->layout()->itemAt(j)->widget())
                    w->setVisible(false);
            }
        } else if (QWidget *w = item->widget()) {
            w->setVisible(false);
        }
    }
    layout->addWidget(customWidget);

    // ==================== BUTTONS ====================
    // Start button
    auto btnStart = new QPushButton("START SIMULATION");
    btnStart->setStyleSheet(
        "QPushButton { background-color: #1a1a2e; border: 2px solid #00ff00; border-radius: 8px; padding: 14px; font-size: 16px; font-weight: bold; color: #00ffcc; }"
        "QPushButton:hover { background-color: #00ff00; color: #0a0a12; }"
    );
    connect(btnStart, &QPushButton::clicked, this, &MainWindow::startGame);
    layout->addWidget(btnStart);

    // Exit button
    auto btnExit = new QPushButton("EXIT");
    btnExit->setStyleSheet(
        "QPushButton { background-color: #1a1a2e; border: 2px solid #ff3333; border-radius: 8px; padding: 10px; font-size: 16px; font-weight: bold; color: #00ffcc; }"
        "QPushButton:hover { background-color: #ff3333; color: #0a0a12; }"
    );
    connect(btnExit, &QPushButton::clicked, this, &MainWindow::exitApp);
    layout->addWidget(btnExit);

    connect(comboType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onTypeChanged);
}

void MainWindow::onTypeChanged(int index) {
    bool show = (index == 2);
    auto customLayout = customWidget->layout();
    for (int i = 0; i < customLayout->count(); ++i) {
        QLayoutItem *item = customLayout->itemAt(i);
        if (item->layout()) {
            for (int j = 0; j < item->layout()->count(); ++j) {
                if (QWidget *w = item->layout()->itemAt(j)->widget())
                    w->setVisible(show);
            }
        } else if (QWidget *w = item->widget()) {
            w->setVisible(show);
        }
    }
}

void MainWindow::startGame() {
    int w = spinW->value();
    int h = spinH->value();
    int type = comboType->currentIndex();

    if (type == 2) {
        unsigned int b = 0, s = 0;
        for (QChar c : birthEdit->text()) if (c.isDigit()) b |= (1u << c.digitValue());
        for (QChar c : surviveEdit->text()) if (c.isDigit()) s |= (1u << c.digitValue());
        customBirth = b;
        customSurvive = s;
    }

    GameWindow *game = new GameWindow(w, h, type, customBirth, customSurvive);
    game->setAttribute(Qt::WA_DeleteOnClose);
    game->showFullScreen();
    this->hide();
}

void MainWindow::exitApp() {
    qApp->quit(); // Завершает приложение
}