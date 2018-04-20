#include "kuka.h"
#include <QWSKeyboardHandler>

#define CREAT_INFO(a) a++;
#define CONFIRM_INFO(a) --a;
#define MOVEANGE 100
QString str_of_tipcontext;
QString space="_____________________________________________________________________";
QLabel *lab_infotext;
QLabel *lab_info_img;
QList<QWidget*> listwidget;

QRegExp rg_var("([a-z]|[A-Z]){1,}\\w{0,30}"); //命名规范：字母开头，由字母数字或下划线组成，最多30个字符
SysRob KukaRob;
QFileInfoList dirInfoList;
QFileInfoList DataFileList;  //保存目录下所有的.dat数据文件
QFileInfoList ParserFileList;   //保存目录下的所有待译码的文件
QList<ModuleI> ModuleInfo;

KUKA::KUKA(QWidget *parent)
    : QMainWindow(parent)
{
#if ARM_PC
    setWindowFlags(Qt::FramelessWindowHint);
#endif
    /*基页面，尺寸为600*800*/
    resize(600,800);
    allframe=new QFrame(this);
    allframe->setGeometry(0,0,600,800);

    CreatAllPath();
    func_initmap();
    func_initSysData();
    init_UpFrame();
    init_LeftFrame();
    init_rightFrame();
    init_infoFrame();
    init_info_window();
    init_mainWindow();
    init_taskWindow();
    func_checkState();
    func_iteratorAllDir(KUKA_MAIZI);
    func_initCodedata();

    timer_Info=new QTimer(this);
    connect(timer_Info,SIGNAL(timeout()),this,SLOT(slot_infolab_color_changed()));
    MenuBarWidget->frame_alarm->installEventFilter(this);

    File_Manager=new FileMange(allframe);
    File_Manager->move(53,106);
    File_Manager->hide();
    connect(MenuBarWidget,SIGNAL(Sig_CodeDeselect()),File_Manager->win_CodePad,SLOT(func_codedelselect()));

    IOWidget = new DIO(allframe);
    IOWidget->move(53,106);
    IOWidget->hide();

    AIOWidget = new AIO(allframe);
    AIOWidget->move(53,106);
    AIOWidget->hide();

    POSWidget = new Pos(allframe);
    POSWidget->move(53,106);
    POSWidget->hide();

    PowerWidget=new Power(allframe);
    PowerWidget->move(53,106);
    PowerWidget->hide();

    flag_task_exist=0;
}

/*
** 函数功能：创建文件夹
** 函数参数：path，文件夹路径
** 函数返回值：无
** 函数说明：创建失败则弹出警告框
*/
void KUKA::CreatPath(QString path)
{
    QDir *temp = new QDir;
    bool exist = temp->exists(path);
    if( !exist )
    {
        bool ok = temp->mkdir(path);
        if( !ok )
            QMessageBox::warning(this,tr("创建文件夹"),tr("无操作权限，无法创建程序目录，请联系供应商"));
    }
}

/*
** 函数功能：创建所有需要的文件夹
** 函数参数：无
** 函数返回值：无
*/
void KUKA::CreatAllPath()
{
    CreatPath(KUKA_PATH);
    CreatPath(KUKA_MAIZI);
    CreatPath(KUKA_ARCHIVE);
    CreatPath(KUKA_R1);
    CreatPath(KUKA_Pro);
    CreatPath(KUKA_SYS);
    CreatPath(KUKA_USB);
    CreatPath(KUKA_PARA);
}

/*
** 函数功能：初始化各个数据类型对应的整形值
** 函数参数：无
** 函数返回值：无
*/
void KUKA::func_initmap()
{
    QString DataTypName[CNT_Type]={"BOOL","INT","REAL","CHAR","AXIS",\
                                      "E6AXIS","FRAME","POS","E6POS","FDAT","PDAT","LDAT",\
                                       "BASEDAT","LOADDAT","TOOLDAT"};
    for(int i=0;i<CNT_Type;i++)
            TypeToIndex.insert(DataTypName[i] , i);

    QString cmdtype[CNT_Cmd]={"PTP","LIN","CIRC","SLIN","SCIRC"};
    for(int i=0;i<CNT_Cmd;i++)
            CMDToIndex.insert(cmdtype[i] , i);

}

/*
** 函数功能：初始化必要的系统数据，如32个基座标和16个工具坐标
** 函数参数：无
** 函数返回值：无
*/
void KUKA::func_initSysData()
{
    KukaRob.Model = Manual1;
    KukaRob.Motor = 0;
    KukaRob.PMotor = 0;
    KukaRob.EMG = 1;
    KukaRob.PEMG = 1;
    KukaRob.PVel = 100;
    KukaRob.MVel = 2;
    KukaRob.User = Operator;
    flag_codeeditor = 0;
    KukaRob.Incre = 1;

    QStringList basevarlist;
    basevarlist.clear();
    basevarlist<<"0"<<"0"<<"0"<<"0"<<"0"<<"0";

    for(int i=0;i<32;i++)
    {
        CodeData data_base;
        data_base.name = tr("未命名");
        data_base.val.basedatVal.setbasedatval(basevarlist);
        data_base.Svalue = "X 0 , Y 0 , Z 0 , A 0 , B 0 , C 0";
        data_base.dtype = basedattype;
        data_base.stotype = Variable;
        data_base.module = "";
        data_base.code = "";
        data_base.dimension = 1;
        codedata[12].append(data_base);
    }

    QStringList toolvallist;
    toolvallist.clear();
    toolvallist<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"-1"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0";

    for(int i=0;i<16;i++)
    {
        CodeData data_tool;
        data_tool.name = tr("未命名");
        data_tool.val.tooldatVal.settooldatval(toolvallist);
        data_tool.Svalue = "X 0 , Y 0 , Z 0 , A 0 , B 0 , C 0 , Mass -1 , X 0 , Y 0 , Z 0 , A 0 , B 0 , C 0 , JX 0 , JY 0 , JZ 0";
        data_tool.dtype = tooldatatype;
        data_tool.stotype = Variable;
        data_tool.module = "";
        data_tool.code = "";
        data_tool.dimension = 1;
        codedata[14].append(data_tool);
    }

    QStringList pointvallist;
    pointvallist.clear();
    pointvallist<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"2"<<"0";
    CodeData homepoint;
    homepoint.name = "HOME";
    homepoint.val.e6posVal.sete6posval(pointvallist);
    homepoint.Svalue = "{X 0 , Y 0 , Z 0 , A 0 , B 0 , C 0 , S 2 , T 0 , E1 0 , E2 0 , E3 0 , E4 0 , E5 0 , E6 0}";
    homepoint.dtype = e6postype;
    homepoint.stotype = Variable;
    homepoint.module = "";
    homepoint.code = "";
    homepoint.dimension = 1;
    codedata[8].append(homepoint);

    QStringList pdatvallist;
    pdatvallist.clear();
    pdatvallist<<"100"<<"100"<<"100"<<"0";
    CodeData pdefault;
    pdefault.name = "DEFAULT";
    pdefault.val.pdatVal.setpdatval(pdatvallist);
    pdefault.Svalue = "{VEL 100 , ACC 100 , APO_DIST 100}";
    pdefault.dtype = pdattype;
    pdefault.stotype = Variable;
    pdefault.module = "";
    pdefault.code = "";
    pdefault.dimension = 1;
    codedata[10].append(pdefault);

    QStringList fdatvallist;
    fdatvallist.clear();
    pdatvallist<<"0"<<"0"<<"1"<<""<<"FALSE";
    CodeData fhome;
    fhome.name = "FHOME";
    fhome.val.fdatVal.setfdatval(fdatvallist);
    fhome.Svalue = "{ TOOL_NO 0 , BASE_NO 0 , IPO_FRAME #BASE , POINT2[] " " , TQ_STATE False }";
    fhome.dtype = fdattype;
    fhome.stotype = Variable;
    fhome.module = "";
    fhome.code = "";
    fhome.dimension = 1;
    codedata[9].append(fhome);
}

/*
** 函数功能：初始化上栏界面
** 函数参数：无
** 函数返回值：无
*/
void KUKA::init_UpFrame()
{
    MenuBarWidget = new MenuBar(this);

    lab_frame[0]=new QLabel(allframe);
    lab_frame[0]->setGeometry(0,0,600,53);
    lab_frame[0]->setStyleSheet("background-color:rgb(160,165,170)");

    /*机器人按钮，按下调处主页面*/
    btn_machine=new QPushButton(lab_frame[0]);
    btn_machine->setGeometry(0,0,53,53);
    btn_machine->show();
    btn_machine->setFocusPolicy(Qt::NoFocus);
    btn_machine->setStyleSheet("border-image:url(:/machine.png)");
    connect(btn_machine,SIGNAL(clicked(bool)),this,SLOT(func_showMainWindow()));

    for(int i=0;i<2;i++)
    {
        lab_up_tip[i]=new QLabel(lab_frame[0]);
        lab_up_tip[i]->setGeometry(55,3+24*i,145,21);
        lab_up_tip[i]->show();
        lab_up_tip[i]->setText(tr("0"));
        lab_up_tip[i]->setStyleSheet("background-color:rgb(82,90,94);color:white;border-radius:3px");
    }
    MenuBarWidget->func_BuildUp(lab_frame[0],200,0);
}

/*
** 函数功能：初始化左侧界面
** 函数参数：无
** 函数返回值：无
*/
void KUKA::init_LeftFrame()
{
    lab_frame[1]=new QLabel(allframe);
    lab_frame[1]->setGeometry(0,53,53,747);
    lab_frame[1]->setStyleSheet("background-color:rgb(160,165,170)");

    /*左侧四栏状态按钮,用于显示当前各类报警信息个数*/
    MenuBarWidget->func_BuildAlarm(lab_frame[1],0,0);

    btn_close = new QPushButton(lab_frame[1]);
    btn_close->setGeometry(13,110,40,40);
    btn_close->setFocusPolicy(Qt::NoFocus);
    btn_close->setStyleSheet("background-color:rgb(254,138,59)");
    btn_close->setStyleSheet("border-image:url(:/close.PNG);border-radius: 5px");
    btn_close->hide();
    connect(btn_close,SIGNAL(clicked(bool)),this,SLOT(slot_close()));

    btn_pen = new QPushButton(lab_frame[1]);
    btn_pen->setGeometry(0,180,60,60);
    btn_pen->setFocusPolicy(Qt::NoFocus);
    btn_pen->setStyleSheet("border-radius: 25px");
    btn_pen->setIcon(QIcon(":/image/image/pen.png"));
    btn_pen->setIconSize(QSize(60,60));
}

/*
** 函数功能：初始化右侧界面
** 函数参数：无
** 函数返回值：无
*/
void KUKA::init_rightFrame()
{
    lab_frame[2]=new QLabel(allframe);
    lab_frame[2]->setGeometry(547,53,53,747);
    lab_frame[2]->setStyleSheet("background-color:rgb(160,165,170)");

    MenuBarWidget->func_BuildActmod(lab_frame[2],0,0);
}

