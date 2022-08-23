#ifndef SRC_INCLUDE_MAINWINDOW_HPP_
#define SRC_INCLUDE_MAINWINDOW_HPP_

#include <QAbstractButton>
#include <QInputDialog>
#include <QMainWindow>
#include <QRegularExpression>

#include "creditcalc.hpp"
#include "debitcalc.hpp"
#include "graphics.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  Graphics graphics_window;
  CreditCalc credit_window;
  DebitCalc debit_window;
  int bracket_counter = 0;
  const QList<QRegularExpression> regExps{
      QRegularExpression(".(?<![.+\\-*\\/^(d]|\\d)(\\d)"),
      QRegularExpression(".(?<![+\\-*\\/^(da])((a?(sin|cos|tan)|log|ln|sqrt))"),
      QRegularExpression(".(?<![()Ex]|\\d)([+\\-*\\/^]|mod)"),
      QRegularExpression("nan|inf")};
  bool validateInput(QString exp);

 private slots:
  void click_common(QAbstractButton *button);
  void click_function(QAbstractButton *button);
  void click_dot();
  void clear_display();
  void click_equal();
  void click_backspace();
  void click_graphics();

  void on_actionCredit_calculator_triggered();

  void on_actionDebit_calculator_triggered();

 signals:
  void signal(QString expression);
};
#endif  // SRC_INCLUDE_MAINWINDOW_HPP_
