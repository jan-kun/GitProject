#ifndef UPDATESYS_H
#define UPDATESYS_H

#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif
#include"Protocol/robot.h"

class UpdateSys : public QWidget
{
    Q_OBJECT
public:
    explicit UpdateSys(QWidget *parent = 0);

    QFrame* frame_base;
    QFrame *frame_update = NULL;
    QPushButton *btn_updatefunc;
    QLineEdit *line_mima;

    QFrame *frame_size = NULL;
    QLineEdit *line_size[6];
    QPushButton *btn_sizefunc[2];
    QDoubleValidator *validator;

    void frame_updateSys();
    int func_mountdisk();
    void updatesys(QString mima);
    void frame_setRobSize();
    void func_savexSys();

signals:

public slots:
    void slot_btn();

};

#endif // UPDATESYS_H
