#ifndef DEBITCALC_H
#define DEBITCALC_H

#include <QWidget>
#include <QLayout>
#include <QHash>
#include <QPushButton>
#include <QList>

namespace Ui {
class DebitCalc;
}

class DebitCalc : public QWidget
{
    Q_OBJECT

public:
    explicit DebitCalc(QWidget *parent = nullptr);
    ~DebitCalc();

private slots:
    void onAddWidget();
    void onRemoveWidget();

private:
    Ui::DebitCalc *ui;
    QHash<QPushButton*, QHBoxLayout*> mButtonToLayout;
    QStringList periodItems = {"Разовое", "Раз в месяц", "Раз в квартал", "Раз в полгода", "Раз в год"};
};

#endif // DEBITCALC_H
