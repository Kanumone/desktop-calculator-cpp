#ifndef GRAPHICS_H
#define GRAPHICS_H

extern "C" {
    #include "s21_parser.h"
    #include "s21_calculator.h"
}

#include <QWidget>
#include <QVector>

namespace Ui {
class Graphics;
}

class Graphics : public QWidget
{
    Q_OBJECT

public:
    explicit Graphics(QWidget *parent = nullptr);
    ~Graphics();

private:
    Ui::Graphics *ui;
    double xBegin, xEnd, yBegin, yEnd, step;
    int n;
    QVector<double> x,y;

public slots:
    void get_label(QString expression);

private slots:
    void build_graphic();

};

#endif // GRAPHICS_H