/*
** 函数功能：初始化整体信息提示栏标签
** 函数参数：无
** 函数返回值：无
*/
void KUKA::init_infoFrame()
{
    /*整体信息提示栏标签*/
    lab_show_tip=new QLabel(allframe);  //用来显示当前报警或提示信息
    lab_show_tip->setGeometry(53,53,494,53);
    lab_show_tip->setStyleSheet("background-color:rgb(230,230,230)");
    lab_show_tip->show();
    lab_show_tip->installEventFilter(this);
    /*显示信息提示图标标签*/
    lab_info_img=new QLabel(lab_show_tip);
    lab_info_img->setGeometry(5,2,20,20);
    /*设置实时时间标签*/
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slot_timerout()));
    timer->start(1000);
    lab_time=new QLabel(lab_show_tip);
    lab_time->setGeometry(37,2,280,20);

    /*信息提示栏上显示当前具体信息提示的标签*/
    lab_infotext=new QLabel(lab_show_tip);
    lab_infotext->setGeometry(5,25,280,28);

    btn_ask = new QPushButton(lab_show_tip);
    btn_ask->setGeometry(360,5,62,42);
    btn_ask->setFocusPolicy(Qt::NoFocus);
    btn_ask->setIconSize(QSize(62,42));
    btn_ask->setIcon(QIcon(":/image/image/ask.png"));
    btn_ask->setStyleSheet(" QPushButton:pressed {background-image: url(:/image/image/ok.png) ;}");

    btn_ok = new QPushButton(lab_show_tip);
    btn_ok->setGeometry(428,5,62,42);
    btn_ok->setFocusPolicy(Qt::NoFocus);
    btn_ok->setIconSize(QSize(62,42));
    btn_ok->setIcon(QIcon(":/image/image/ok.png"));
    connect(btn_ok,SIGNAL(clicked(bool)),this,SLOT(func_ConfirmInfo()));
}

/*
** 函数功能：初始化显示所有日志的界面
** 函数参数：无
** 函数返回值：无
*/
void KUKA::init_info_window()
{
    frame_info = new QFrame(allframe);
    frame_info->setGeometry(53,106,494,694);
    frame_info->setStyleSheet("background-color:rgb(230,230,230)");
    frame_info->hide();
    INFOWidget=new INFO(frame_info);
    INFOWidget->move(0,0);
    connect(INFOWidget,SIGNAL(num_of_error_changed()),this,SLOT(slot_info_count()));
    connect(INFOWidget,SIGNAL(num_of_warning_changed()),this,SLOT(slot_info_count()));
    connect(INFOWidget,SIGNAL(num_of_tip_changed()),this,SLOT(slot_info_count()));
    connect(this,SIGNAL(num_changed(int,QString)),INFOWidget,SLOT(slot_change_num_of_info(int,QString)));
    connect(this,SIGNAL(No_Tip()),INFOWidget,SLOT(slot_Has_No_info()));

    QFrame *frame_ctl=new QFrame(frame_info);
    frame_ctl->setGeometry(424,0,70,694);
    QPushButton *btn_allok=new QPushButton(tr("全部OK"),frame_ctl);
    btn_allok->setFocusPolicy(Qt::NoFocus);
    btn_allok->setGeometry(5,5,60,45);
    connect(btn_allok,SIGNAL(clicked(bool)),this,SLOT(slot_Cancel_All_Info()));

    QScrollBar *slider=new QScrollBar(frame_ctl);
    slider->setGeometry(5,50,60,644);
}

/*
** 函数功能：建立中间的主功能界面
** 函数参数：无
** 函数返回值：无
** 函数说明：一开始frame_home是隐藏的，点击左上角机器人按钮才会显示出来
*/
void KUKA::init_mainWindow()
{
    frame_home = new QFrame(allframe);
    frame_home->setGeometry(53,106,494,694);
    frame_home->setObjectName("fh");
    frame_home->setStyleSheet("#fh{border:8px solid rgb(254,138,59);background-color:rgb(230,230,230)}");
    frame_home->hide();

    /*标题菜单*/
    main_tip=new QLabel(frame_home);
    main_tip->setGeometry(0,0,494,30);
    main_tip->setText(tr("主菜单"));   //红底白字
    main_tip->setStyleSheet("background-color:rgb(254,138,59);color:white;font:bold;font-size:14px;padding-left:15px");
    main_tip->show();
    /*主页面按钮*/
    btn_home=new QPushButton(frame_home);
    btn_home->setGeometry(13,35,62,42);
    btn_home->setFocusPolicy(Qt::NoFocus);
    btn_home->setIcon(QIcon(":/image/image/home.png"));
    btn_home->setIconSize(QSize(62,42));
    btn_home->setStyleSheet("border-radius: 3px");
    btn_home->show();
    connect(btn_home,SIGNAL(clicked()),this,SLOT(func_showHome()));
    /*返回上一页按钮*/
    btn_back = new QPushButton(frame_home);
    btn_back->setGeometry(85,35,62,42);
    btn_back->setFocusPolicy(Qt::NoFocus);
    btn_back->setIconSize(QSize(62,42));
    btn_back->setIcon(QIcon(":/image/image/back.png"));
    btn_back->setStyleSheet("border-radius: 3px");
    btn_back->show();
    connect(btn_back,SIGNAL(clicked()),this,SLOT(slot_btn_back()));
    /*主页面，总共有六栏，用qscrollarea提供滚动条来显示全部*/
    mainscroll=new QScrollArea(frame_home);
    mainscroll->setGeometry(8,82,478,518);
    mainscroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainscroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mainframe=new QFrame(frame_home);
    QSizePolicy main_SizePolicy = mainframe->sizePolicy();
    main_SizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    mainframe->setSizePolicy(main_SizePolicy);
    mainframe->setMinimumSize(478,518);
    mainframe->setStyleSheet("background-color:rgb(230,230,230)");
    mainframe->move(8,82);
    mainscroll->setWidget(mainframe);
    /*六栏菜单项上的提示标签*/
    for(int i=0;i<6;i++)
    {
        tip_lab[i]=new QLabel(mainframe);
        tip_lab[i]->setGeometry(0+i*164,0,160,30);
        tip_lab[i]->hide();
        tip_lab[i]->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);  //字体设置居中
        tip_lab[i]->setStyleSheet("font:bold;font-size:14px;color:white;background-color:rgb(160,165,170)");
    }
    tip_lab[0]->show();
    tip_lab[0]->setText(tr("主菜单"));
    
    init_arrow();
    /*六栏菜单项界面*/
    for(int i=0;i<6;i++)
    {
        childframe[i]=new QFrame(mainframe);
        childframe[i]->setGeometry(0+i*164,30,160,574);
        childframe[i]->setObjectName("cfc"+QString::number(i));
        childframe[i]->setStyleSheet("#cfc"+QString::number(i)+"{background-color:rgb(230,230,230);border-right:2px solid rgb(160,165,170);}");
        childframe[i]->hide();
    }
    childframe[0]->show();
    /*第一栏菜单下的所有按钮选项*/
    QString str0[7] = {"文件","配置","显示","诊断","投入运行","关机","帮助"};
    for(int i=0;i<7;i++)
    {
        btn_frame0[i]=new QPushButton(childframe[0]);
        btn_frame0[i]->setGeometry(0,0+i*54,160,50);
        btn_frame0[i]->setFocusPolicy(Qt::NoFocus);
        btn_frame0[i]->setText(str0[i]);
//            btn_frame0[i]->setStyleSheet("text-align :  left center;padding-left:10px;padding-right:1px");
        btn_frame0[i]->setStyleSheet("background-color:rgb(248,248,248);");
        connect(btn_frame0[i],SIGNAL(clicked(bool)),this,SLOT(slot_btn_frame0_clicked()));
        btn_frame0[i]->show();
        if( i != 5)
            setarrow(btn_frame0[i],lab_arrow0[i]);
    }
    old_stysheet=btn_frame0[0]->styleSheet();
    new_stylesheet="background-color:rgb(57,185,255)";
    /*第二栏菜单下的所有按钮选项*/
    for(int i=0;i<9;i++)
    {
        btn_frame1[i]=new QPushButton(childframe[1]);
        btn_frame1[i]->setGeometry(0,0+i*54,160,50);
        btn_frame1[i]->setFocusPolicy(Qt::NoFocus);
        btn_frame1[i]->setStyleSheet("background-color:rgb(248,248,248)");
        connect(btn_frame1[i],SIGNAL(clicked(bool)),this,SLOT(slot_btn_frame1_clicked()));
        btn_frame1[i]->show();
    }
    /*第三栏菜单下的所有按钮选项*/
    for(int i=0;i<7;i++)
    {
        btn_frame2[i]=new QPushButton(childframe[2]);
        btn_frame2[i]->setGeometry(0,0+i*54,160,50);
        btn_frame2[i]->setFocusPolicy(Qt::NoFocus);
        btn_frame2[i]->setStyleSheet("background-color:rgb(248,248,248)");
        connect(btn_frame2[i],SIGNAL(clicked(bool)),this,SLOT(slot_btn_frame2_clicked()));
        btn_frame2[i]->show();
    }
    /*第四栏菜单下的所有按钮选项*/
    for(int i=0;i<7;i++)
    {
        btn_frame3[i]=new QPushButton(childframe[3]);
        btn_frame3[i]->setGeometry(0,0+i*54,160,50);
        btn_frame3[i]->setFocusPolicy(Qt::NoFocus);
        btn_frame3[i]->setStyleSheet("background-color:rgb(248,248,248)");
        connect(btn_frame3[i],SIGNAL(clicked(bool)),this,SLOT(slot_btn_frame3_clicked()));
        btn_frame3[i]->show();
    }
    /*第五栏菜单下的所有按钮选项*/
    for(int i=0;i<3;i++)
    {
        btn_frame4[i]=new QPushButton(childframe[4]);
        btn_frame4[i]->setGeometry(0,0+i*54,160,50);
        btn_frame4[i]->setFocusPolicy(Qt::NoFocus);
        btn_frame4[i]->setStyleSheet("background-color:rgb(248,248,248)");
        connect(btn_frame4[i],SIGNAL(clicked(bool)),this,SLOT(slot_btn_frame4_clicked()));
        btn_frame4[i]->show();
    }
    /*第六栏菜单下的所有按钮选项*/
    for(int i=0;i<2;i++)
    {
        btn_frame5[i]=new QPushButton(childframe[5]);
        btn_frame5[i]->setGeometry(0,0+i*54,160,50);
        btn_frame5[i]->setFocusPolicy(Qt::NoFocus);
        btn_frame5[i]->setStyleSheet("background-color:rgb(248,248,248)");
        connect(btn_frame5[i],SIGNAL(clicked(bool)),this,SLOT(slot_btn_frame5_clicked()));
        btn_frame5[i]->show();
    }
}

void KUKA::func_checkState()
{
    StateTimer = new QTimer();
    connect(StateTimer,SIGNAL(timeout()),this,SLOT(func_updateState()));
    StateTimer->start(100);
}

