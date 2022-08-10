#include "graphics.h"

#include "ui_graphics.h"

Graphics::Graphics(QWidget *parent) : QWidget(parent), ui(new Ui::Graphics) {
  ui->setupUi(this);
  connect(ui->pushButton_build, SIGNAL(clicked()), this, SLOT(build_graphic()));
}

Graphics::~Graphics() { delete ui; }

void Graphics::get_label(QString expression) { ui->expression_label->setText(expression); }

void Graphics::build_graphic() {
  xBegin = ui->doubleSpinBox_minX->text().toDouble();
  xEnd = ui->doubleSpinBox_maxX->text().toDouble();
  yBegin = ui->doubleSpinBox_minY->text().toDouble();
  yEnd = ui->doubleSpinBox_maxY->text().toDouble();
  step = ui->doubleSpinBox_step->text().toDouble();
  n = (int)((xBegin - xEnd) / step);

  if (step >= 1e-10 && n <= 1000000) {
    ui->widget->xAxis->setRange((int)xBegin, (int)xEnd);
    ui->widget->yAxis->setRange((int)yBegin, (int)yEnd);
    std::string buff = ui->expression_label->text().toStdString();
    char *str_exp = const_cast<char *>(buff.c_str());

    s21_deque_t *rpn = s21_parse_exp(str_exp);
    double X = xBegin;
    while (X <= xEnd) {
      x.push_back(X);
      y.push_back(s21_calculate_rpn(rpn, X));
      X += step;
    }

    s21_delete_deque(rpn);
    ui->widget->addGraph();
    ui->widget->graph(0)->addData(x, y);
    ui->widget->replot();
  } else {
  }
}
