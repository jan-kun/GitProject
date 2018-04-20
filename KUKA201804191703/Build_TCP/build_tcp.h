#ifndef BUILD_TCP_H
#define BUILD_TCP_H

#include <QWidget>
#include"InputPanel/keyboard.h"
#include"Conditional.h"
#include"data.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif
#include"Protocol/RobConvert.h"

class Build_TCP : public QWidget
{
    Q_OBJECT

public:
    Build_TCP(QWidget *parent = 0);
    ~Build_TCP();
    typedef struct{
        int   iAixs[6];
    }TPoint;
    TPoint point[11];
    double res[7];
    double cor[3];
    QFrame *frame_base = NULL;
    QFrame *frame_child[4] = {NULL,NULL,NULL,NULL};
    int count;   //表示测量了几个方向
    int currentPage;   //表示当前在第几页
    int TCPNum;  //选中的工具编号
    QString TCPName;
    double toolinfo[6];  //工具位置：X,Y,Z,A,B,C
    double toolload[10];  //工具负载：mass,x,y,z,a,b,c,jx,jy,jz
    double tcpdata[16];   //工具坐标信息
    QStringList valuelist;
    /*第一页控件*/
    QLabel *lab_tcp1[15];
    QComboBox *cb_tool_no;
    QLineEdit *line_name;
    QPushButton *btn_tcp[7];
    /*第二页控件*/
    QLabel *lab_tcp2[6];
    /*第三页控件*/
    QLabel *lab_tcp3[18];
    QDoubleValidator *validator;
    QLineEdit *line_weight[10];
    /*第四页控件*/
    QLabel *lab_tcp4[13];

    void frame_SelectTool();
    void frame_Orient();
    void frame_Load();
    void frame_Res();

    void func_back();
    void func_contiue();
    void func_measure();
    int func_RealMeasure();   //真正测量函数
    void func_CalcToolData();
    void func_save();
    void func_BuildToolData();
    void func_CreatToolDataDecl(CodeData data);

public slots:
    void slot_BtnClicked();
    void slot_CbClicked(int index);
};
#endif // BUILD_TCP_H