/*
** 函数功能：列出KUKA工作目录下所有的文件夹
** 函数参数：path，路径
** 函数返回值：无
** 函数说明：此函数递归调用，列出所有的文件夹
*/
void KUKA::func_iteratorAllDir(QString path)
{
    QDir dir(path);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0 ; i != folder_list.size() ; i++)
    {
        dirInfoList.append(folder_list.at(i));
        QString childdirpath = folder_list.at(i).absoluteFilePath();
        func_iteratorAllDir(childdirpath);
    }
}

/*初始化近期任务窗口*/
void KUKA::init_taskWindow()
{
    frame_task = new QFrame(frame_home);
    frame_task->setGeometry(0,600,494,94);
    frame_task->setObjectName("ftask");
    frame_task->setStyleSheet("#ftask{border-top:8px solid rgb(254,138,59)}");
    frame_task->raise();
    frame_task->show();

    my_task_manager = (task_fifo*)malloc(sizeof(task_fifo));
    my_task_manager->fun_name[0] = (char *)malloc(300);
    memset(my_task_manager, 0, sizeof(task_fifo));
    fifo_init(my_task_manager,6);
    Num_Of_Task=0;
    temp_Num_of_Task=0;
    connect(this,SIGNAL(Num_Of_Task_Changed()),this,SLOT(slot_tasknum_changed()));
}


/*将打开的任务添加进FIFO，如果发现FIFO里已经有此任务，则不填加*/
int KUKA::find_has_existed(void (KUKA::*funp)())
{
    int i=0;
    flag_task_exist = 0;
    for(i ; i<6 ; i++)
    {
        if(funp == my_task_manager->fun_task[i])
            flag_task_exist = 1;
    }
    return flag_task_exist;
}


/*
** 函数功能：初始化箭头标签
** 函数参数：无
** 函数返回值：无
** 函数说明：此函数的作用只有一个，就是为了初始化按钮上的箭头标签，共有六栏，也就是说最多有5列标签
*/
void KUKA::init_arrow()
{
    QPixmap pixmap;
    pixmap.load(":/arrow.PNG");
    for(int i=0;i<7;i++)
    {
        lab_arrow0[i]=new QLabel(this);
        lab_arrow0[i]->hide();
        lab_arrow0[i]->resize(20,15);
        lab_arrow0[i]->setStyleSheet("background: transparent");
        lab_arrow0[i]->setPixmap(pixmap);
    }
    for(int i=0;i<9;i++)
    {
        lab_arrow1[i]=new QLabel(this);
        lab_arrow1[i]->hide();
        lab_arrow1[i]->resize(20,15);
        lab_arrow1[i]->setStyleSheet("background: transparent");
        lab_arrow1[i]->setPixmap(pixmap);
    }
    for(int i=0;i<7;i++)
    {
        lab_arrow2[i]=new QLabel(this);
        lab_arrow2[i]->hide();
        lab_arrow2[i]->resize(20,15);
        lab_arrow2[i]->setStyleSheet("background: transparent");
        lab_arrow2[i]->setPixmap(pixmap);
    }
    for(int i=0;i<7;i++)
    {
        lab_arrow3[i]=new QLabel(this);
        lab_arrow3[i]->hide();
        lab_arrow3[i]->resize(20,15);
        lab_arrow3[i]->setStyleSheet("background: transparent");
        lab_arrow3[i]->setPixmap(pixmap);
    }
    for(int i=0;i<3;i++)
    {
        lab_arrow4[i]=new QLabel(this);
        lab_arrow4[i]->hide();
        lab_arrow4[i]->resize(20,15);
        lab_arrow4[i]->setStyleSheet("background: transparent");
        lab_arrow4[i]->setPixmap(pixmap);
    }
}

void KUKA::setarrow(QPushButton* btn , QLabel *lab)
{
    lab->setParent(btn);
    lab->move(130,20);
    lab->show();
    lab->raise();
}

/*
** 函数功能：隐藏按钮上的箭头图标
** 函数参数：ID,第ID栏
** 函数返回值：无
** 函数说明：重新设置某一栏上的箭头图标时，先隐藏这栏按钮的所有图标为下一次设置箭头图标做准备
*/
void KUKA::hide_arrow(int ID)
{
    if( ID == 0)
        for(int i=0 ;i<7;i++)
            lab_arrow0[i]->hide();
    else if( ID == 1)
        for(int i=0 ;i<9;i++)
            lab_arrow1[i]->hide();
    else if( ID == 2)
        for(int i=0 ;i<7;i++)
            lab_arrow2[i]->hide();
    else if( ID == 3)
        for(int i=0 ;i<7;i++)
            lab_arrow3[i]->hide();
    else if( ID == 4)
        for(int i=0 ;i<3;i++)
            lab_arrow4[i]->hide();
}

/*获取当前时间，为报警或提示信息提供时间戳*/
QString KUKA::func_getCurTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str=time.toString("hh:mm:ss yyyy/MM/dd");
    return str;
}

QString KUKA::get_tipcontext()
{
    QStringList strList = str_of_tipcontext.split("\n");
    return strList.at(1);
}

void KUKA::slot_infolab_color_changed()
{
        lab_show_tip->setStyleSheet("background-color:rgb(230,230,230)");
        timer_Info->stop();
}


void KUKA::fifo_init(task_fifo *fifo, int size)
{
    fifo->size = size;
    fifo->putp = 0;
    for(int i=0;i<6;i++)
    {
        fifo->fun_task[i] = NULL;
    }
}


void KUKA::fifo_put(task_fifo* fifo, void (KUKA::*funp)(), char* str)
{
    fifo->fun_task[fifo->putp] = funp;
    fifo->fun_name[fifo->putp] = str;
    fifo->putp++;
    if(fifo->putp == fifo->size)
        fifo->putp = 0;
}


void KUKA::Creat_Alert_Info(int ID, QString text, QString lab_text)
{
    CREAT_INFO(INFOWidget->Num_OF_Info)
    str_of_tipcontext = text;
    lab_infotext->setText(lab_text);
    emit num_changed(ID,str_of_tipcontext);
    if(ID == 0)  //Error
        lab_show_tip->setStyleSheet("background-color:rgb(255,0,0)");
    else if(ID == 1)  //Warning
        lab_show_tip->setStyleSheet("background-color:rgb(255,249,0)");
    else if(ID == 2)  //Tip
        lab_show_tip->setStyleSheet("background-color:rgb(110,199,241)");
    timer_Info->start(1000);
}

/*XYZ 四点法确定TCP*/
void KUKA::func_showTcp4()
{
    if(TCP_4_Widget == NULL)
        TCP_4_Widget = new Build_TCP(allframe);
    TCP_4_Widget->move(53,106);
    TCP_4_Widget->frame_SelectTool();
    TCP_4_Widget->show();
    TCP_4_Widget->raise();
    connect(TCP_4_Widget->btn_tcp[5],SIGNAL(clicked(bool)),this,SLOT(slot_TCP_Save()));
    listwidget.prepend(TCP_4_Widget);

    QString text = func_getCurTime()+"\n"+tr("进行TCP 4点法示教")+"\n"+space;
    QString lab_text = tr("TCP 4点法示教");
    Creat_Alert_Info(2,text,lab_text);

    void (KUKA::*func)();
    func = & KUKA::func_showTcp4;
    char* name = "TCP 4点法";
    if( find_has_existed(func) == 0)
    {
        Num_Of_Task++;
        fifo_put(my_task_manager,func,name);
        emit Num_Of_Task_Changed();
    }
}

/*TCP直接数字输入*/
void KUKA::func_showTcpDI()
{
    if(TCP_DI_Widget == NULL)
        TCP_DI_Widget = new TCP_IN(allframe);
    TCP_DI_Widget->move(53,106);
    TCP_DI_Widget->frame_SelectTool();
    TCP_DI_Widget->show();
    TCP_DI_Widget->raise();
    listwidget.prepend(TCP_DI_Widget);

    QString text = func_getCurTime()+"\n"+tr("进行TCP数字输入示教")+"\n"+space;
    QString lab_text = tr("TCP数字输入示教");
    Creat_Alert_Info(2,text,lab_text);

    void (KUKA::*func)();
    func = & KUKA::func_showTcpDI;
    char* name = "TCP数字输入";
    if( find_has_existed(func) == 0)
    {
        Num_Of_Task++;
        fifo_put(my_task_manager,func,name);
        emit Num_Of_Task_Changed();
    }
}


void KUKA::func_showBase3()
{
    if(Base_3_Widget == NULL)
        Base_3_Widget = new Build_Base(allframe);
    Base_3_Widget->move(53,106);
    Base_3_Widget->frame_SelectBase();
    Base_3_Widget->show();
    Base_3_Widget->raise();
    listwidget.prepend(Base_3_Widget);
    QString text = func_getCurTime()+"\n"+tr("进行基坐标系三点法示教")+"\n"+space;
    QString lab_text = tr("基坐标系三点法示教");
    Creat_Alert_Info(2,text,lab_text);

    void (KUKA::*func)();
    func = & KUKA::func_showBase3;
    char* name = "Base 3点法";
    if( find_has_existed(func) == 0)
    {
        Num_Of_Task++;
        fifo_put(my_task_manager,func,name);
        emit Num_Of_Task_Changed();
    }
}

void KUKA::func_showBaseDI()
{
    if(Base_DI_Widget == NULL)
        Base_DI_Widget = new Base_IN(allframe);
    Base_DI_Widget->move(53,106);
    Base_DI_Widget->frame_SelectBase();
    Base_DI_Widget->show();
    Base_DI_Widget->raise();
//    connect(Base_DI_Widget->btn_tcp[5],SIGNAL(clicked(bool)),this,SLOT(slot_TCP_DI_Save()));
    listwidget.prepend(Base_DI_Widget);

    QString text = func_getCurTime()+"\n"+tr("进行基坐标系数字输入示教")+"\n"+space;
    QString lab_text = tr("基坐标系数字输入示教");
    Creat_Alert_Info(2,text,lab_text);

    void (KUKA::*func)();
    func = & KUKA::func_showBaseDI;
    char* name = "Base数字输入";
    if( find_has_existed(func) == 0)
    {
        Num_Of_Task++;
        fifo_put(my_task_manager,func,name);
        emit Num_Of_Task_Changed();
    }
}

/*
** 函数功能：调用主窗口界面
** 函数参数：无
** 函数返回值：无
** 函数说明：此函数是构建窗口的主体，显示frame_home
*/
void KUKA::func_showMainWindow()
{
    if(!listwidget.isEmpty())
    {
        listwidget.first()->hide();
        listwidget.removeFirst();
    }
    if(frame_home->isHidden())
    {
        frame_home->show();
        frame_home->raise();
        btn_close->show();
    }
    else
    {
        frame_home->hide();
        btn_close->hide();
    }
}

