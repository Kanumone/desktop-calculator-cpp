#include "creditcalc.hpp"

#include "ui_creditcalc.h"

CreditCalc::CreditCalc(QWidget *parent) : QWidget(parent), ui(new Ui::CreditCalc) {
  ui->setupUi(this);
  QRegularExpressionValidator *pVal = new QRegularExpressionValidator(QRegularExpression("^\\d{1,100}"));
  ui->lineEdit_money->setValidator(pVal);
}

CreditCalc::~CreditCalc() { delete ui; }

/*!
Метод, выполняющий расчет кредита
---------------------------------
Возможен расчет кредитов двух типов:
- Аннуитетный
- Дифференцированный
 */
void CreditCalc::on_pushButton_calculate_clicked() {
  double amount = 0.0;
  int period = ui->spinBox_period->text().toInt();
  int creditType = ui->comboBox_type->currentIndex();
  double sum = ui->lineEdit_money->text().toDouble();
  double monthPercent = ui->doubleSpinBox_percent->text().toDouble() / (12.0 * 100);
  if (creditType == 0) {
    double monthPayment = sum * monthPercent / (1 - pow(1 + monthPercent, -period));
    amount = period * monthPayment;
    ui->label_mounthPayment->setText(QString::number(monthPayment));
  } else {
    QString result = QString::number(sum / period + sum * monthPercent);
    double currentSum = sum;
    double constPart = sum / period;
    double monthPayment = 0.0;
    while (period > 0) {
      monthPayment = constPart + currentSum * monthPercent;
      currentSum -= constPart;
      amount += monthPayment;
      period--;
    }
    result.append("...");
    result.append(QString::number(monthPayment));
    ui->label_mounthPayment->setText(result);
  }

  ui->label_overpayment->setText(QString::number(amount - sum));

  ui->label_sum->setText(QString::number(amount));
}
