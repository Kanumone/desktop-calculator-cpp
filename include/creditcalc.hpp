#ifndef SRC_INCLUDE_CREDITCALC_HPP_
#define SRC_INCLUDE_CREDITCALC_HPP_

#include <QWidget>

namespace Ui {
class CreditCalc;
}
/*!
 * @brief Класс, реализующий логику кредитного калькулятора
 *
 */
class CreditCalc : public QWidget {
  Q_OBJECT

 public:
  explicit CreditCalc(QWidget *parent = nullptr);
  ~CreditCalc();

 private slots:
  void on_pushButton_calculate_clicked();

 private:
  Ui::CreditCalc *ui;
};

#endif  // SRC_INCLUDE_CREDITCALC_HPP_