/*
** 函数功能：显示主窗口中的主菜单
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void KUKA::func_showHome()
{
    for(int i = 0 ; i < 6 ; i++)
    {
        tip_lab[i]->hide();
        childframe[i]->hide();
    }
    tip_lab[0]->show();   
    childframe[0]->show();
    
    mainscroll->ensureWidgetVisible(tip_lab[0]);
}


void KUKA::slot_btn_back()
{
    if(!childframe[5]->isHidden())
    {
        childframe[5]->hide();
        tip_lab[5]->hide();
        mainscroll->ensureWidgetVisible(tip_lab[4]);
    }
    else if(!childframe[4]->isHidden())
    {
        childframe[4]->hide();
        tip_lab[4]->hide();
        tip_lab[5]->hide();
        mainscroll->ensureWidgetVisible(tip_lab[3]);
    }
    else if(!childframe[3]->isHidden())
    {
        childframe[3]->hide();
        tip_lab[3]->hide();
        tip_lab[4]->hide();
        tip_lab[5]->hide();
        mainscroll->ensureWidgetVisible(tip_lab[2]);
    }
    else if(!childframe[2]->isHidden())
    {
        childframe[2]->hide();
        tip_lab[2]->hide();
        tip_lab[3]->hide();
        tip_lab[4]->hide();
        tip_lab[5]->hide();
        mainscroll->ensureWidgetVisible(tip_lab[1]);
    }
    else if(!childframe[1]->isHidden())
    {
        childframe[1]->hide();
        tip_lab[1]->hide();
        tip_lab[2]->hide();
        tip_lab[3]->hide();
        tip_lab[4]->hide();
        tip_lab[5]->hide();
        mainscroll->ensureWidgetVisible(tip_lab[0]);
    }
}

/*
** 函数功能：第一栏某个按钮按下，展开第二栏的内容
** 函数参数：无
** 函数返回值：无
** 函数说明：第一栏只需要判断是第几个按钮按下即可，其后几栏不仅需要判断是第几个按钮，还要判断按钮上的内容
*/
void KUKA::slot_btn_frame0_clicked()
{
    mainframe->resize(478,518);
    for(int i = 2 ; i< 6 ;i++)
    {
        childframe[i]->hide();
        tip_lab[i]->hide();
    }
    childframe[1]->show();
    tip_lab[1]->show();
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    for(int i=0;i<7;i++)
        btn_frame0[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<9;i++)
        btn_frame1[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<7;i++)
        btn_frame2[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<7;i++)
        btn_frame3[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<3;i++)
        btn_frame4[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<2;i++)
        btn_frame5[i]->setStyleSheet(old_stysheet);
    btn->setStyleSheet(new_stylesheet);
    if(btn == btn_frame0[0])
    {
        QString str[4] = {"打印","存档","还原","项目管理"};
        tip_lab[1]->setText(tr("文件"));
        hide_arrow(1);
        for(int i=0;i<4;i++)
        {
            btn_frame1[i]->show();
            btn_frame1[i]->setText(str[i]);
            if(i !=3 )
                setarrow(btn_frame1[i],lab_arrow1[i]);
        }
        for(int i=4;i<9;i++)
            btn_frame1[i]->hide();
    }
    else if(btn == btn_frame0[1])
    {
        QString str[7] = {"输入/输出端","SUBMIT解释器","状态键","用户组","其他","安全配置","外部轴"};
        tip_lab[1]->setText(tr("配置"));
        hide_arrow(1);
        for(int i=0;i<7;i++)
        {
            btn_frame1[i]->show();
            btn_frame1[i]->setText(str[i]);
        }
        for(int i=7;i<9;i++)
            btn_frame1[i]->hide();
        for(int i=0;i<5;i++)
            if(i != 3)
                setarrow(btn_frame1[i],lab_arrow1[i]);
    }
    else if(btn == btn_frame0[2])
    {
        QString str[4] = {"输入/输出端","实际位置","变量","窗口"};
        tip_lab[1]->setText(tr("显示"));
        hide_arrow(1);
        for(int i=0;i<4;i++)
        {
            btn_frame1[i]->show();
            btn_frame1[i]->setText(str[i]);
            if(i != 1)
                setarrow(btn_frame1[i],lab_arrow1[i]);
        }
        for(int i=4;i<9;i++)
            btn_frame1[i]->hide();
    }
    else if(btn == btn_frame0[3])
    {
        QString str[6] = {"跟踪","运行日志","调用程序栈","中断","诊断显示器","KrcDiag"};
        tip_lab[1]->setText(tr("诊断"));
        hide_arrow(1);
        for(int i=0;i<6;i++)
        {
            btn_frame1[i]->show();
            btn_frame1[i]->setText(str[i]);
            if(i < 2)
                setarrow(btn_frame1[i],lab_arrow1[i]);
        }
        for(int i=6;i<9;i++)
            btn_frame1[i]->hide();
    }
    else if(btn == btn_frame0[4])
    {
        QString str[9] = {"投入运行助手","测量","调整","软件更新","售后服务","机器人数据","网络配置","安装附加软件","复制机器数据"};
        tip_lab[1]->setText(tr("投入运行"));
        hide_arrow(1);
        for(int i=0;i<9;i++)
        {
            btn_frame1[i]->show();
            btn_frame1[i]->setText(str[i]);
            connect(btn_frame1[i],SIGNAL(clicked(bool)),this,SLOT(slot_btn_frame1_clicked()));
            if( i>0 && i<5 )
                setarrow(btn_frame1[i],lab_arrow1[i]);
        }
    }
    else if(btn == btn_frame0[5])
    {
        childframe[1]->hide();
        tip_lab[1]->hide();
        func_showPower();
    }
    else if(btn == btn_frame0[6])
    {
        QString str[3] = {"信息","信息提示","文献"};
        tip_lab[1]->setText(tr("帮助"));
        hide_arrow(1);
        for(int i=0;i<3;i++)
        {
            btn_frame1[i]->show();
            btn_frame1[i]->setText(str[i]);
            if( i != 0)
                setarrow(btn_frame1[i],lab_arrow1[i]);
        }
        for(int i=3;i<9;i++)
            btn_frame1[i]->hide();
    }
}

