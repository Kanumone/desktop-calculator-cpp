#include "debitcalc.h"

#include "ui_debitcalc.h"

DebitCalc::DebitCalc(QWidget *parent) : QWidget(parent), ui(new Ui::DebitCalc) {
  ui->setupUi(this);
  connect(ui->pushButton_add, &QPushButton::clicked, this, &DebitCalc::onAddWidget);
  connect(ui->pushButton_remove, &QPushButton::clicked, this, &DebitCalc::onAddWidget);
}

DebitCalc::~DebitCalc() { delete ui; }

void DebitCalc::onAddWidget() {
  QVBoxLayout *layout;
  QString usingFrame = qobject_cast<QPushButton *>(sender())->text();
  if (usingFrame.compare("Добавить пополнение") == 0) {
    layout = qobject_cast<QVBoxLayout *>(ui->frame_add->layout());
  } else {
    layout = qobject_cast<QVBoxLayout *>(ui->frame_remove->layout());
  }

  QHBoxLayout *newLayout = new QHBoxLayout();
  QPushButton *deleteButton = new QPushButton("X");
  QDateEdit *depositDate = new QDateEdit();
  QComboBox *period = new QComboBox();
  QLineEdit *sumLine = new QLineEdit();
  deleteButton->setFixedSize(18, 18);
  deleteButton->setStyleSheet("border: 1px solid red; border-radius: 2px; color: red;");
  connect(deleteButton, &QPushButton::clicked, this, &DebitCalc::onRemoveWidget);
  period->addItems(periodItems);
  sumLine->setPlaceholderText("Сумма");
  mButtonToLayout.insert(deleteButton, newLayout);

  newLayout->addWidget(period);
  newLayout->addWidget(depositDate);
  newLayout->addWidget(sumLine);
  newLayout->addWidget(deleteButton);
  layout->insertLayout(0, newLayout);
}

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
