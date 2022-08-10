#include "mainwindow.h"

#include <QPixmap>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  graphics_window = new Graphics;
  credit_window = new CreditCalc;
  debit_window = new DebitCalc;
  ui->setupUi(this);
  connect(ui->main_buttons_group, SIGNAL(buttonClicked(QAbstractButton*)), this,
          SLOT(click_common(QAbstractButton *)));
  connect(ui->function_buttons_group, SIGNAL(buttonClicked(QAbstractButton*)), this,
          SLOT(click_function(QAbstractButton *)));
  connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(click_dot()));
  connect(ui->pushButton_AC, SIGNAL(clicked()), this, SLOT(clear_display()));
  connect(ui->pushButton_eq, SIGNAL(clicked()), this, SLOT(click_equal()));
  connect(ui->pushButton_backspace, SIGNAL(clicked()), this, SLOT(click_backspace()));

  connect(ui->pushButton_graphics, SIGNAL(clicked()), this, SLOT(click_graphics()));
  connect(this, &MainWindow::signal, graphics_window, &Graphics::get_label);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::click_common(QAbstractButton *button) {
  static QRegularExpression re_num("[\\d|x]$");
  static QRegularExpression re_double_operator("[*/+-]$");
  QString button_text = button->text();
  QString display_text = ui->display->text();
  bool bracket_error = (button_text == '(' || button_text == 'x') && display_text.contains(re_num);
  bool op_error = button_text.contains(re_double_operator) &&
                  (display_text.contains(re_double_operator) || display_text.size() == 0);
  if (!(button_text[0] == ')' && bracket_counter == 0) && !bracket_error && !op_error &&
      display_text.size() < 255) {
    if (button_text[0] == '(') bracket_counter++;
    display_text.append(button_text);
    ui->display->setText(display_text);
    if (button_text[0] == ')') bracket_counter--;
  }
}

void MainWindow::click_function(QAbstractButton *button) {
  static QRegularExpression re_num("[\\d|x]$");
  QString button_text = button->text() + "(";
  QString display_text = ui->display->text();
  bool op_error = display_text.contains(re_num);
  if (display_text.size() < 255 && !op_error) {
    bracket_counter++;
    display_text.append(button_text);
    ui->display->setText(display_text);
  }
}

void MainWindow::click_dot() {
  QString display_text = ui->display->text();
  static QRegularExpression re_number("\\d+\\.(\\d+)?$");
  if (!display_text.contains(re_number)) {
    ui->display->setText(ui->display->text() + ".");
  }
}

void MainWindow::clear_display() {
  bracket_counter = 0;
  ui->display->clear();
}

void MainWindow::click_equal() {
  if (bracket_counter == 0) {
    std::string buff = ui->display->text().toStdString();
    char *str_exp = const_cast<char *>(buff.c_str());
    s21_deque_t *rpn = s21_parse_exp(str_exp);

    double result = 0.0;
    if (ui->display->text().contains('x')) {
        QInputDialog inputX = QInputDialog();
        bool ok;
        double x = inputX.getDouble(this, tr("Input X"), tr("Your value"), 0, -100000000, 100000000, 1, &ok);
        if (ok) {
            result = s21_calculate_rpn(rpn, x);
        }
    } else {
        result = s21_calculate_rpn(rpn, 0.0);
    }

    QString output_res = QString::number(result);
    ui->display->setText(output_res);
  } else {
    ui->statusBar->showMessage("Incorrect input", 2000);
  }
}

void MainWindow::click_backspace() {
  QString display_text = ui->display->text();
  int last_symbol = display_text.size() - 1;
  int delete_counter = 0;
  if (last_symbol >= 0) {
    if (display_text[last_symbol] == '(') {
      bracket_counter--;
    } else if (display_text[last_symbol] == '(') {
      bracket_counter++;
    }

    while (display_text[last_symbol] >= 'a' && display_text[last_symbol] <= 'z' &&
           last_symbol > 0) {
      delete_counter++;
      last_symbol--;
    }

    if (delete_counter == 0 || last_symbol == 0) {
      delete_counter++;
    }

    display_text.chop(delete_counter);
    ui->display->setText(display_text);
  }
}

void MainWindow::click_graphics() {
  if (bracket_counter == 0) {
    graphics_window->show();
    emit signal(ui->display->text());
  }
}

void MainWindow::on_actionCredit_calculator_triggered() { credit_window->show(); }

void MainWindow::on_actionDebit_calculator_triggered() { debit_window->show(); }
