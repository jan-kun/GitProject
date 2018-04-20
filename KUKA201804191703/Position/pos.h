#ifndef POS_H
#define POS_H

#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

#include <QWidget>

class Pos : public QWidget
{
    Q_OBJECT

public:
    Pos(QWidget *parent = 0);
    ~Pos();
    int DKR_EXIST;
    QLabel *lab_tip;
    QTableWidget *dtable,*ztable;
    QPushButton *btn_mod,*btn_kong[5];
    void func_updateJogPos();
    void func_updateDescartesPos();

public slots:
    void slot_mod_changed();
    void Current_Pos_Changed();  //当前点位置发生变化调用此槽函数，点发生变化信号应该在移动中体现出来
};

#endif // POS_H
