#ifndef AIO_H
#define AIO_H

#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

#include <QWidget>
#include"InputPanel/keyboard.h"

class AIO : public QWidget
{
    Q_OBJECT

public:
    AIO(QWidget *parent = 0);
    ~AIO();
    QTableWidget *itable,*otable;
    QPushButton *btn_aio[6];
    QPushButton *input_btn,*output_btn;
public slots:
    void find_num();
    void slot_name();
    void slot_item_clicked(QTableWidgetItem *item);
    void slot_showinput();
    void slot_showoutput();
};

#endif // AIO_H
