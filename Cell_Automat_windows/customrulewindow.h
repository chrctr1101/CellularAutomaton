#pragma once
#include <QDialog>
#include <QLineEdit>

class CustomRuleWindow : public QDialog {
    Q_OBJECT
public:
    explicit CustomRuleWindow(QWidget *parent = nullptr);
    unsigned int birthMask() const;
    unsigned int surviveMask() const;

private:
    QLineEdit *birthEdit;
    QLineEdit *surviveEdit;
};