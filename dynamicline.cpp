#include "dynamicline.h"

#include <QKeyEvent>

QDynamicLine::QDynamicLine(QWidget *parent) :
    QLineEdit(parent)
{
    count++;
    next = nullptr;
    prev = last;
    if (last) {
        last->next = this;
    }
    last = this;
    this->setFixedHeight(50);
    this->setAlignment(Qt::AlignRight);
    this->setFont(QFont("Ubuntu", 22));
}

QDynamicLine::~QDynamicLine()
{
    count--;
    if (prev) {
        prev->next = next;
    }
    if (next) {
        next->prev = prev;
    } else {
        last = prev;
    }
}

int QDynamicLine::count = 0;
QDynamicLine* QDynamicLine::last = nullptr;
