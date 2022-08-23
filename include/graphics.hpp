#ifndef SRC_INCLUDE_GRAPHICS_HPP_
#define SRC_INCLUDE_GRAPHICS_HPP_

extern "C" {
#include "s21_calculator.h"
#include "s21_parser.h"
}

#include <QVector>
#include <QWidget>

namespace Ui {
class Graphics;
}
/*!
 * @brief Класс, реализующий логику построения графиков
 *
 */
class Graphics : public QWidget {
  Q_OBJECT

 public:
  explicit Graphics(QWidget *parent = nullptr);
  ~Graphics();

 private:
  Ui::Graphics *ui;
  double xBegin, xEnd, yBegin, yEnd, step;
  int n;
  QVector<double> x, y;

 public slots:
  void get_label(QString expression);

 private slots:
  void build_graphic();
};

#endif  // SRC_INCLUDE_GRAPHICS_HPP_
