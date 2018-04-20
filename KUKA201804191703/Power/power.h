#ifndef Power_H
#define Power_H

#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

#include <QWidget>
#include<QRadioButton>
#include<QGroupBox>
#include<QLabel>
#include<QPushButton>
#include<QSlider>
class Power : public QWidget
{
    Q_OBJECT

public:
    Power(QWidget *parent = 0);
    QFrame *frame_base;
    QLabel *title1,*title2,*title3,*title1_1,*title1_2,*title3_1,*title3_2,*lab_time;
    QGroupBox *group1;
    QButtonGroup *bGroup_Stype;
    QPushButton *btn_base[5];
    QPushButton *btn_option[5];
    QRadioButton *rBtn[2];

    QCheckBox *cBtn[3];
    QPushButton *btn_del,*btn_add;
    QSlider *slider_time;
    int time;
    QPushButton *btn_closePC,*btn_restartPC,*btn_light,*btn_i,*btn_o;

    void init_frame();
    void func_setWaitTime(int v);
    int func_getWaitTime();

public slots:
    void slot_btnClicked();
    void slot_rBtnClicked(int id);
    void slot_cBtnClicked();
    void slot_SliderValueChg(int v);
};

#endif // Power_H
