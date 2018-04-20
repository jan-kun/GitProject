#ifndef BUILD_BASE_H
#define BUILD_BASE_H

#include <QWidget>
#include"Conditional.h"
#include"data.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

class Build_Base : public QWidget
{
    Q_OBJECT

public:
    Build_Base(QWidget *parent = 0);
    ~Build_Base();

    QFrame *frame_base = NULL;
    QFrame *frame_child[4] = {NULL,NULL,NULL,NULL};
    QPushButton *btn_base[7];
    int count;    //表示测量到了几个点
    int currentpage;   //当前在第几页
    int BASENum;
    QString BASEName;
    int TCPNum;
    QString TCPName;
    double baseinfo[6];  //计算好的base坐标值保存在此

    /*第一页所需控件*/
    QLabel *lab_base1[6];
    QComboBox *cb_base;
    QLineEdit *line_name;   
    /*第二页所需控件*/
    QLabel *lab_toolname;
    QLabel *lab_base2[6];
    QComboBox *cb_tool;
    /*第三页所需控件*/
    QLabel *lab_base3[7];
    /*第四页所需控件*/
    QLabel *lab_base4[17];

    void frame_SelectBase();
    void frame_SelectTool();
    void frame_Orient();
    void frame_Res();
    
    void func_measure();
    void func_continue();
    void func_back();
    void func_save();
    int func_RealMeasure();

public slots:
    void slot_BtnClicked();
    void slot_CbClicked(int index);
};
#endif // BUILD_BASE_H
