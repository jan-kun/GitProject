#ifndef MENUBAR_H
#define MENUBAR_H

#include <QWidget>
#include"Conditional.h"
#include"data.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif
#include"MyWidget/menuwidget.h"

class MenuBar : public QWidget
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = 0);
    /*Left Widget*/
    QPushButton* frame_left = NULL;
    QLabel *lab_leftb[4];
    QLabel *lab_leftd[4];
    QTimer* timer_err;
    MenuWidget *frame_alarm = NULL;
    /*Up Widget*/
    QFrame* frame_up = NULL;
    QPushButton* btn_up[8];
    QLabel *lab_icon[8];
    QButtonGroup *Gbtn;
    MenuWidget *frame_upmenu[8];
    /*Right Widget*/
    QFrame* frame_right = NULL;
    QPushButton* btn_right[3];
    QLabel* lab_right[8];
    MenuWidget *frame_rightmenu[3];
    QButtonGroup *Gbtn1;
    QPalette pa;
    int flag_mod = 0;  //0 Jog , 1 World

    /*Alarm*/
    QLabel* lab_alarm[4];
    /*Submit*/
    QPushButton* btn_submit[3];
    /*Driver*/
    QPushButton* btn_driver[6];
    /*code*/
    QPushButton* btn_code[2];
    /*Spd*/
    QLabel* lab_spd[2];
    QPushButton* btn_spd[5];
    QSlider *slider[2];
    /*CodeMod*/
    QPushButton* btn_codemod[4];
    QRadioButton* rbtn_codemod[3];
    QButtonGroup* GbtnCodeMod;
    /*tool&base*/
    QPushButton* btn_ipo[2];
    QRadioButton* rbtn_ipo[2];
    QButtonGroup* GbtnIpo;
    QComboBox *cb_base;
    QComboBox* cb_tool;
    /*Incre*/
    QPushButton* btn_incre[6];
    QButtonGroup* GbtnIncre;
    /*mouse*/
    QPushButton* btn_mouse[5];
    QRadioButton* rbtn_mouse[4];
    QButtonGroup* GbtnMouse;
    /*Key*/
    QPushButton* btn_key[5];
    QRadioButton* rbtn_key[4];
    QButtonGroup* GbtnKey;
    void func_BuildAlarm(QWidget *parent, int x, int y);
    void func_BuildAlarmDetail();

    void func_BuildUp(QWidget* parent, int x, int y);
    void func_BuildSumit();
    void func_BuildDriver();
    void func_BuildCode();
    void func_BuildSpd();
    void func_BuildMod();
    void func_BuildTB();
    void func_BuildIncre();

    void func_BuildActmod(QWidget* parent, int x, int y);
    void func_BuildMActmod();
    void func_BuildKActmod();

    void func_showAlarm(int alarm[]);
    void func_hideAlarm();
    void func_showSubmit();
    void func_hideSubmit();
    void func_showDriver();
    void func_hideDriver();
    void func_showCode();
    void func_hideCode();
    void func_showSpd(int code,int hand);
    void func_hideSpd();
    void func_showMod(int mod);
    void func_hideMod();
    void func_showTB(int indext , int indexb , int ipo);
    void func_hideTB();
    void func_showIncre(int incre);
    void func_hideIncre();

    void func_showMActmod(int mod);
    void func_hideMActmod();
    void func_showKActmod(int mod);
    void func_hideKActmod();

    void func_UpdateSubmitState(int state);
    void func_UpdateDriver(int state);
    void func_UpdateRobMod(int mod);
    void func_UpdateSpd(int hand , int code);
    void func_UpdateCMod(int mod);
    void func_UpdateTB(int indext , int indexb , int ipo);
    void func_UpdateIncre(int incre);
    void func_UpdateMActmod(int mod);
    void func_UpdateKActmod(int mod);
    void func_UpdateAlarm(int alarm[]);

signals:
    void Sig_SubmitSelect();
    void Sig_SubmitStop();
    void Sig_SubmitDeselect();
    void Sig_CodeDeselect();
    void Sig_CodeReset();
public slots:
    void func_UpdateRobState();
    void slot_BtnGClicked(bool state);
    void slot_BtnG1Clicked(bool state);
    void slot_BtnGCodeModCliked(int id);
    void slot_btnGIPOClicked(int);
    void slot_BtnGIncre(int id);
    void slot_BtnGMCor(int id);
    void slot_BtnGKCor(int id);
    void slot_BtnClicked(bool state);
    void slot_Timerout();
    void slot_Slider(int val);
    void slot_CbClicked(int);
    void slot_aboutToHide();
    void slot_aboutToShow(MenuWidget* wid);

};

#endif // MENUBAR_H
