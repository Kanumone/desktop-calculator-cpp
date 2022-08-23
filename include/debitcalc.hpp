#ifndef SRC_INCLUDE_DEBITCALC_HPP_
#define SRC_INCLUDE_DEBITCALC_HPP_

#include <QWidget>
#include <QtWidgets>

namespace Ui {
class DebitCalc;
}

/*!
 * @brief Класс, реализующий логику депозитного калькулятора
 *
 */
class DebitCalc : public QWidget {
  Q_OBJECT

 public:
  explicit DebitCalc(QWidget *parent = nullptr);
  ~DebitCalc();

 private slots:
  void onAddWidget();
  void onRemoveWidget();
  void onCalculateClicked();

 private:
  Ui::DebitCalc *ui;
  QMessageBox *outWindow;
  QHash<QPushButton *, QHBoxLayout *> mButtonToLayout;
  QList<QString> periodItems = {"Разовое", "Раз в месяц", "Раз в квартал", "Раз в полгода",
                                "Раз в год"};
  QHash<QString, int> paymentPeriod{{"В конце срока", -1},
                                    {"Разовое", 0},
                                    {"Раз в месяц", 1},
                                    {"Раз в квартал", 3},
                                    {"Раз в полгода", 6},
                                    {"Раз в год", 12}};
  struct Changer {
    QDate date;
    int type;
    double value;
    Changer(QDate _d, int _t, double _v) : date(_d), type(_t), value(_v) {}
    bool operator<(const Changer &other) const { return this->date < other.date; }
  };
};

#endif  // SRC_INCLUDE_DEBITCALC_HPP_
