#ifndef CREDITCALC_H
#define CREDITCALC_H

#include <QWidget>

namespace Ui {
class CreditCalc;
}

class CreditCalc : public QWidget
{
    Q_OBJECT

public:
    explicit CreditCalc(QWidget *parent = nullptr);
    ~CreditCalc();

private slots:
    void on_pushButton_calculate_clicked();

private:
    Ui::CreditCalc *ui;

};

#endif // CREDITCALC_H
