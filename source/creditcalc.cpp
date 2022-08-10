#include "creditcalc.h"

#include <QDebug>

#include "ui_creditcalc.h"

CreditCalc::CreditCalc(QWidget *parent) : QWidget(parent), ui(new Ui::CreditCalc) {
  ui->setupUi(this);
}

CreditCalc::~CreditCalc() { delete ui; }

void CreditCalc::on_pushButton_calculate_clicked() {
  double amount = 0.0, monthPercent, monthPayment, sum;
  int period = ui->spinBox_period->text().toInt();
  int creditType = ui->comboBox_type->currentIndex();
  sum = ui->lineEdit_money->text().toDouble();
  monthPercent = ui->lineEdit_percent->text().toDouble() / (12.0 * 100);
  if (creditType == 0) {
    monthPayment = sum * monthPercent / (1 - pow(1 + monthPercent, -36));
    amount = period * monthPayment;
    ui->label_mounthPayment->setText(QString::number(monthPayment));
  } else {
    double first = sum / period + sum * monthPercent;
    double currentSum = sum;
    double constPart = sum / period;
    while (period > 0) {
      monthPayment = constPart + currentSum * monthPercent;
      currentSum -= constPart;
      amount += monthPayment;
      period--;
    }

    QString result = QString::number(first) + "-" + QString::number(monthPayment);
    ui->label_mounthPayment->setText(result);
  }

  ui->label_overpayment->setText(QString::number(amount - sum));

  ui->label_sum->setText(QString::number(amount));
}
