#include "customrulewindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

CustomRuleWindow::CustomRuleWindow(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Пользовательские правила");
    resize(300, 150);

    auto layout = new QVBoxLayout(this);

    auto l1 = new QHBoxLayout();
    l1->addWidget(new QLabel("Рождение (B):"));
    birthEdit = new QLineEdit("3");
    birthEdit->setPlaceholderText("например, 3");
    l1->addWidget(birthEdit);
    layout->addLayout(l1);

    auto l2 = new QHBoxLayout();
    l2->addWidget(new QLabel("Выживание (S):"));
    surviveEdit = new QLineEdit("23");
    surviveEdit->setPlaceholderText("например, 23");
    l2->addWidget(surviveEdit);
    layout->addLayout(l2);

    auto btn = new QPushButton("OK");
    connect(btn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(btn);
}

unsigned int CustomRuleWindow::birthMask() const {
    unsigned int m = 0;
    for (QChar c : birthEdit->text()) if (c.isDigit()) m |= (1u << c.digitValue());
    return m;
}

unsigned int CustomRuleWindow::surviveMask() const {
    unsigned int m = 0;
    for (QChar c : surviveEdit->text()) if (c.isDigit()) m |= (1u << c.digitValue());
    return m;
}