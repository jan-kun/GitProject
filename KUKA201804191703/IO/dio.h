#ifndef DIO_H
#define DIO_H

#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

#include <QWidget>
#include"InputPanel/keyboard.h"
class DIO : public QWidget
{
    Q_OBJECT

public:

    DIO(QWidget *parent = 0);
    ~DIO();
    QTableWidget *ditable,*dotable;
    QPushButton *btn_io[6];
    QPushButton *input_btn,*output_btn;

public slots:
    void find_num();
    void slot_add();
    void slot_dec();
    void slot_name();
    void slot_item_clicked(QTableWidgetItem *item);

    void slot_setValue();
    void slot_showinput();
    void slot_showoutput();
};

#endif // DIO_H
