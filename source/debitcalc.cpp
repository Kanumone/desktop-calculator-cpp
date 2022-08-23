#include "debitcalc.hpp"

#include "ui_debitcalc.h"

DebitCalc::DebitCalc(QWidget *parent) : QWidget(parent), ui(new Ui::DebitCalc) {
  ui->setupUi(this);
  outWindow = new QMessageBox(this);
  connect(ui->pushButton_add, &QPushButton::clicked, this, &DebitCalc::onAddWidget);
  connect(ui->pushButton_remove, &QPushButton::clicked, this, &DebitCalc::onAddWidget);
  connect(ui->pushButton_calculate, &QPushButton::clicked, this, &DebitCalc::onCalculateClicked);
}

DebitCalc::~DebitCalc() { delete ui; }

//*! Метод добавления виджета пополнения или снятия средств *  //
void DebitCalc::onAddWidget() {
  QVBoxLayout *layout;

  QString usingFrame = qobject_cast<QPushButton *>(sender())->text();
  if (usingFrame.compare("Добавить пополнение") == 0) {
    layout = qobject_cast<QVBoxLayout *>(ui->frame_add->layout());
  } else {
    layout = qobject_cast<QVBoxLayout *>(ui->frame_remove->layout());
  }

  QHBoxLayout *newLayout = new QHBoxLayout(qobject_cast<QWidget *>(layout));
  QPushButton *deleteButton = new QPushButton("X");
  QDateEdit *depositDate = new QDateEdit(ui->dateEdit_start->date());
  QComboBox *period = new QComboBox();
  QDoubleSpinBox *depositSumInput = new QDoubleSpinBox();
  depositDate->setCalendarPopup(true);
  depositDate->setDisplayFormat("dd/MM/yyyy");
  deleteButton->setFixedSize(18, 18);
  deleteButton->setStyleSheet("border: 1px solid red; border-radius: 2px; color: red;");
  connect(deleteButton, &QPushButton::clicked, this, &DebitCalc::onRemoveWidget);
  period->addItems(periodItems);
  depositSumInput->setRange(0.0, 100000000000000000.0);
  depositSumInput->setDecimals(2);
  depositSumInput->setMinimumWidth(130);
  mButtonToLayout.insert(deleteButton, newLayout);

  newLayout->addWidget(period);
  newLayout->addWidget(depositDate);
  newLayout->addWidget(depositSumInput);
  newLayout->addWidget(deleteButton);
  layout->insertLayout(0, newLayout);
}

//*! @brief Метод удаления виджета пополнения или снятия средств */
void DebitCalc::onRemoveWidget() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  delete button;
  QHBoxLayout *layout = mButtonToLayout.take(button);

  while (layout->count() != 0) {
    QLayoutItem *item = layout->takeAt(0);
    delete item->widget();
    delete item;
  }
  delete layout;
}

/*!
Метод, выполняющий расчет депозита
---------------------------------
Возможности:
- Добавление снятий и пополнений
- Расчет депозита с капитализией процентов
 */
void DebitCalc::onCalculateClicked() {
  auto deposits = ui->frame_add->layout()->children();
  auto removes = ui->frame_remove->layout()->children();
  QList<DebitCalc::Changer> changeDeposit;
  for (auto deposit : deposits) {
    QHBoxLayout *l = qobject_cast<QHBoxLayout *>(deposit);
    QString frequency = qobject_cast<QComboBox *>(l->itemAt(0)->widget())->currentText();
    QDate incomeDate = qobject_cast<QDateEdit *>(l->itemAt(1)->widget())->date();
    double sum = qobject_cast<QDoubleSpinBox *>(l->itemAt(2)->widget())->value();
    Changer c(incomeDate, paymentPeriod[frequency], sum);
    changeDeposit.push_back(c);
  }

  for (auto remove : removes) {
    QHBoxLayout *l = qobject_cast<QHBoxLayout *>(remove);
    QString frequency = qobject_cast<QComboBox *>(l->itemAt(0)->widget())->currentText();
    QDate incomeDate = qobject_cast<QDateEdit *>(l->itemAt(1)->widget())->date();
    double sum = qobject_cast<QDoubleSpinBox *>(l->itemAt(2)->widget())->value();
    Changer c(incomeDate, paymentPeriod[frequency], -sum);
    changeDeposit.push_back(c);
  }

  double percentRate = ui->doubleSpinBox_percentRate->value() / 100;
  int duration = ui->spinBox_duration->value();
  QString regularity = ui->comboBox_regularity->currentText();
  int paymentRegularity = paymentPeriod.value(regularity);
  QDate currentPoint = ui->dateEdit_start->date();
  QDate endTerm = currentPoint.addMonths(duration);
  QDate nextPayment = paymentRegularity >= 0 ? currentPoint.addMonths(paymentRegularity) : endTerm;
  double depositSum = ui->doubleSpinBox_sum->value();
  double income = 0.0;

  std::sort(changeDeposit.begin(), changeDeposit.end());

  while (currentPoint < endTerm) {
    double periodIncome = 0.0;
    if (!changeDeposit.isEmpty()) {
      QDate nextChange = changeDeposit[0].date;
      while (nextChange < nextPayment) {
        int t = currentPoint.daysTo(nextChange);
        currentPoint = nextChange;
        double S = (depositSum * percentRate * t / 365);
        income += S;
        periodIncome += S;
        depositSum += changeDeposit[0].value;
        if (changeDeposit[0].type == 0) {
          changeDeposit.pop_front();
        } else {
          int next = changeDeposit[0].type;
          nextChange = nextChange.addMonths(next);
          changeDeposit[0].date = nextChange;
          std::sort(changeDeposit.begin(), changeDeposit.end());
        }
        nextChange = changeDeposit[0].date;
      }
    }

    int t = currentPoint.daysTo(nextPayment);
    double S = (depositSum * percentRate * t / 365);
    income += S;
    periodIncome += S;
    if (ui->checkBox_capitaliztion->isChecked()) {
      depositSum += periodIncome;
    }
    currentPoint = nextPayment;
    nextPayment = nextPayment.addMonths(paymentRegularity);
  }

  double mainRate = ui->doubleSpinBox_mainRate->value();
  double nonTaxIncome = 1e6 * mainRate / 100;
  double taxdepositSum = 0.0;

  if (income > nonTaxIncome) {
    taxdepositSum = (income - nonTaxIncome) * 0.13;
  }

  outWindow->setText("Начисленный проценты = " + QString::number(income) +
                     "\nНалог = " + QString::number(taxdepositSum) +
                     "\nИтоговая сумма = " + QString::number(depositSum));
  outWindow->show();
}
