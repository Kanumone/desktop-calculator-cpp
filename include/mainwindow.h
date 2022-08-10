#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractButton>
#include <QRegularExpression>
#include <QInputDialog>
#include "graphics.h"
#include "creditcalc.h"
#include "debitcalc.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Graphics *graphics_window;
    CreditCalc *credit_window;
    DebitCalc *debit_window;
    int bracket_counter = 0;

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
#endif // MAINWINDOW_H
