#pragma once
#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void startGame();
    void onTypeChanged(int index);

private:
    QSpinBox *spinW;
    QSpinBox *spinH;
    QComboBox *comboType;

    QWidget *customWidget;
    QLineEdit *birthEdit;
    QLineEdit *surviveEdit;

    unsigned int customBirth;
    unsigned int customSurvive;
};