/*
** 函数功能：第二栏某个按钮按下，展开第三栏的内容
** 函数参数：无
** 函数返回值：无
** 函数说明：第二栏不仅需要判断是第几个按钮，还要判断按钮上的内容以改变第三栏的内容
*/
void KUKA::slot_btn_frame1_clicked()
{
    mainframe->resize(478,518);
    for(int i = 3 ; i< 6 ;i++)
    {
        childframe[i]->hide();
        tip_lab[i]->hide();
    }
    childframe[2]->show();
    tip_lab[2]->show();
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    for(int i=0;i<9;i++)
        btn_frame1[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<7;i++)
        btn_frame2[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<7;i++)
        btn_frame3[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<3;i++)
        btn_frame4[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<2;i++)
        btn_frame5[i]->setStyleSheet(old_stysheet);
    btn->setStyleSheet(new_stylesheet);
    if(btn == btn_frame1[0])
    {
        if(btn_frame1[0]->text() == tr("打印"))
        {
            tip_lab[2]->setText(tr("打印"));
            hide_arrow(2);
            btn_frame2[0]->show();
            btn_frame2[0]->setText(tr("运行日志"));
            for(int i=1;i<7;i++)
                btn_frame2[i]->hide();
        }
        else if(btn_frame1[0]->text() == tr("输入/输出端")&&btn_frame1[1]->text()==tr("SUBMIT解释器"))
        {
            tip_lab[2]->setText(tr("输入/输出端"));
            hide_arrow(2);
            for(int i=0;i<2;i++)
                btn_frame2[i]->show();
            for(int i=2;i<7;i++)
                btn_frame2[i]->hide();
            btn_frame2[0]->setText(tr("外部自动运行"));
            btn_frame2[1]->setText(tr("输入/输出端驱动程序"));
        }
        else if(btn_frame1[0]->text() == tr("输入/输出端")&&btn_frame1[1]->text()==tr("实际位置"))
        {
            QString str[4] = {"数字输入/输出端","模拟输入/输出端","外部自动运行","输入/输出端驱动程序"};
            tip_lab[2]->setText(tr("输入/输出端"));
            hide_arrow(2);
            for(int i=0;i<4;i++)
            {
                btn_frame2[i]->show();
                btn_frame2[i]->setText(str[i]);
            }
            for(int i=4;i<7;i++)
                btn_frame2[i]->hide();
        }
        else if(btn_frame1[0]->text() == tr("跟踪"))
        {
            tip_lab[2]->setText(tr("跟踪"));
            hide_arrow(2);
            for(int i=0;i<2;i++)
                btn_frame2[i]->show();
            for(int i=2;i<7;i++)
                btn_frame2[i]->hide();
            btn_frame2[0]->setText(tr("配置"));
            btn_frame2[1]->setText(tr("示波器"));
        }
        else if(btn_frame1[0]->text() == tr("信息"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
            func_showSysVersion();
        }
        else
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
    }
    else if(btn == btn_frame1[1])
    {
        if(btn_frame1[1]->text() == tr("存档"))
        {
            QString str[4] = {"USB(KCP)","USB(控制柜)","网络","运行日志"};
            tip_lab[2]->setText(tr("存档"));
            hide_arrow(2);
            for(int i=0;i<4;i++)
            {
                btn_frame2[i]->show();
                btn_frame2[i]->setText(str[i]);
                if( i != 3 )
                    setarrow(btn_frame2[i],lab_arrow2[i]);
            }
            for(int i=4;i<7;i++)
                btn_frame2[i]->hide();
        }
        else if(btn_frame1[1]->text() == tr("SUBMIT解释器"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else if(btn_frame1[1]->text() == tr("实际位置"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
            func_showPos();
        }
        else if(btn_frame1[1]->text() == tr("运行日志"))
        {
            tip_lab[2]->setText(tr("运行日志"));
            hide_arrow(2);
            for(int i=0;i<2;i++)
                btn_frame2[i]->show();
            for(int i=2;i<7;i++)
                btn_frame2[i]->hide();
            btn_frame2[0]->setText(tr("显示"));
            btn_frame2[1]->setText(tr("配置"));
        }
        else if(btn_frame1[1]->text() == tr("测量"))
        {
            QString str[7] = {"工具","基坐标","固定工具","附加负载数据","外部运动装置","测量点","允差"};
            tip_lab[2]->setText(tr("测量"));
            hide_arrow(2);
            for(int i=0;i<7;i++)
            {
                btn_frame2[i]->show();
                btn_frame2[i]->setText(str[i]);
                if( i == 3 || i == 6)
                    continue;
                setarrow(btn_frame2[i],lab_arrow2[i]);
            }
        }
        else if(btn_frame1[1]->text() == tr("信息提示"))
        {
            tip_lab[2]->setText(tr("信息提示"));
            hide_arrow(2);
            btn_frame2[0]->show();
            btn_frame2[0]->setText(tr("系统软件"));
            for(int i=1;i<7;i++)
                btn_frame2[i]->hide();
        }
        else
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
    }
    else if(btn == btn_frame1[2])
    {
        if(btn_frame1[2]->text() == tr("还原"))
        {
            QString str[3] = {"USB(KCP)","USB(控制柜)","网络"};
            tip_lab[2]->setText(tr("还原"));
            hide_arrow(2);
            for(int i=0;i<3;i++)
            {
                btn_frame2[i]->show();
                btn_frame2[i]->setText(str[i]);
                setarrow(btn_frame2[i],lab_arrow2[i]);
            }
            for(int i=3;i<7;i++)
                btn_frame2[i]->hide();
        }
        else if(btn_frame1[2]->text() == tr("状态键"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else if(btn_frame1[2]->text() == tr("变量"))
        {
            QString str[6] = {"单个","概览","周期性旗标","标识器","计数器","计时器"};
            tip_lab[2]->setText(tr("变量"));
            hide_arrow(2);
            for(int i=0;i<6;i++)
            {
                btn_frame2[i]->show();
                btn_frame2[i]->setText(str[i]);
                if( i == 1)
                    setarrow(btn_frame2[i],lab_arrow2[i]);
            }
            btn_frame2[6]->hide();
        }
        else if(btn_frame1[2]->text() == tr("调用程序栈"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else if(btn_frame1[2]->text() == tr("调整"))
        {
            QString str[4] = {"千分表","EMD","参考","去调节"};
            tip_lab[2]->setText(tr("调整"));
            hide_arrow(2);
            for(int i=0;i<4;i++)
            {
                btn_frame2[i]->show();
                btn_frame2[i]->setText(str[i]);
                if( i == 1)
                    setarrow(btn_frame2[i],lab_arrow2[i]);
            }
            for(int i=4;i<7;i++)
                btn_frame2[i]->hide();
        }
        else if(btn_frame1[2]->text() == tr("文献"))
        {
            tip_lab[2]->setText(tr("文献"));
            hide_arrow(2);
            for(int i=0;i<1;i++)
            {
                btn_frame2[i]->show();
            }
            for(int i=1;i<7;i++)
            {
                btn_frame2[i]->hide();
            }
            btn_frame2[0]->setText(tr("系统软件"));
        }
        else
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
    }
    else if(btn == btn_frame1[3])
    {
        if(btn_frame1[3]->text() == tr("项目管理"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else if(btn_frame1[3]->text() == tr("用户组"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
            func_showUser();
        }
        else if(btn_frame1[3]->text() == tr("窗口"))
        {
            QString str[3] = {"资源管理器","程序","EDITOR"};
            tip_lab[2]->setText(tr("窗口"));
            hide_arrow(2);
            for(int i=0;i<3;i++)
            {
                btn_frame2[i]->setText(str[i]);
                btn_frame2[i]->show();
            }
            for(int i=3;i<7;i++)
                btn_frame2[i]->hide();
        }
        else if(btn_frame1[3]->text() == tr("中断"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else if(btn_frame1[3]->text() == tr("软件更新"))
        {
            QString str[2] = {"自动","电量"};
            tip_lab[2]->setText(tr("软件更新"));
            hide_arrow(2);
            for(int i=0;i<2;i++)
            {
                btn_frame2[i]->show();
                btn_frame2[i]->setText(str[i]);
            }
            for(int i=2;i<7;i++)
                btn_frame2[i]->hide();
        }
        else
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
    }
    else if(btn == btn_frame1[4])
    {
        if(btn_frame1[4]->text() == tr("其他"))
        {
            QString str[5] = {"语言","工作区域监控","操作计划器","点坐标的修正极限","碰撞识别"};
            tip_lab[2]->setText(tr("其他"));
            hide_arrow(2);
            for(int i=0;i<5;i++)
            {
                btn_frame2[i]->show();
                btn_frame2[i]->setText(str[i]);
                if( i == 1)
                    setarrow(btn_frame2[i],lab_arrow2[i]);
            }
            for(int i=5;i<7;i++)
                btn_frame2[i]->hide();
        }
        else if(btn_frame1[4]->text() == tr("诊断显示器"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else if(btn_frame1[4]->text() == tr("售后服务"))
        {
            QString str[5] = {"软件限位开关","长文本","保养手册","投入运行模式","HMI 最小化"};
            tip_lab[2]->setText(tr("售后服务"));
            hide_arrow(2);
            for(int i=0;i<5;i++)
            {
                btn_frame2[i]->show();
                btn_frame2[i]->setText(str[i]);
            }
            for(int i=5;i<7;i++)
                btn_frame2[i]->hide();
        }
        else
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
    }
    else if(btn == btn_frame1[5])
    {
        if(btn_frame1[5]->text() == tr("安全配置"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else if(btn_frame1[5]->text() == tr("KrcDiag"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else if(btn_frame1[5]->text() == tr("机器人数据"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
    }
    else if(btn == btn_frame1[6])
    {
        if(btn_frame1[6]->text() == tr("外部轴"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else if(btn_frame1[6]->text() == tr("网络配置"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
    }
    else if(btn == btn_frame1[7])
    {
        if(btn_frame1[7]->text() == tr("安装附加软件"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
    }
    else if(btn == btn_frame1[8])
    {
        if(btn_frame1[8]->text() == tr("复制机器数据"))
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
        else
        {
            childframe[2]->hide();
            tip_lab[2]->hide();
        }
    }
}

/*
** 函数功能：第三栏某个按钮按下，展开第四栏的内容
** 函数参数：无
** 函数返回值：无
** 函数说明：第三栏不仅需要判断是第几个按钮，还要判断按钮上的内容以改变第四栏的内容
*/
void KUKA::slot_btn_frame2_clicked()
{
    mainframe->resize(652,518);
    for(int i = 4 ; i< 6 ;i++)
    {
        childframe[i]->hide();
        tip_lab[i]->hide();
    }
    tip_lab[3]->show();
    childframe[3]->show();
    mainscroll->ensureWidgetVisible(tip_lab[3]);
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    for(int i=0;i<7;i++)
        btn_frame2[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<7;i++)
        btn_frame3[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<3;i++)
        btn_frame4[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<2;i++)
        btn_frame5[i]->setStyleSheet(old_stysheet);
    btn->setStyleSheet(new_stylesheet);
    if(btn == btn_frame2[0])
    {
        if(btn_frame2[0]->text()==tr("USB(KCP)") && (!btn_frame2[3]->isHidden()))
        {
            QString str[5] = {"所有","应用","系统数据","LOG数据","KrcDiag"};
            tip_lab[3]->setText(tr("USB(KCP)"));
            hide_arrow(3);
            for(int i=0;i<5;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
            for(int i=5;i<7;i++)
                btn_frame3[i]->hide();
        }
        else if(btn_frame2[0]->text()==tr("USB(KCP)") && (btn_frame2[3]->isHidden()))
        {
            QString str[3] = {"所有","应用","系统数据"};
            tip_lab[3]->setText(tr("USB(KCP)"));
            hide_arrow(3);
            for(int i=0;i<3;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
            for(int i=3;i<7;i++)
                btn_frame3[i]->hide();
        }
        else if(btn_frame2[0]->text()==tr("数字输入/输出端"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
            func_showDIO();
        }
        else if(btn_frame2[0]->text()==tr("单个"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
            func_showVar();
        }
        else if(btn_frame2[0]->text()==tr("资源管理器"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
            func_showFileManger();
        }
        else if(btn_frame2[0]->text()==tr("工具"))
        {
            QString str[7] = {"XYZ 4点法","XYZ 参照法","ABC 2点法","ABC 世界坐标系","数字输入","更改名称","工具负荷数据"};
            tip_lab[3]->setText(tr("工具"));
            hide_arrow(3);
            for(int i=0;i<7;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
        }
        else if(btn_frame2[0]->text()==tr("千分表"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[0]->text() == "自动")
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
            func_showUpdate();
        }
        else
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
    }
    else if(btn == btn_frame2[1])
    {
        if(btn_frame2[1]->text()==tr("USB(控制柜)") && (!btn_frame2[3]->isHidden()))
        {
            QString str[5] = {"所有","应用","系统数据","LOG数据","KrcDiag"};
            tip_lab[3]->setText(tr("USB(控制柜)"));
            hide_arrow(3);
            for(int i=0;i<5;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
            for(int i=5;i<7;i++)
                btn_frame3[i]->hide();
        }
        else if(btn_frame2[1]->text()==tr("USB(控制柜)") && (btn_frame2[3]->isHidden()))
        {
            QString str[3] = {"所有","应用","系统数据"};
            tip_lab[3]->setText(tr("USB(控制柜)"));
            hide_arrow(3);
            for(int i=0;i<3;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
            for(int i=3;i<7;i++)
                btn_frame3[i]->hide();
        }
        else if(btn_frame2[1]->text()==tr("概览"))
        {
            QString str[3] = {"显示","配置","编辑\"ConfigMon.ini\" "};
            tip_lab[3]->setText(tr("概览"));
            hide_arrow(3);
            for(int i=0;i<3;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
            for(int i=3;i<7;i++)
                btn_frame3[i]->hide();
        }
        else if(btn_frame2[1]->text()==tr("程序"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[1]->text()==tr("基坐标"))
        {
            QString str[4] = {"3点","间接","数字输入","更改名称"};
            tip_lab[3]->setText(tr("基坐标"));
            hide_arrow(3);
            for(int i=0;i<4;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
            for(int i=4;i<7;i++)
                btn_frame3[i]->hide();
        }
        else if(btn_frame2[1]->text()==tr("EMD"))
        {
            QString str[2] = {"标准","带负载校正"};
            tip_lab[3]->setText(tr("EMD"));
            hide_arrow(3);
            for(int i=0;i<2;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
                setarrow(btn_frame3[i],lab_arrow3[i]);
            }
            for(int i=2;i<7;i++)
                btn_frame3[i]->hide();
        }
        else if(btn_frame2[1]->text()==tr("长文本"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[1]->text() == tr("模拟输入/输出端"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
            func_showAIO();
        }
        else
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
    }
    else if(btn == btn_frame2[2])
    {
        if(btn_frame2[2]->text()==tr("网络") && (!btn_frame2[3]->isHidden()))
        {
            QString str[5] = {"所有","应用","系统数据","LOG数据","KrcDiag"};
            tip_lab[3]->setText(tr("网络"));
            hide_arrow(3);
            for(int i=0;i<5;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
            for(int i=5;i<7;i++)
                btn_frame3[i]->hide();
        }
        else if(btn_frame2[2]->text()==tr("网络") && (btn_frame2[3]->isHidden()))
        {
            QString str[3] = {"所有","应用","系统数据"};
            tip_lab[3]->setText(tr("网络"));
            hide_arrow(3);
            for(int i=0;i<3;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
            for(int i=3;i<7;i++)
                btn_frame3[i]->hide();
        }
        else if(btn_frame2[2]->text()==tr("操作计划器"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[2]->text()==tr("外部自动运行"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[2]->text()==tr("周期性旗标"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[2]->text()==tr("EDITOR"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[2]->text()==tr("固定工具"))
        {
            QString str[4] = {"工件","工具","外部运动系统偏量","数字输入"};
            tip_lab[3]->setText(tr("固定工具"));
            hide_arrow(3);
            for(int i=0;i<4;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
            setarrow(btn_frame3[0],lab_arrow3[0]);
            for(int i=4;i<7;i++)
                btn_frame3[i]->hide();
        }
        else if(btn_frame2[2]->text()==tr("参考"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[2]->text()==tr("保养手册"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
    }
    else if(btn == btn_frame2[3])
    {
        if(btn_frame2[3]->text()==tr("运行日志"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[3]->text()==tr("点坐标的修正极限"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[3]->text()==tr("输入/输出端驱动程序"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[3]->text()==tr("输入/标识器"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[3]->text()==tr("附加负载数据"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[3]->text()==tr("去调节"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[3]->text()==tr("投入运行模式"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
    }
    else if(btn == btn_frame2[4])
    {
        if(btn_frame2[4]->text()==tr("碰撞识别"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[4]->text()==tr("计数器"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[4]->text()==tr("外部运动装置"))
        {
            QString str[6] = {"基点","基点(数值)","偏量","偏量(数值)","线性滑轨","线性滑轨(数字)"};
            tip_lab[3]->setText(tr("外部运动装置"));
            hide_arrow(3);
            for(int i=0;i<6;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
            for(int i=6;i<7;i++)
                btn_frame3[i]->hide();
        }
        else if(btn_frame2[4]->text()==tr("HMI最小化"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
    }
    else if(btn == btn_frame2[5])
    {
        if(btn_frame2[5]->text()==tr("计时器"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else if(btn_frame2[5]->text()==tr("测量点"))
        {
            QString str[3] = {"工具类型","基座型号","外部轴"};
            tip_lab[3]->setText(tr("测量点"));
            hide_arrow(3);
            for(int i=0;i<3;i++)
            {
                btn_frame3[i]->show();
                btn_frame3[i]->setText(str[i]);
            }
            for(int i=3;i<7;i++)
                btn_frame3[i]->hide();
        }
        else
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
    }
    else if(btn == btn_frame2[6])
    {
        if(btn_frame2[6]->text()==tr("允差"))
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
        else
        {
            childframe[3]->hide();
            tip_lab[3]->hide();
        }
    }
}

/*
** 函数功能：第四栏某个按钮按下，展开第吴栏的内容
** 函数参数：无
** 函数返回值：无
** 函数说明：第四栏不仅需要判断是第几个按钮，还要判断按钮上的内容以改变第五栏的内容
*/
void KUKA::slot_btn_frame3_clicked()
{
    mainframe->resize(816,518);
    childframe[5]->hide();
    tip_lab[5]->hide();
    tip_lab[4]->show();
    mainscroll->ensureWidgetVisible(tip_lab[4]);
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    for(int i=0;i<7;i++)
        btn_frame3[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<3;i++)
        btn_frame4[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<2;i++)
        btn_frame5[i]->setStyleSheet(old_stysheet);
    btn->setStyleSheet(new_stylesheet);
    if(btn == btn_frame3[0])
    {
        if(btn_frame3[0]->text()==tr("工件"))
        {
            tip_lab[4]->setText(tr("工件"));
            childframe[4]->show();
            hide_arrow(4);
            for(int i=0;i<2;i++)
                btn_frame4[i]->show();
            for(int i=2;i<3;i++)
                btn_frame4[i]->hide();
            btn_frame4[0]->setText(tr("直接测量"));
            btn_frame4[1]->setText(tr("间接测量"));
        }
        else if(btn_frame3[0]->text()==tr("标准"))
        {
            tip_lab[4]->setText(tr("标准"));
            childframe[4]->show();
            hide_arrow(4);
            for(int i=0;i<2;i++)
                btn_frame4[i]->show();
            for(int i=2;i<3;i++)
                btn_frame4[i]->hide();
            btn_frame4[0]->setText(tr("执行零点校正"));
            btn_frame4[1]->setText(tr("检查零点校正"));
        }
        else if(btn_frame3[0]->text()==tr("XYZ 4点法"))
        {
            func_showTcp4();
        }
        else if(btn_frame3[0]->text()==tr("3点"))
        {
            func_showBase3();
        }
        else
        {
            childframe[4]->hide();
            tip_lab[4]->hide();
        }
    }
    else if(btn == btn_frame3[1])
    {
        if(btn_frame3[1]->text()==tr("带负载校正"))
        {
            QString str[3] = {"首次调整","偏量学习","负载校正"};
            tip_lab[4]->setText(tr("带负载校正"));
            childframe[4]->show();
            hide_arrow(4);
            for(int i=0;i<3;i++)
            {
                btn_frame4[i]->show();
                btn_frame4[i]->setText(str[i]);
            }
            setarrow(btn_frame4[2],lab_arrow4[2]);
        }
        else
        {
            childframe[4]->hide();
            tip_lab[4]->hide();
        }
    }
    else if(btn == btn_frame3[2])
    {
        if(btn->text() == tr("数字输入"))
            func_showBaseDI();
        else
        {
            childframe[4]->hide();
            tip_lab[4]->hide();
        }
    }
    else if(btn == btn_frame3[3])
    {
            childframe[4]->hide();
            tip_lab[4]->hide();
    }
    else if(btn == btn_frame3[4])
    {
        if(btn->text() == tr("数字输入"))
            func_showTcpDI();
        else
        {
            childframe[4]->hide();
            tip_lab[4]->hide();
        }
    }
    else if(btn == btn_frame3[5])
    {
        childframe[4]->hide();
        tip_lab[4]->hide();
    }
    else if(btn == btn_frame3[6])
    {
        childframe[4]->hide();
        tip_lab[4]->hide();
    }
}

/*
** 函数功能：第五栏某个按钮按下，展开第六栏的内容
** 函数参数：无
** 函数返回值：无
** 函数说明：第五栏不仅需要判断是第几个按钮，还要判断按钮上的内容以改变第六栏的内容
*/
void KUKA::slot_btn_frame4_clicked()
{
    mainframe->resize(980,518);
    tip_lab[5]->show();
    mainscroll->ensureWidgetVisible(tip_lab[5]);
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    for(int i=0;i<3;i++)
        btn_frame4[i]->setStyleSheet(old_stysheet);
    for(int i=0;i<2;i++)
        btn_frame5[i]->setStyleSheet(old_stysheet);
    btn->setStyleSheet(new_stylesheet);
    if(btn == btn_frame4[0])
    {
        if(btn->text() == tr("执行零点校正"))
        {

        }
    }
    else if(btn == btn_frame4[1])
    {

    }
    else if(btn == btn_frame4[2])
    {
        tip_lab[5]->setText(tr("负载校正"));
        childframe[5]->show();
        for(int i=0;i<2;i++)
            btn_frame5[i]->show();
        btn_frame5[0]->setText(tr("带偏量"));
        btn_frame5[1]->setText(tr("无偏量"));
    }
}

/*
** 函数功能：第六栏某个按钮按下
** 函数参数：无
** 函数返回值：无
*/
void KUKA::slot_btn_frame5_clicked()
{
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    for(int i=0;i<2;i++)
        btn_frame5[i]->setStyleSheet(old_stysheet);
    btn->setStyleSheet(new_stylesheet);
}

/*
** 函数功能：以秒为单位，实时更新当前时间
** 函数参数：无
** 函数返回值：无
*/
void KUKA::slot_timerout()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("hh:mm:ss yyyy/MM/dd"); //设置显示格式
    lab_time->setText(str);//在标签上显示时间
}

void KUKA::func_updateState()
{
    if(IO_Get(ioDIc, NCK_DI_EMG))       KukaRob.EMG = 1;
    else                                KukaRob.EMG = 0;
    if(IO_Get(ioDIc, NCK_DI_DEAD))      KukaRob.Motor = 1;
    else                                KukaRob.Motor = 0;
    MenuBarWidget->func_UpdateRobState();
    POSWidget->func_updateJogPos();
    POSWidget->func_updateDescartesPos();
}

/*TCP4点示教法保存*/
void KUKA::slot_TCP_Save()
{
    if(TCP_4_Widget->btn_tcp[5]->text() == tr("保存"))   //TCP4点示教法到了最后一页，继续按钮变成保存按钮
    {
        QString text /*= func_getCurTime()+"\n"+tr("数据已被成功保存(")+name_tcp_tool+tr(")")+"\n"+space*/;
        QString lab_text /*= (tr("数据已被成功保存(")+name_tcp_tool+tr(")"))*/;
        Creat_Alert_Info(2,text,lab_text);
    }
}

/*TCP数字输入法保存*/
void KUKA::slot_TCP_DI_Save()
{
    QString text /*= func_getCurTime()+"\n"+tr("数据已被成功保存(")+name_tcp_tool+tr(")")+"\n"+space*/;
    QString lab_text/* = (tr("数据已被成功保存(")+name_tcp_tool+tr(")"))*/;
    Creat_Alert_Info(2,text,lab_text);
}


void KUKA::func_showFileManger()
{
    if(btn_frame2[0]->text() == tr("资源管理器"))
    {
        listwidget.prepend(File_Manager);
        File_Manager->show();
        File_Manager->raise();
        CREAT_INFO(INFOWidget->Num_OF_Info);
        str_of_tipcontext = func_getCurTime()+"\n"+"File_Manager"+"\n"+space;
        lab_infotext->setText("File_Manager");
//        emit INFOWidget->num_changed(1,str_of_tipcontext);
        emit num_changed(1,str_of_tipcontext);
        lab_show_tip->setStyleSheet("background-color:rgb(255,249,0)");
        timer_Info->start(1000);
        void (KUKA::*func)();
        func = & KUKA::func_showFileManger;
        char*  name = "资源管理器";
        if( find_has_existed(func) == 0)
        {
            Num_Of_Task++;
            fifo_put(my_task_manager,func,name);
            emit Num_Of_Task_Changed();

        }

    }
}


/*提示信息确认按钮，按下代表确认当前的信息，并且删除已经确认的信息*/
void KUKA::func_ConfirmInfo()
{
    if(INFOWidget->Num_OF_Info > 0)   //有报警信息时才继续往下执行，否则直接返回
    {
        int Info_Count = INFOWidget->model->rowCount(); //判断当前总共有几个报警信息
        if(Info_Count > 0)
            INFOWidget->current_item = (MyStandardItem *)INFOWidget->model->item(Info_Count-1,0);
        INFOWidget->model->takeRow(INFOWidget->current_item->row());   //先删除当前选择的报警信息
        CONFIRM_INFO(INFOWidget->Num_OF_Info)   //报警信息总数减一
    }
    else
    {
        lab_infotext->setText(" "); //清除报警栏报警文字
        emit No_Tip();  //发射无警告信息信号
        return;
    }

    QString str="test";
    if(INFOWidget->current_item->ID == 0)
    {
        INFOWidget->Num_OF_ERROR--;
//        emit INFOWidget->num_changed(0,str);
        emit num_changed(0,str);
    }

    if(INFOWidget->current_item->ID == 1)
    {
        INFOWidget->Num_OF_Warning--;
//        emit INFOWidget->num_changed(1,str);
        emit num_changed(1,str);
    }
}


void KUKA::slot_Cancel_All_Info()
{
    int Info_Count = INFOWidget->model->rowCount(); //判断当前总共有几个报警信息
    if(Info_Count > 0)
    {
        for(int i=Info_Count-1;i>=0 ; i--)
        {
            INFOWidget->model->takeRow(i);
        }
        lab_infotext->setText(" ");  //清除报警栏报警文字

        INFOWidget->Num_OF_Info = 0;
        INFOWidget->temp_num = 0;
        INFOWidget->Num_OF_ERROR=0;
        INFOWidget->Num_OF_Warning=0;
        INFOWidget->Num_OF_Tip=0;
        INFOWidget->Num_OF_Waiting=0;
        emit No_Tip();  //发射无警告信息信号

        slot_info_count();
    }
}
void KUKA::slot_info_count()
{
    int count[4];
    count[0] = INFOWidget->Num_OF_ERROR;
    count[1] = INFOWidget->Num_OF_Warning;
    count[2] = INFOWidget->Num_OF_Tip;
    count[3] = 0;
    MenuBarWidget->func_UpdateAlarm(count);
}

void KUKA::func_showDIO()
{
    if(btn_frame2[0]->text() == tr("数字输入/输出端"))
    {
        IOWidget->show();
        IOWidget->raise();
        listwidget.prepend(IOWidget);
        void (KUKA::*func)();
        func = & KUKA::func_showDIO;
        char*  name = "数字输入/输出端";
        if( find_has_existed(func) == 0)
        {
            Num_Of_Task++;
            fifo_put(my_task_manager,func,name);
            emit Num_Of_Task_Changed();
        }

    }
}


void KUKA::func_showAIO()
{
    AIOWidget->show();
    AIOWidget->raise();
    listwidget.prepend(AIOWidget);
    void (KUKA::*func)();
    func = & KUKA::func_showAIO;
    char*  name = "模拟输入/输出端";
    if( find_has_existed(func) == 0)
    {
        Num_Of_Task++;
        fifo_put(my_task_manager,func,name);
        emit Num_Of_Task_Changed();
    }
}


/*单项变量显示*/
void KUKA::func_showVar()
{
    if(SigVarWidget == NULL)
        SigVarWidget = new SigVar(allframe);
    SigVarWidget->move(53,106);
    SigVarWidget->init_frame();
    SigVarWidget->show();
    SigVarWidget->raise();

    listwidget.prepend(SigVarWidget);

    void (KUKA::*func)();
    func = & KUKA::func_showVar;
    char*  name = "单项变量";
    if( find_has_existed(func) == 0)
    {
        Num_Of_Task++;
        fifo_put(my_task_manager,func,name);
        emit Num_Of_Task_Changed();
    }
}


void KUKA::func_showPos()
{
    POSWidget->show();
    POSWidget->raise();
    listwidget.prepend(POSWidget);
    void (KUKA::*func)();
    func = & KUKA::func_showPos;
    char*  name = "实际位置";
    if( find_has_existed(func) == 0)
    {
        Num_Of_Task++;
        fifo_put(my_task_manager,func,name);
        emit Num_Of_Task_Changed();
    }
}


void KUKA::func_showPower()
{
    PowerWidget->show();
    PowerWidget->raise();
    listwidget.prepend(PowerWidget);
    CREAT_INFO(INFOWidget->Num_OF_Info)
    str_of_tipcontext = func_getCurTime()+"\n"+"Poweroff"+"\n"+space;
    lab_infotext->setText("Poweroff");
//    emit INFOWidget->num_changed(0,str_of_tipcontext);
    emit num_changed(0,str_of_tipcontext);
    lab_show_tip->setStyleSheet("background-color:rgb(255,0,0)");  //Error
    timer_Info->start(1000);

    void (KUKA::*func)();
    func = & KUKA::func_showPower;
    char*  name = "关机";
    if( find_has_existed(func) == 0)
    {
        Num_Of_Task++;
        fifo_put(my_task_manager,func,name);
        emit Num_Of_Task_Changed();
    }
}


/*显示用户组界面*/
void KUKA::func_showUser()
{
    if(UserWidget == NULL)
    {
        UserWidget = new UserGroup(allframe);
        connect(UserWidget,SIGNAL(sig_userlogin(int)),File_Manager,SLOT(slot_handeleUserChanged(int)));
    }
    UserWidget->move(53,106);
    UserWidget->init_frame();
    UserWidget->show();
    UserWidget->raise();
    listwidget.prepend(UserWidget);
    void (KUKA::*func)();
    func = & KUKA::func_showUser;
    char*  name = "用户组";
    if( find_has_existed(func) == 0)
    {
        Num_Of_Task++;
        fifo_put(my_task_manager,func,name);
        emit Num_Of_Task_Changed();
    }
}

void KUKA::func_showUpdate()
{
    if(UpdateWidget == NULL)
        UpdateWidget = new UpdateSys(allframe);
    UpdateWidget->move(53,106);
    UpdateWidget->show();
    UpdateWidget->raise();
    listwidget.prepend(UpdateWidget);
    UpdateWidget->frame_updateSys();
    void (KUKA::*func)();
    func = & KUKA::func_showUpdate;
    char*  name = "系统升级";
    if( find_has_existed(func) == 0)
    {
        Num_Of_Task++;
        fifo_put(my_task_manager,func,name);
        emit Num_Of_Task_Changed();
    }
}

void KUKA::func_showSysVersion()
{
    if(frame_version == NULL)
    {
        frame_version = new QFrame(this);
        frame_version->setGeometry(53,106,494,694);
        frame_version->setStyleSheet("background-color:rgb(230,230,230)");

        QLabel *lab_tip = new QLabel(frame_version);
        lab_tip->setGeometry(0,0,494,30);
        lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
        lab_tip->setText("  信息");
        lab_tip->show();

        QLabel *lab_version = new QLabel(frame_version);
        lab_version->setGeometry(200,350,200,30);
        lab_version->show();
        u16 sysv = xSys.uVer;
        QString h = "V2.8.0326";
        QString v= QString::number((int)sysv);
        QString ver = h+v;
        lab_version->setText(ver);
    }
    frame_version->show();
    frame_version->raise();
    listwidget.prepend(frame_version);
    void (KUKA::*func)();
    func = & KUKA::func_showSysVersion;
    char*  name = "帮助信息";
    if( find_has_existed(func) == 0)
    {
        Num_Of_Task++;
        fifo_put(my_task_manager,func,name);
        emit Num_Of_Task_Changed();
    }
}

void KUKA::func_HandMove(int n)
{
    for(int i=0;i<12;i++)
    {
        if(i == n)
            IO_Set(ioDIState.ioDIc,NCK_DI_XSP+n);
        else
            IO_Reset(ioDIState.ioDIc,NCK_DI_XSP+i);
    }
    ioDIState.flag=1;
    NcktCmd(&(xImdT.aImdT), NCK_JOG, 0, 100);
    xImdT.flag = 1;
}

void KUKA::slot_tasknum_changed()
{
    if(Num_Of_Task > temp_Num_of_Task && Num_Of_Task<7)
    {
        btn_task[Num_Of_Task-1] = new QPushButton(frame_task);
        btn_task[Num_Of_Task-1]->setFocusPolicy(Qt::NoFocus);
        connect(btn_task[Num_Of_Task-1],SIGNAL(clicked(bool)),this,SLOT(slot_task_inter()));
        if((Num_Of_Task-1)<3)
            btn_task[Num_Of_Task-1]->setGeometry(8+158*(Num_Of_Task-1),47,158,39);
        else
            btn_task[Num_Of_Task-1]->setGeometry(8+158*((Num_Of_Task-1)%3),8,158,39);
        btn_task[Num_Of_Task-1]->show();
        btn_task[Num_Of_Task-1]->setText(my_task_manager->fun_name[Num_Of_Task-1]);
        temp_Num_of_Task = Num_Of_Task;
    }
    if(Num_Of_Task >= 7)
    {
        connect(btn_task[Num_Of_Task%6-1],SIGNAL(clicked(bool)),this,SLOT(slot_task_inter()));
        btn_task[Num_Of_Task%6-1]->setText(my_task_manager->fun_name[Num_Of_Task%6-1]);
    }
}


void KUKA::slot_task_inter()
{
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    if(btn == btn_task[0])
        (this->*my_task_manager->fun_task[0])();
    if(btn == btn_task[1])
        (this->*my_task_manager->fun_task[1])();
    if(btn == btn_task[2])
        (this->*my_task_manager->fun_task[2])();
    if(btn == btn_task[3])
        (this->*my_task_manager->fun_task[3])();
    if(btn == btn_task[4])
        (this->*my_task_manager->fun_task[4])();
    if(btn == btn_task[5])
        (this->*my_task_manager->fun_task[5])();
}


void KUKA::slot_close()
{
    if(!listwidget.isEmpty())
    {
        listwidget.first()->hide();
        qDebug()<<listwidget.first();
        btn_close->show();
        /*让程序编辑框恢复成原来的尺寸和位置*/
        if(listwidget.first() == File_Manager->win_CodePad->cmdline->frame_TcpBase)
            File_Manager->win_CodePad->func_restoreFrame();
        if(listwidget.first() == File_Manager->win_CodePad->cmdline->frame_SpdZone)
            File_Manager->win_CodePad->func_restoreFrame();
        if(listwidget.first() == File_Manager->win_CodePad->cmdline->frame_SpdDire)
            File_Manager->win_CodePad->func_restoreFrame();
        /*关闭程序编辑界面进行保存*/
         if(listwidget.first() == File_Manager->win_CodePad)
         {
             func_SaveToFile(File_Manager->win_CodePad->codeeditor,KukaRob.CurMName);
             if(!(KukaRob.CurMName.contains(".src")))
             {
                 funcParser->startParser(KukaRob.CurMName);
             }
         }
        listwidget.removeFirst();
    }
    else
    {
        File_Manager->show();
        File_Manager->raise();
    }
}

void KUKA::keyPressEvent(QKeyEvent *e)
{
    int dire;
    qDebug()<<e->key();
    if(e->key() == Qt::Key_F1)
    {
        if(KukaRob.Cor_Key == 0)  //1+
            dire = NCK_DI_XSM;
        else
            dire = NCK_DI_XSP;  //线性X+
        func_HandMove(dire);
    }
    else if(e->key() == Qt::Key_F9)
    {
        if(KukaRob.Cor_Key == 0)  //1-
            dire = NCK_DI_XSP;
        else
            dire = NCK_DI_XSM;  //线性X-
        func_HandMove(dire);
    }
    else if(e->key() == Qt::Key_F2)   //2+
    {
        if(KukaRob.Cor_Key == 0)  //2+
            dire = NCK_DI_YLP;
        else
            dire = NCK_DI_YLP;  //线性Y+
        func_HandMove(dire);
    }
    else if(e->key() == Qt::Key_F10)   //2-
    {
        if(KukaRob.Cor_Key == 0)  //2-
            dire = NCK_DI_YLM;
        else
            dire = NCK_DI_YLM;  //线性Y-
        func_HandMove(dire);
    }
    else if(e->key() == Qt::Key_F4)   //3+
    {
        if(KukaRob.Cor_Key == 0)  //3+
            dire = NCK_DI_ZUM;
        else
            dire = NCK_DI_ZUP;  //线性Z+
        func_HandMove(dire);
    }
    else if(e->key() == Qt::Key_F11)   //3-
    {
        if(KukaRob.Cor_Key == 0)  //3-
            dire = NCK_DI_ZUP;
        else
            dire = NCK_DI_ZUM;  //线性Z-
        func_HandMove(dire);
    }
    else if(e->key() == Qt::Key_F6)  // 4+
    {
        qDebug()<<"this is F6 key";
        if(KukaRob.Cor_Key == 0)  //4+
            dire = NCK_DI_XRP;
        else
            dire = NCK_DI_ZTM;  //重定位绕Z+
        func_HandMove(dire);

    }
    else if(e->key() == Qt::Key_F12)     // 4-
    {   
        if(KukaRob.Cor_Key == 0)  //4-
            dire = NCK_DI_XRM;
        else
            dire = NCK_DI_ZTP;  //重定位绕Z-
        func_HandMove(dire);
    }
    else if(e->key() == Qt::Key_F3)     // 5+
    {
        if(KukaRob.Cor_Key == 0)  //5+
            dire = NCK_DI_YBM;
        else
            dire = NCK_DI_YBM;  //重定位绕Y+
        func_HandMove(dire);
    }
    else if(e->key() == Qt::Key_ScrollLock)       // 5-
    {
        if(KukaRob.Cor_Key == 0)  //5-
            dire = NCK_DI_YBP;
        else
            dire = NCK_DI_YBP;  //重定位绕Y-
        func_HandMove(dire);
    }
    else if(e->key() == Qt::Key_F5)   // 6+
    {
        if(KukaRob.Cor_Key == 0)  //6+
            dire = NCK_DI_ZTM;
        else
            dire = NCK_DI_XRM;  //重定位绕X+
        func_HandMove(dire);
    }
    else if(e->key() == Qt::Key_NumLock) // 6-
    {
        if(KukaRob.Cor_Key == 0)  //6-
            dire = NCK_DI_ZTP;
        else
            dire = NCK_DI_XRP;  //重定位绕X-
        func_HandMove(dire);
    }
    else if(e->key() == Qt::Key_F8)   //CODE+
    {
        qDebug()<<"key_F8";
    }
    else if(e->key() == Qt::Key_Down)  //CODE-
    {
        qDebug()<<"Key_Down";
    }
    else if(e->key() == Qt::Key_F7)   //HAND+
    {
        qDebug()<<"key_F7";
    }
    else if(e->key() == Qt::Key_VolumeDown)   //HAND-
    {
        qDebug()<<"Key_VolumeDown";
    }
    else if(e->key() == Qt::Key_F20)
    {
        func_showMainWindow();
    }
    else if(e->key() == Qt::Key_Escape){  /* stop */
        qDebug()<<"this is stop code";
    }
    else if(e->key() == Qt::Key_Left){  /* back */
        qDebug()<<"this is back code";
    }
    else if(e->key() == Qt::Key_End){  /* keypad */
        qDebug()<<"this is keypad";
    }
    else if(e->key() == Qt::Key_PageUp){  /* forward */

        xSys.iCyc = SYS_CYC_STEP;
        xSys.iRun = SYS_RUN_AUTO;
        xSys.iDecode = SYS_DECODE_YES;
        qDebug()<<"this is forward1";
    }
    else if(e->key() == Qt::Key_Return)
    {
        File_Manager->win_CodePad->func_editFileToSrcFile();
    }
    else if(e->key() == Qt::Key_Home){  /* home key */
        qDebug()<<"this is home key";
    }
    NcktCmd(&(xImdT.aImdT), NCK_JOG, 0, 100);
    xImdT.flag = 1;

    if(POSWidget!=NULL)
        POSWidget->func_updateJogPos();
}


void KUKA::keyReleaseEvent(QKeyEvent *e)
{
    qDebug()<<e->key();
    if(e->key() == Qt::Key_Escape){  /* stop */
        qDebug()<<"this is stop code";
    }
    else if(e->key() == Qt::Key_Down)  //CODE-
    {
        qDebug()<<"Key_Down";
    }
    else if(e->key() == Qt::Key_Left){  /* back */
        qDebug()<<"this is back code";
    }
    else if(e->key() == Qt::Key_PageUp){  /* forward */
//        xSys.iCyc = SYS_CYC_STEP;
//        xSys.iRun = SYS_RUN_AUTO;
//        xSys.iDecode = SYS_DECODE_YES;
        qDebug()<<"this is forward2";
    }
//    if(e->key() == Qt::Key_F1)   //1+
//    {
//        IO_Reset(ioDIc,NCK_DI_XSP);
//        qDebug()<<"key_F1_0";

//    }
//    if(e->key() == Qt::Key_F9) //1-
//    {
//        IO_Reset(ioDIc,NCK_DI_XSM);
//        qDebug()<<"key_F9_0";

//    }
//    if(e->key() == Qt::Key_F2) //2+
//    {
//        IO_Reset(ioDIc,NCK_DI_YLP);
//        qDebug()<<"key_F2_0";

//    }
//    if(e->key() == Qt::Key_F10) //2-
//    {
//        IO_Reset(ioDIc,NCK_DI_YLM);
//        qDebug()<<"key_F10_0";

//    }
//    if(e->key() == Qt::Key_F4) //3+
//    {
//        IO_Reset(ioDIc,NCK_DI_ZUP);
//        qDebug()<<"key_F4_0";

//    }
//    if(e->key() == Qt::Key_F11) //3-
//    {
//        IO_Reset(ioDIc,NCK_DI_ZUM);
//        qDebug()<<"key_F11_0";

//    }
//    if(e->key() == Qt::Key_F6) //4+
//    {
//         IO_Reset(ioDIc,NCK_DI_XRP);
//        qDebug()<<"Key_F6_0";

//    }
//    if(e->key() == Qt::Key_F12) //4-
//    {
//        IO_Reset(ioDIc,NCK_DI_XRM);
//        qDebug()<<"Key_F12_0";

//    }
//    if(e->key() == Qt::Key_F3) //5+
//    {
//        IO_Reset(ioDIc,NCK_DI_YBP);
//        qDebug()<<"Key_F3_0";

//    }
//    if(e->key() == Qt::Key_ScrollLock) //5-
//    {
//         IO_Reset(ioDIc,NCK_DI_YBM);
//        qDebug()<<"Key_ScrollLock_0";

//    }
//    if(e->key() == Qt::Key_F5) //6+
//    {
//         IO_Reset(ioDIc,NCK_DI_ZTP);
//        qDebug()<<"Key_F5_0";

//    }
//    if(e->key() == Qt::Key_NumLock)//6-
//    {
//        IO_Reset(ioDIc,NCK_DI_ZTM);
//        qDebug()<<"Key_NumLock_0";

//    }
//    if(e->key() == Qt::Key_F8)   //CODE+
//    {
//        qDebug()<<"key_5_0";

//    }
//    if(e->key() == Qt::Key_F16)    //CODE-
//    {
//        qDebug()<<"key_4_0";

//    }
//    if(e->key() == Qt::Key_F7)//  HAND+
//    {
//        qDebug()<<"key_2_0";

//    }
//    if(e->key() == Qt::Key_F15)//  HAND-
//    {
//        SysCmdT(&xImdT, NCK_CMD_JOG, 100, 0);
//        xImdT.cNck = NCK_IMD;
//        qDebug()<<"key_1_0";

//    }
//    if(e->key() == Qt::Key_R)
//    {
//        qDebug()<<"key_R_0";

//    }

//    NcktCmd(&(xImdT.aImdT), NCK_JOG, 0, 100);
//    xImdT.flag = 1;
}

/*
** 函数功能：传递鼠标点击事件给MenuWidget控件
** 函数参数：鼠标事件
** 函数返回值：无
** 函数说明：此函数的作用就是将鼠标单击事件传递给子类MenuWidget控件，
************当点击的区域在控件外面时隐藏该控件
*/
void KUKA::mousePressEvent(QMouseEvent *e)
{
    QMainWindow::mousePressEvent(e);
    MenuBarWidget->frame_alarm->mousePressEvent(e);
    for(int i = 0 ; i < 8 ; i++)
    {
        if( i != 3)
            MenuBarWidget->frame_upmenu[i]->mousePressEvent(e);
    }
    for(int i = 0 ; i < 3 ; i++)
    {
        if( i != 1)
            MenuBarWidget->frame_rightmenu[i]->mousePressEvent(e);
    }
}

bool KUKA::eventFilter(QObject *watched, QEvent *event)
{
    if(  (  watched == lab_show_tip ) && event->type() == QEvent::MouseButtonPress)
    {
        qDebug()<<"come";
        if(frame_info->isHidden())
        {
            frame_info->show();
            frame_info->raise();
            INFOWidget->show();
        }
        else
            frame_info->hide();
    }
    return QWidget::eventFilter(watched,event);
}

/*
** 函数功能：检查当前有多少模块
** 函数参数：path，模块存储路径
** 函数返回值：无
** 函数说明：在任务目录下寻找所有的.dat文件，不管这个目录下的子文件夹
*/
void func_ReadModuleFromPath(QString path)
{
    QDir dir(path);
    QFileInfoList InfoList = dir.entryInfoList(QDir::Files);
    foreach(QFileInfo fileInfo , InfoList)
    {
        if( QString::compare(fileInfo.suffix() , "dat" , Qt::CaseInsensitive ) == 0 )
            DataFileList << fileInfo;//只将数据文件放入列表
    }
}

/*
** 函数功能：列出文件夹下的所有需要被译码的文件
** 函数参数：path，文件夹路径
** 函数返回值：无
** 函数说明：在任务目录下寻找所有的编辑器显示文件
*/
void func_ReadParserFileFromPath(QString path)
{
    QDir dir(path);
    QFileInfoList InfoList = dir.entryInfoList(QDir::Files);
    foreach(QFileInfo fileInfo , InfoList)
    {
        if( fileInfo.suffix().isEmpty() )
            ParserFileList << fileInfo;//只将供编辑器显示的文件放入列表
    }
}

void KUKA::func_initCodedata()
{
    funcParser = new Parser();
    for(int i = 0 ; i < dirInfoList.count() ; i++)
    {
        QString path = dirInfoList.at(i).absoluteFilePath();
        func_ReadModuleFromPath(path);
        func_ReadParserFileFromPath(path);
    }
    int count = DataFileList.count();
    for(int i = 0 ; i < count ; i++)
    {
        ModuleI temp;
        QString datpath;
        datpath = DataFileList.at(i).absoluteFilePath();
        temp.name = DataFileList.at(i).baseName();
        temp.datpath = datpath;
        temp.srcpath = datpath.replace(".dat",".src");
        if(!VarIsStandable(temp.name))  //不符合命名规范
            continue;
        if(func_checkModName( temp.name ) == -1)  //同名的模块
            continue;
        ModuleInfo.append(temp);
        func_ReadCodeDataFromFile(DataFileList.at(i).absoluteFilePath());
    }
    count = ParserFileList.count();
    for(int i = 0 ; i < count ; i++)
    {
        QString path = ParserFileList.at(i).absoluteFilePath();
        funcParser->startParser(path);
    }
    func_ReadRobSysDataFromFile(RobSysDataFile);
}


/*
** 函数功能：检查有无同名的模块
** 函数返回值：发现同名模块返回-1，没有同名的则返回0
** 函数说明：只比较名称，不比较后缀，即即使名称相同但后缀不同也会视为同名
*/
int func_checkModName(QString name)
{
    for(int i=0;i<ModuleInfo.count();i++)
    {
        if(QString::compare(name,ModuleInfo.at(i).name) == 0)
        {
            return -1;
        }
    }
    return 0;
}
KUKA::~KUKA()
{

}
