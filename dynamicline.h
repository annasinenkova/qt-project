#ifndef DYNAMICLINE_H
#define DYNAMICLINE_H

#include <QLineEdit>

class QDynamicLine : public QLineEdit
{
    Q_OBJECT
public:
    explicit QDynamicLine(QWidget *parent = 0);
    ~QDynamicLine();
    static int count;
    static QDynamicLine* last;
    QDynamicLine* next;
    QDynamicLine* prev;
};

#endif // DYNAMICLINE_H
