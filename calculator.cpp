#include "calculator.h"
#include "ui_calculator.h"

#include <QKeyEvent>


calculator::calculator(QWidget *parent)
    : QMainWindow(parent)
    , focused(nullptr)
    , ui(new Ui::calculator)
{
    ui->setupUi(this);

    ui->verticalLayout->setAlignment(Qt::AlignTop);
    ui->second_value->installEventFilter(this);

    add_slot();

    key_add = new QShortcut(this);
    key_add->setKey(Qt::Key_A);
    key_sum = new QShortcut(this);
    key_sum->setKey(Qt::Key_W);
    key_cnt = new QShortcut(this);
    key_cnt->setKey(Qt::Key_E);

    connect(key_add, SIGNAL(activated()), this, SLOT(add_slot()));
    connect(key_sum, SIGNAL(activated()), this, SLOT(sum_slot()));
    connect(key_cnt, SIGNAL(activated()), this, SLOT(cnt_slot()));

    connect(focused, SIGNAL(textChanged(QString)), this, SLOT(eval_slot()));
    connect(ui->second_value, SIGNAL(textChanged(QString)), this, SLOT(eval_slot()));
}

calculator::~calculator()
{
    delete ui;
}

void calculator::eval_slot()
{
    if (!focused) {
        return;
    }
    if (ui->second_value->text().isEmpty() or focused->text().isEmpty()) {
        ui->sum_lbl->clear();
        ui->sub_lbl->clear();
        ui->mul_lbl->clear();
        ui->div_lbl->clear();
        return;
    }
    double first_value = focused->text().toDouble();
    double second_value = ui->second_value->text().toDouble();
    QString sum = QString::number(first_value + second_value);
    QString sub = QString::number(first_value - second_value);
    QString mul = QString::number(first_value * second_value);
    QString div = QString::number(first_value / second_value);
    ui->sum_lbl->setText(sum);
    ui->sub_lbl->setText(sub);
    ui->mul_lbl->setText(mul);
    ui->div_lbl->setText(div);
}

void calculator::swap_slot()
{
    if (focused) {
        QString first_value = focused->text();
        focused->setText(ui->second_value->text());
        ui->second_value->setText(first_value);
    }
}

void calculator::add_slot()
{
    if (focused) {
        focused->setStyleSheet("color: black");
    }
    QDynamicLine* line = new QDynamicLine(this);
    ui->verticalLayout->addWidget(line);
    line->installEventFilter(this);
    line->setFocus();
    focused = line;
    connect(line, SIGNAL(textChanged(QString)), this, SLOT(eval_slot()));
}

void calculator::delete_slot()
{
    if (QDynamicLine::count > 1 and focused) {
        focused->clearFocus();
        focused->hide();
        QDynamicLine* prev;
        if (focused->prev) {
            prev = focused->prev;
        } else {
            prev = focused->next;
        }
        delete focused;
        focused = prev;
        if (prev) {
            prev->setFocus();
            prev->setStyleSheet("color: red");
        }
    }
}

void calculator::sum_slot()
{
    if (focused) {
        focused->setStyleSheet("color: black");
    }
    QDynamicLine* last = QDynamicLine::last;
    double sum = 0;
    while (last) {
        sum = sum + last->text().toDouble();
        last = last->prev;
    }
    ui->second_value->setText(QString::number(sum));
    ui->sum_lbl->clear();
    ui->sub_lbl->clear();
    ui->mul_lbl->clear();
    ui->div_lbl->clear();
    return;
}

void calculator::cnt_slot()
{
    add_slot();
    focused->setText(ui->second_value->text());
    return;
}

bool calculator::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != ui->second_value) {
        if (event->type() == QEvent::FocusIn) {
            focused = static_cast<QDynamicLine*>(watched);
            focused->setStyleSheet("color: red");
            eval_slot();
            return true;
        }
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_D) {
                delete_slot();
                return true;
            }
            if (keyEvent->key() == Qt::Key_Down) {
                if (focused->next) {
                    focused->setStyleSheet("color: black");
                    focused = focused->next;
                    focused->setFocus();
                }
                return true;
            }
            if (keyEvent->key() == Qt::Key_Up) {
                if (focused->prev) {
                    focused->setStyleSheet("color: black");
                    focused = focused->prev;
                    focused->setFocus();
                }
                return true;
            }
            if (keyEvent->key() == Qt::Key_Right) {
                ui->second_value->setFocus();
                return true;
            }
            if (keyEvent->key() == Qt::Key_Minus) {
                if (!focused->text().isEmpty()) {
                    return true;
                }
                return false;
            }
            if (keyEvent->key() == Qt::Key_Period) {
                if (focused->text().contains('.')) {
                    return true;
                }
                return false;
            }
            if (keyEvent->key() == Qt::Key_Delete) {
                focused->clear();
                return true;
            }
        }
    }
    if (watched == ui->second_value) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Left) {
                focused->setFocus();
                return true;
            }
            if (keyEvent->key() == Qt::Key_Minus) {
                if (!ui->second_value->text().isEmpty()) {
                    return true;
                }
                return false;
            }
            if (keyEvent->key() == Qt::Key_Period) {
                if (ui->second_value->text().contains('.')) {
                    return true;
                }
                return false;
            }
            if (keyEvent->key() == Qt::Key_Delete) {
                ui->second_value->clear();
                return true;
            }
        }
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_S) {
            swap_slot();
            return true;
        }
        if (keyEvent->key() == Qt::Key_A) {
            add_slot();
            return true;
        }
        if (keyEvent->key() == Qt::Key_W) {
            sum_slot();
            return true;
        }
        if (keyEvent->key() == Qt::Key_E) {
            cnt_slot();
            return true;
        }
        if (keyEvent->key() != Qt::Key_0 and
            keyEvent->key() != Qt::Key_1 and
            keyEvent->key() != Qt::Key_2 and
            keyEvent->key() != Qt::Key_3 and
            keyEvent->key() != Qt::Key_4 and
            keyEvent->key() != Qt::Key_5 and
            keyEvent->key() != Qt::Key_6 and
            keyEvent->key() != Qt::Key_7 and
            keyEvent->key() != Qt::Key_8 and
            keyEvent->key() != Qt::Key_9 and
            keyEvent->key() != Qt::Key_Delete and
            keyEvent->key() != Qt::Key_Backspace)
        {
            eval_slot();
            return true;
        }
    }
    return false;
}
