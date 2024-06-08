#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QShortcut>
#include <QDoubleValidator>
#include <dynamicline.h>

QT_BEGIN_NAMESPACE
namespace Ui { class calculator; }
QT_END_NAMESPACE

class calculator : public QMainWindow
{
    Q_OBJECT

public:
    calculator(QWidget *parent = nullptr);
    ~calculator();
    bool eventFilter(QObject *watched, QEvent *event);
    QDynamicLine* focused;

private:
    Ui::calculator *ui;
    QShortcut *key_sum;
    QShortcut *key_add;
    QShortcut *key_cnt;

private slots:
    void eval_slot();
    void swap_slot();
    void delete_slot();
    void add_slot();
    void sum_slot();
    void cnt_slot();
};
#endif // CALCULATOR_H
