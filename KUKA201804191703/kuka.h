#ifndef KUKA_H
#define KUKA_H

#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

#include <QMainWindow>
#include "FileMange/filemange.h"
#include "IO/dio.h"
#include"AIO/aio.h"
#include"Position/pos.h"
#include"MyDial/mydial.h"
#include"INFO/info.h"
#include"UserGroup/usergroup.h"
#include"SigVar/sigvar.h"
#include"Build_TCP/build_tcp.h"
#include"Build_TCP/tcp_in.h"
#include"Build_Base/build_base.h"
#include"Build_Base/base_in.h"
#include"Power/power.h"
#include"Port/port.h"
#include"Protocol/IO.h"
#include"MyWidget/menuwidget.h"
#include"MenuBar/menubar.h"
#include"UpdateSys/updatesys.h"
#include"Parser/parser.h"
typedef void (*fun_task)();

class KUKA : public QMainWindow
{
    Q_OBJECT

public:
    KUKA(QWidget *parent = 0);
    ~KUKA();
    FileMange *File_Manager;
    DIO *IOWidget;
    AIO *AIOWidget;
    Pos *POSWidget;
    UserGroup *UserWidget = NULL;
    SigVar *SigVarWidget = NULL;
    Build_TCP *TCP_4_Widget = NULL;
    TCP_IN *TCP_DI_Widget = NULL;
    Build_Base *Base_3_Widget = NULL;
    Base_IN *Base_DI_Widget = NULL;
    INFO *INFOWidget;
    MyDial *MyDialWidget;
    Power *PowerWidget;
    MenuBar *MenuBarWidget = NULL;
    Port *SendPort = NULL;
    UpdateSys *UpdateWidget = NULL;
    Parser *funcParser;

//    Receive_Thread *RecePort;

    QString old_stysheet,new_stylesheet;
    QFrame *allframe,*mainframe;
    QLabel *lab_frame[3]={NULL,NULL,NULL};

    QLabel *lab_show_tip;   //用来显示当前报警或提示信息
    QPushButton *btn_close;   //左侧栏的关闭按钮，用于关闭打开的各个界面
    QPushButton *btn_pen;   //左侧栏的笔按钮，点击调出键盘
    QFrame *frame_home = NULL;
    QScrollArea *mainscroll;
    QLabel *main_tip,*tip_lab[6];
    QFrame *childframe[6]={NULL};
    QFrame *frame_info;   //显示具体报警信息页面
    QPushButton *btn_machine;
    QLabel *lab_time,*lab_up_tip[2];
    QPushButton *btn_ask,*btn_ok;   //信息提示栏右侧的一个问号按钮和一个确认按钮

    QPushButton *btn_home,*btn_back;
    QPushButton *btn_frame0[7],*btn_frame1[9],*btn_frame2[7];
    QPushButton *btn_frame3[7],*btn_frame4[3],*btn_frame5[2];
    QLabel *lab_arrow0[7],*lab_arrow1[9],*lab_arrow2[7],*lab_arrow3[7],*lab_arrow4[3];

    /**/
    QFrame* frame_version = NULL;

    /*为创建近期任务所需*/
    int Num_Of_Task,temp_Num_of_Task;
    typedef struct task_manger{
        void (KUKA::*fun_task[6])();
        int size;
        int putp;
        char* fun_name[6];
//        QString fun_name[6];
    }task_fifo;
    task_fifo* my_task_manager;
    QFrame *frame_task;
    QPushButton *btn_task[6];
    int flag_task_exist;
    QTimer *timer_Info;

    QTimer *StateTimer;

    void CreatPath(QString path);
    void CreatAllPath();
    void func_initmap();
    void func_initSysData();
    void init_UpFrame();    //初始化上栏界面
    void init_LeftFrame();   //初始化左侧栏界面
    void init_rightFrame();    //初始化最右侧栏上的控件显示
    void init_infoFrame();  //初始化日志显示界面
    void init_info_window();  //初始化信息显示窗口。用来显示各个报警或提示信息
    void init_mainWindow();   //建立中间的主功能界面
    void func_checkState();

    void func_iteratorAllDir(QString path);
    void func_initCodedata();

    void init_taskWindow();    //初始化近期任务窗口
    int find_has_existed(void (KUKA::*funp)());
    void init_arrow();
    void hide_arrow(int ID);
    void setarrow(QPushButton *btn, QLabel *lab);
    QString func_getCurTime();   //获取当前时间，为报警或提示信息提供时间戳
    QString get_tipcontext();   //获取警报提示文本

    void fifo_init(task_fifo* fifo,int size);
    void fifo_put(task_fifo* fifo, void (KUKA::*funp)(), char *str);

    /*产生报警信息，int表示报警等级，QString表示报警文本信息,QString表示显示在信息标签上的简略信息*/
    void Creat_Alert_Info(int,QString,QString);

    void func_showTcp4();    //显示4点法建立TCP界面
    void func_showTcpDI();   //显示数字输入建立TCP界面
    void func_showBase3();   //显示3点法建立Base界面
    void func_showBaseDI();   //显示数字输入建立Base界面
    void func_showFileManger();   //显示文本管理器界面
    void func_showDIO();   //显示数字IO界面
    void func_showAIO();   //显示模拟IO界面
    void func_showVar();  //显示单项变量界面
    void func_showPos();   //显示当前位置界面
    void func_showPower();  //显示关机界面
    void func_showUser();  //显示用户组界面
    void func_showUpdate();
    void func_showSysVersion();
    void func_HandMove(int n);

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void Num_Of_Task_Changed();
    void num_changed(int warningID , QString str);
    void No_Tip();
public slots:
    void func_showMainWindow();
    void func_showHome();
    void slot_btn_back();
    void slot_btn_frame0_clicked();
    void slot_btn_frame1_clicked();
    void slot_btn_frame2_clicked();
    void slot_btn_frame3_clicked();
    void slot_btn_frame4_clicked();
    void slot_btn_frame5_clicked();
    void slot_timerout();
    void func_updateState();

    void slot_TCP_Save();   //TCP示教完成后保存功能
    void slot_TCP_DI_Save();    //TCP数字输入完成后的保存功能

    void slot_infolab_color_changed();
    void func_ConfirmInfo();   //信息栏OK按钮，用以消除报警信息
    void slot_Cancel_All_Info();    //信息窗口全部OK按钮，用以消除全部报警信息
    void slot_info_count();
    void slot_tasknum_changed();
    void slot_task_inter();
    void slot_close();
};

#endif // KUKA_H
