#ifndef INFO_H
#define INFO_H

#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

#include <QWidget>
#include"mystandarditem.h"

#define MAX_Tip_NUM 100

class INFO : public QWidget
{
    Q_OBJECT

public:
    INFO(QWidget *parent = 0);
    ~INFO();
    QWidget *widget;
    QStandardItemModel *model;
    QListView *view;
    MyStandardItem *item[MAX_Tip_NUM];
    MyStandardItem *current_item;

    QLabel *lab_noinfo;
    int Num_OF_Info,temp_num;
    int Num_OF_ERROR,Num_OF_Warning;
    int Num_OF_Tip,Num_OF_Waiting;

signals:
//    void num_changed(int warningID , QString str);
    void num_of_error_changed();
    void num_of_warning_changed();
    void num_of_tip_changed();

public slots:
    void slot_change_num_of_info(int warningID , QString str);
    void slot_select_info(QModelIndex index);
    void slot_Has_No_info();
};

#endif // INFO_H
