#ifndef CMD_LINE_H
#define CMD_LINE_H

#include <QWidget>
#include"Conditional.h"
#include"data.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif
class Cmd_Line : public QWidget
{
    Q_OBJECT
public:
    explicit Cmd_Line(QWidget *parent = 0);
    QFrame *frame_cmd = NULL;  //命令条框架
    QComboBox *cb_cmd;   //选择运动指令下拉框
    QComboBox *cb_cont;  //选择是否圆滑过渡下拉框
    QLineEdit *lineAuxPoint;  //辅助点
    QLineEdit *linePoint;   //目标点
    QLineEdit *lineSpd;  //输入速度
    QLineEdit *lineData;   //输入点数据
    QPushButton *Btn[2];
    QLabel *lab[2];
    /*选择工具和基座标界面控件*/
    QFrame *frame_TcpBase = NULL;  //设置TCP和BASE界面
    QComboBox *cb_TB[4];  //分别是选择工具、基座标、外部TCP、碰撞识别下拉框
    /*设置各类速度控件*/
    QFrame *frame_SpdZone = NULL;  //设置加速和圆滑过渡距离界面
    QPushButton *BtnAdjust[4];
    QLineEdit *lineAcce;
    QLineEdit *lineDist;
    /*设置各类速度和方向导引控件*/
    QFrame *frame_SpdDire = NULL;
    QPushButton *btnDire[4];
    QLineEdit *lineDire[2];
    QComboBox *cb_Dire;

    void func_BuildCmdLine();
    void func_AdjustCmdLine(QString cmd, QString auxpoint, QString point, bool cont , QString Spd , QString Data);
    void func_BuildTcpFrame();
    void func_showTcpFrame(int tool, int base, bool tcp, bool collision);
    void func_showTcpFrame();
    void func_BuildSpdDisFrame();
    void func_showSpdDisFrame(int acc,int dist);
    void func_showSpdDisFrame();
    void func_BuildSpdDireFrame();
    void func_showSpdDireFrame(int acc,int dist,int dire);
    void func_showSpdDireFrame();
    int func_GetCmdType();
    QStringList func_getFDATInfo();
    QStringList func_getPDATInfo();
    QStringList func_getLDATInfo();
    void func_UpdateCb();
    QString func_GetCmdStr(int type);  //0：PTP，1：LIN，2：CIRC，3：SLIN，4：SCIRC

signals:
    void Sig_PointFrame();
    void sig_cmdchange(int i);
public slots:
    void slot_Btn();
    void slot_cbClicked(int i);
};

#endif // CMD_LINE_H
