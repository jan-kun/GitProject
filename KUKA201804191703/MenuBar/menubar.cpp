#include "menubar.h"

MenuBar::MenuBar(QWidget *parent) :
    QWidget(parent)
{
    for( int i = 0 ; i < 8 ; i++ )
        frame_upmenu[i] = NULL;
    for( int i = 0 ; i < 3 ; i++ )
        frame_rightmenu[i] = NULL;
}

/*
** 函数功能：创建左侧显示各类报警信息控件界面
** 函数参数：parent：该控件的父类；xy：坐标
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_BuildAlarm(QWidget* parent,int x,int y)
{
    frame_left = new QPushButton(parent);
    frame_left->setGeometry(x,y,53,92);
    frame_left->setFocusPolicy(Qt::NoFocus);
    frame_left->setCheckable(true);
    frame_left->setStyleSheet("QPushButton:checked{border-top: 5px solid rgb(0,170,255);border-left: 5px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}"\
                        "QPushButton:unchecked{border:none} "\
                "QPushButton{background-color:rgb(160,165,170)}");
    frame_left->show();
    connect(frame_left,SIGNAL(clicked(bool)),this,SLOT(slot_BtnClicked(bool)));

    /*左侧四栏状态标签,用于显示当前各类报警信息个数*/
    for(int i=0;i<4;i++)
    {
        lab_leftb[i]=new QLabel(frame_left);
        lab_leftb[i]->setGeometry(2,2+i*23,48,19);
        lab_leftb[i]->setStyleSheet("background-color:rgb(82,90,94);color:white;border-radius:3px");
        lab_leftb[i]->setAlignment(Qt::AlignRight);
        lab_leftb[i]->setText(QString::number(KukaRob.alarm[i]));
        lab_leftb[i]->show();
    }
    for(int i=0;i<4;i++)
    {
        lab_leftd[i]=new QLabel(lab_leftb[i]);
        lab_leftd[i]->setGeometry(2,2,15,15);
        lab_leftd[i]->show();
    }
    lab_leftd[0]->setText(tr("X"));
    lab_leftd[0]->setStyleSheet("background-color:rgb(255,0,0);border-radius:2px");
    lab_leftd[1]->setText(tr("!"));
    lab_leftd[1]->setStyleSheet("background-color:rgb(255,215,0);border-radius:2px");
    lab_leftd[2]->setText(tr("i"));
    lab_leftd[2]->setStyleSheet("background-color:rgb(106,90,205);border-radius:2px");
    lab_leftd[3]->setText(tr("<"));
    lab_leftd[3]->setStyleSheet("background-color:rgb(248,248,255);border-radius:2px");
    /*用以闪烁Err标签*/
    timer_err = new QTimer();
    connect(timer_err,SIGNAL(timeout()),this,SLOT(slot_Timerout()));
    /*构建显示详细数目界面*/
    func_BuildAlarmDetail();
    func_hideAlarm();
}

/*
** 函数功能：构建显示详细各类报警数目界面
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_BuildAlarmDetail()
{
    frame_alarm=new MenuWidget(frame_left->parentWidget()->parentWidget());
    frame_alarm->setGeometry(53,53,250,210);
    frame_alarm->setObjectName("fa");
    frame_alarm->setStyleSheet("#fa{background-color:rgb(230,230,230);border-right: 5px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);border-top: 5px solid rgb(0,170,255);}");
    connect(frame_alarm,SIGNAL(aboutToHide()),this,SLOT(slot_aboutToHide()));
    connect(frame_alarm,SIGNAL(aboutToShow(MenuWidget*)),this,SLOT(slot_aboutToShow(MenuWidget*)));

    QLabel *lab_tip=new QLabel(frame_alarm);
    lab_tip->setGeometry(0,5,245,30);
    lab_tip->setText(tr(" 现有信息"));
    lab_tip->setStyleSheet("background-color:rgb(112,123,137);color:white;font:bold;font-size:14px");

    QLabel* lab_temp[8];
    for(int i=0;i<8;i++)
    {
        lab_temp[i]=new QLabel(frame_alarm);
        if(i<4)
        {
            lab_temp[i]->setGeometry(20,50+38*i,18,18);
            lab_temp[i]->setAlignment(Qt::AlignCenter);
        }
        else
            lab_temp[i]->setGeometry(58,50+40*(i-4),100,20);
    }
    lab_temp[0]->setText(tr("X"));
    lab_temp[0]->setStyleSheet("background-color:rgb(255,0,0)");
    lab_temp[1]->setText(tr("!"));
    lab_temp[1]->setStyleSheet("background-color:rgb(255,215,0)");
    lab_temp[2]->setText(tr("i"));
    lab_temp[2]->setStyleSheet("background-color:rgb(44,152,208)");
    lab_temp[3]->setText(tr("<"));
    lab_temp[3]->setStyleSheet("background-color:rgb(155,247,16)");

    lab_temp[4]->setText(tr("确认信息   :"));
    lab_temp[5]->setText(tr("状态信息   :"));
    lab_temp[6]->setText(tr("提示信息   :"));
    lab_temp[7]->setText(tr("等待信息   :"));

    /*显示各类报警信息数目标签*/
    for(int i = 0 ; i < 4 ; i++)
    {
        lab_alarm[i] = new QLabel(frame_alarm);
        lab_alarm[i]->setGeometry(178,50+40*i,20,20);
    }
}

/*
** 函数功能：创建上栏8个控件
** 函数参数：parent：该控件的父类；xy：坐标
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_BuildUp(QWidget *parent, int x, int y)
{
    frame_up = new QFrame(parent);
    frame_up->setGeometry(x,y,410,53);
    frame_up->setStyleSheet("background-color:rgb(160,165,170)");
    frame_up->show();

//    Gbtn = new QButtonGroup(this);
//    Gbtn->setExclusive(true);   //设置其中按钮点击互斥
    for(int i=0;i<8;i++)
    {
        btn_up[i]=new QPushButton(frame_up);
        btn_up[i]->setGeometry(i*42,6,38,40);
        btn_up[i]->show();
        btn_up[i]->setFocusPolicy(Qt::NoFocus);
        btn_up[i]->setCheckable(true);
        if( i < 4)
            btn_up[i]->setStyleSheet("QPushButton:checked{border: 3px solid rgb(0,170,255);border-radius:3px}"\
                                  "QPushButton:unchecked{border:none} "\
                                 "QPushButton{background-color:rgb(215,215,215);border-radius:3px}");
        else
            btn_up[i]->setStyleSheet("QPushButton:checked{border: 3px solid rgb(0,170,255);border-radius:3px;border-top:1px solid black}"\
                                     "QPushButton:unchecked{border:none} "\
                                 "QPushButton{background-color:rgb(82,90,94);color:white;border-radius:3px;border-top:1px solid black}");
        connect(btn_up[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnGClicked(bool)));
//        Gbtn->addButton(btn_up[i]);
    }

    btn_up[2]->setText("R");
    btn_up[4]->setGeometry(169,6,59,40);
    btn_up[5]->setGeometry(231,6,38,40);
    btn_up[6]->setGeometry(272,6,59,40);
    btn_up[7]->setGeometry(334,6,70,40);

    btn_up[5]->setIconSize(QSize(38,40));
    /*设置按钮上的图片*/
    lab_icon[0]=new QLabel(btn_up[4]);
    lab_icon[0]->setGeometry(2,2,25,36);
    lab_icon[0]->setPixmap(QPixmap(":/1.PNG"));
    lab_icon[0]->setStyleSheet("background-color:rgba(255,255,255,0)");
    /*显示程序运行速度标签*/
    lab_icon[1]=new QLabel(btn_up[4]);
    lab_icon[1]->setGeometry(30,2,30,20);
    lab_icon[1]->setStyleSheet("font:bold 14px;color:white;background-color:rgba(255,255,255,0)");
    lab_icon[1]->setText(QString::number(KukaRob.Spd_code)+"%");
    /*显示手动速度标签*/
    lab_icon[2]=new QLabel(btn_up[4]);
    lab_icon[2]->setGeometry(30,22,30,20);
    lab_icon[2]->setStyleSheet("font:bold 14px;color:white;background-color:rgba(255,255,255,0)");
    lab_icon[2]->setText(QString::number(KukaRob.Spd_hand)+"%");
    /*ipo模式选择*/
    lab_icon[3]=new QLabel(btn_up[6]);
    lab_icon[3]->setGeometry(2,2,25,36);
    lab_icon[3]->setPixmap(QPixmap("://3.1.PNG"));
    lab_icon[3]->setStyleSheet("background-color:rgba(255,255,255,0)");
    /*显示工具号标签*/
    lab_icon[4]=new QLabel(btn_up[6]);
    lab_icon[4]->setGeometry(30,2,30,20);
    lab_icon[4]->setText(tr("W0"));
    lab_icon[4]->setStyleSheet("font:bold 13px;color:white;background-color:rgba(255,255,255,0)");
    /*显示基坐标号标签*/
    lab_icon[5]=new QLabel(btn_up[6]);
    lab_icon[5]->setGeometry(30,22,30,20);
    lab_icon[5]->setText(tr("B0"));
    lab_icon[5]->setStyleSheet("font:bold 13px;color:white;background-color:rgba(255,255,255,0)");
    /*显示增量图标标签*/
    lab_icon[6]=new QLabel(btn_up[7]);
    lab_icon[6]->setGeometry(3,2,25,36);
    lab_icon[6]->setPixmap(QPixmap(":/4.PNG"));
    lab_icon[6]->setStyleSheet("background-color:rgba(255,255,255,0)");
    /*显示增量大小标签*/
    lab_icon[7]=new QLabel(btn_up[7]);
    lab_icon[7]->setGeometry(28,0,40,40);
    lab_icon[7]->setStyleSheet("font:bold 13px;color:white;background-color:rgba(255,255,255,0)");
    lab_icon[7]->setText("100 mm\n  10°");

    func_BuildSumit();
    func_BuildDriver();
    func_BuildCode();
    func_BuildSpd();
    func_BuildMod();
    func_BuildTB();
    func_BuildIncre();

    for(int i = 0 ; i < 8 ; i++)
    {
        if(i == 3)
            continue;
        connect(frame_upmenu[i],SIGNAL(aboutToShow(MenuWidget*)),this,SLOT(slot_aboutToShow(MenuWidget*)));
        connect(frame_upmenu[i],SIGNAL(aboutToHide()),this,SLOT(slot_aboutToHide()));
    }

    func_UpdateSubmitState(0);
    func_UpdateDriver(0);
    func_UpdateRobMod(KukaRob.RobMod);
    func_UpdateSpd(KukaRob.Spd_code,KukaRob.Spd_hand);
    func_UpdateCMod(KukaRob.CodeMod);
    func_UpdateTB(KukaRob.indext,KukaRob.indexb,KukaRob.ipo);
    func_UpdateIncre(KukaRob.Incre);

    func_hideSubmit();
    func_hideDriver();
    func_hideCode();
    func_hideSpd();
    func_hideMod();
    func_hideMod();
    func_hideTB();
    func_hideIncre();
}

void MenuBar::func_BuildSumit()
{
    frame_upmenu[0]=new MenuWidget(frame_up->parentWidget()->parentWidget());
    frame_upmenu[0]->setGeometry(147,53,150,190);
    frame_upmenu[0]->setObjectName("fu0");
    frame_upmenu[0]->setStyleSheet("#fu0{background-color:rgb(230,230,230);border-left: 5px solid rgb(0,170,255);border-right: 5px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}");

    QLabel *lab=new QLabel(frame_upmenu[0]);
    lab->setGeometry(5,0,140,30);
    lab->setText(tr(" SUBMIT解释器"));
    lab->setStyleSheet("background-color:rgb(112,123,137);color:white;font:bold;font-size:14px");

    for(int i=0;i<3;i++)
    {
        btn_submit[i]=new QPushButton(frame_upmenu[0]);
        btn_submit[i]->setGeometry(25,40+i*50,100,40);
        btn_submit[i]->setFocusPolicy(Qt::NoFocus);
        connect(btn_submit[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnClicked(bool)));
    }
    btn_submit[0]->setText(tr("启动/选择"));
    btn_submit[1]->setText(tr("停止"));
    btn_submit[2]->setText(tr("取消选择"));
}

void MenuBar::func_BuildDriver()
{
    frame_upmenu[1]=new MenuWidget(frame_up->parentWidget()->parentWidget());
    frame_upmenu[1]->setGeometry(112,53,300,325 );
    frame_upmenu[1]->setObjectName("fu1");
    frame_upmenu[1]->setStyleSheet("#fu1{background-color:rgb(230,230,230);border-left: 5px solid rgb(0,170,255);border-right: 5px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}");

    QLabel *lab[7];
    QString text[6] = {"驱动装置","Safety驱动装置开通","操作人员防护装置","Safety运行许可","确认键","Office安装时无需确认键"};
    for(int i=0;i<6;i++)
    {
        lab[i]=new QLabel(frame_upmenu[1]);
        lab[i]->setGeometry(20+130*(i%2),50+95*(i/2),150,20);
        lab[i]->setText(text[i]);
    }
    lab[6]=new QLabel(frame_upmenu[1]);
    lab[6]->setGeometry(5,0,290,30);
    lab[6]->setText(tr(" 移动条件"));
    lab[6]->setStyleSheet("background-color:rgb(112,123,137);color:white;font:bold;font-size:14px");

    for(int i=0;i<6;i++)
    {
        btn_driver[i]=new QPushButton(frame_upmenu[1]);
        btn_driver[i]->setFocusPolicy(Qt::NoFocus);
        connect(btn_driver[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnClicked(bool)));
    }
    btn_driver[0]->setText(tr("|"));
    btn_driver[0]->setGeometry(50,75,70,50);
    btn_driver[1]->setText(tr("O"));
    btn_driver[1]->setGeometry(150,75,70,50);
    btn_driver[2]->setGeometry(220,75,50,50);
    btn_driver[3]->setGeometry(50,170,50,50);
    btn_driver[4]->setGeometry(220,170,50,50);
    btn_driver[5]->setGeometry(50,265,50,50);
}

void MenuBar::func_BuildCode()
{
    frame_upmenu[2]=new MenuWidget(frame_up->parentWidget()->parentWidget());
    frame_upmenu[2]->setGeometry(227,53,150,140);
    frame_upmenu[2]->setObjectName("fu2");
    frame_upmenu[2]->setStyleSheet("#fu2{background-color:rgb(230,230,230);border-left: 5px solid rgb(0,170,255);border-right: 5px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}");

    QLabel *lab=new QLabel(frame_upmenu[2]);
    lab->setGeometry(5,0,140,30);
    lab->setText(tr(" 程序"));
    lab->setStyleSheet("background-color:rgb(112,123,137);color:white;font:bold;font-size:14px");

    for(int i=0;i<2;i++)
    {
        btn_code[i]=new QPushButton(frame_upmenu[2]);
        btn_code[i]->setGeometry(25,40+i*50,100,40);
        btn_code[i]->setFocusPolicy(Qt::NoFocus);
        connect(btn_code[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnClicked(bool)));
    }
    btn_code[0]->setText(tr("取消选择程序"));
    btn_code[1]->setText(tr("程序复位"));
}

void MenuBar::func_BuildSpd()
{
    frame_upmenu[4]=new MenuWidget(frame_up->parentWidget()->parentWidget());
    frame_upmenu[4]->setGeometry(58,53,500,260);
    frame_upmenu[4]->setObjectName("fu4");
    frame_upmenu[4]->setStyleSheet("#fu4{background-color:rgb(230,230,230);border-left: 5px solid rgb(0,170,255);border-right: 5px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}");

    QLabel *lab=new QLabel(frame_upmenu[4]);
    lab->setGeometry(5,0,490,30);
    lab->setText(tr(" 调节量"));
    lab->setStyleSheet("background-color:rgb(112,123,137);font:bold;font-size:14px;color:white");

    QLabel *lab1=new QLabel(frame_upmenu[4]);
    lab1->setGeometry(20,50,80,20);
    lab1->setText(tr("程序调节量"));

    QLabel *lab2=new QLabel(frame_upmenu[4]);
    lab2->setGeometry(20,155,80,20);
    lab2->setText(tr("手动调节量"));
    /*显示程序运行速度标签*/
    lab_spd[0]=new QLabel(frame_upmenu[4]);
    lab_spd[0]->setGeometry(190,75,60,50);
    /*显示手动速度标签*/
    lab_spd[1]=new QLabel(frame_upmenu[4]);
    lab_spd[1]->setGeometry(190,180,60,20);

    for(int i=0;i<5;i++)
    {
        btn_spd[i]=new QPushButton(frame_upmenu[4]);
        btn_spd[i]->setFocusPolicy(Qt::NoFocus);
        btn_spd[i]->setGeometry(20+(i%2)*90,75+(i/2)*105,62,42);
        btn_spd[i]->setIconSize(QSize(62,42));
        connect(btn_spd[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnClicked(bool)));
    }
    btn_spd[0]->setIcon(QIcon(":/image/image/btn_decl.png"));
    btn_spd[1]->setIcon(QIcon(":/image/image/btn_add.png"));
    btn_spd[2]->setIcon(QIcon(":/image/image/btn_decl.png"));
    btn_spd[3]->setIcon(QIcon(":/image/image/btn_add.png"));
    btn_spd[4]->setGeometry(430,180,60,50);
    btn_spd[4]->setText(tr("选项"));

    for(int i = 0 ; i < 2 ; i++)
    {
        slider[i]=new QSlider(Qt::Horizontal,frame_upmenu[4]);
        slider[i]->setMaximum(100);
        slider[i]->setMinimum(0);
        slider[i]->setGeometry(260,75+95*i,150,50);
        slider[i]->setStyleSheet("QSlider::handle:horizontal{border: 1px solid black;width:60px;height:50px;border-image:  url(:/1.1.PNG)} QSlider::groove:horizontal{background: white;height: 35px}");
        connect(slider[i],SIGNAL(valueChanged(int)),SLOT(slot_Slider(int)));
    }
    slider[1]->setStyleSheet("QSlider::handle:horizontal{width:60px;border-image: url(:/1.2.PNG)} QSlider::groove:horizontal{background: white;height: 35px}");
}

void MenuBar::func_BuildMod()
{
    frame_upmenu[5]=new MenuWidget(frame_up->parentWidget()->parentWidget());
    frame_upmenu[5]->setGeometry(201,53,340,150);
    frame_upmenu[5]->setObjectName("fu5");
    frame_upmenu[5]->setStyleSheet("#fu5{background-color:rgb(230,230,230);border-left: 5px solid rgb(0,170,255);border-right: 5px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}");

    QLabel *lab[4];
    for(int i=0;i<4;i++)
    {
        lab[i]=new QLabel(frame_upmenu[5]);
        lab[i]->setGeometry(40+(i-1)*90,105,60,30);
    }
    lab[0]->setGeometry(5,0,330,30);
    lab[0]->setText(tr(" 程序运行方式"));
    lab[0]->setStyleSheet("background-color:rgb(112,123,137);font:bold;font-size:14px;color:white");
    lab[1]->setText(tr("Go"));
    lab[2]->setText(tr("动作"));
    lab[2]->setGeometry(120,105,60,30);
    lab[3]->setText(tr("单个步骤"));
    lab[3]->setGeometry(185,105,60,30);

    for(int i=0;i<4;i++)
    {
        btn_codemod[i]=new QPushButton(frame_upmenu[5]);
        btn_codemod[i]->setGeometry(20+i*80,50,60,50);
        btn_codemod[i]->setFocusPolicy(Qt::NoFocus);
        btn_codemod[i]->setStyleSheet("background-color:rgb(182,182,182);");
        connect(btn_codemod[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnClicked(bool)));
    }
    btn_codemod[3]->setText(tr("选项"));

    GbtnCodeMod = new QButtonGroup(frame_upmenu[5]);
    GbtnCodeMod->setExclusive(true);
    for(int i=0;i<3;i++)
    {
        rbtn_codemod[i]=new QRadioButton(btn_codemod[i]);
        rbtn_codemod[i]->setGeometry(5,23,20,20);
        GbtnCodeMod->addButton(rbtn_codemod[i],i);
    }
    connect(GbtnCodeMod,SIGNAL(buttonClicked(int)),this,SLOT(slot_BtnGCodeModCliked(int)));

    QLabel *icon[3];
    for(int i=0;i<3;i++)
    {
        icon[i]=new QLabel(btn_codemod[i]);
        icon[i]->setGeometry(25,5,30,40);
    }
    icon[0]->setPixmap(QPixmap(":/image/image/2.1.1.png"));
    icon[1]->setPixmap(QPixmap(":/image/image/2.2.1.png"));
    icon[2]->setPixmap(QPixmap(":/image/image/2.3.1.png"));
}

void MenuBar::func_BuildTB()
{
    frame_upmenu[6]=new MenuWidget(frame_up->parentWidget()->parentWidget());
    frame_upmenu[6]->setGeometry(191,53,350,345);
    frame_upmenu[6]->setObjectName("fu6");
    frame_upmenu[6]->setStyleSheet("#fu6{background-color:rgb(230,230,230);border-left: 5px solid rgb(0,170,255);border-right: 5px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}");

    QLabel *lab[6];
    for(int i=0;i<6;i++)
    {
        lab[i]=new QLabel(frame_upmenu[6]);
        lab[i]->setGeometry(20,50+(i-1)*85,100,20);
        lab[i]->show();
    }
    lab[0]->setGeometry(5,0,340,30);
    lab[0]->setText(tr("  激活的基坐标/工具"));
    lab[0]->setStyleSheet("background-color:rgb(112,123,137);font:bold;font-size:14px;color:white");
    lab[1]->setText(tr("工具选择"));
    lab[2]->setText(tr("基坐标选择"));
    lab[3]->setText(tr("Ipo模式选择"));
    lab[4]->setText(tr("法兰"));
    lab[4]->setGeometry(70,300,60,20);
    lab[5]->setText(tr("外部工具"));
    lab[5]->setGeometry(135,300,60,20);

    cb_tool=new QComboBox(frame_upmenu[6]);
    cb_tool->setGeometry(50,75,270,50);
    cb_tool->show();
    cb_tool->setStyleSheet("QComboBox:editable{background: white;} QComboBox::drop-down{ width:60;height:50}");

    cb_base=new QComboBox(frame_upmenu[6]);
    cb_base->setGeometry(50,160,270,50);
    cb_base->show();
    cb_base->setStyleSheet("QComboBox:editable{background: white;} QComboBox::drop-down{ width:60;height:50}");

    connect(cb_tool,SIGNAL(activated(int)),this,SLOT(slot_CbClicked(int)));
    connect(cb_base,SIGNAL(activated(int)),this,SLOT(slot_CbClicked(int)));

    for(int i=0;i<2;i++)
    {
        btn_ipo[i]=new QPushButton(frame_upmenu[6]);
        btn_ipo[i]->setGeometry(50+i*80,245,60,50);
        btn_ipo[i]->setFocusPolicy(Qt::NoFocus);
        btn_ipo[i]->setStyleSheet("background-color:rgb(182,182,182);");
        btn_ipo[i]->show();
        connect(btn_ipo[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnClicked(bool)));
    }
    GbtnIpo=new QButtonGroup(frame_upmenu[6]);
    GbtnIpo->setExclusive(true);
    for(int i=0;i<2;i++)
    {
        rbtn_ipo[i]=new QRadioButton(btn_ipo[i]);
        rbtn_ipo[i]->setGeometry(5,23,20,20);
        rbtn_ipo[i]->show();
        GbtnIpo->addButton(rbtn_ipo[i],i);
    }
    connect(GbtnIpo,SIGNAL(buttonClicked(int)),this,SLOT(slot_btnGIPOClicked(int)));
    QLabel *icon[2];
    for(int i=0;i<2;i++)
    {
        icon[i]=new QLabel(btn_ipo[i]);
        icon[i]->setGeometry(25,5,30,40);
        icon[i]->show();
    }
    icon[0]->setPixmap(QPixmap(":/image/image/3.1.1.png"));
    icon[1]->setPixmap(QPixmap(":/image/image/3.2.1.png"));
}

void MenuBar::func_BuildIncre()
{
    frame_upmenu[7]=new MenuWidget(frame_up->parentWidget()->parentWidget());
    frame_upmenu[7]->setGeometry(401,53,140,360);
    frame_upmenu[7]->setObjectName("fu7");
    frame_upmenu[7]->setStyleSheet("#fu7{background-color:rgb(230,230,230);border-left: 5px solid rgb(0,170,255);border-right: 5px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}");

    QLabel *lab=new QLabel(frame_upmenu[7]);
    lab->setGeometry(5,0,130,30);
    lab->setText(tr(" 增量式手动移动"));
    lab->setStyleSheet("background-color:rgb(112,123,137);font:bold;font-size:14px;color:white");
    lab->show();

    GbtnIncre = new QButtonGroup(frame_upmenu[7]);
    GbtnIncre->setExclusive(true);
    for(int i=0;i<6;i++)
    {
        btn_incre[i]=new QPushButton(frame_upmenu[7]);
        btn_incre[i]->setGeometry(10,40+i*45,120,45);
        btn_incre[i]->setFocusPolicy(Qt::NoFocus);
        btn_incre[i]->show();
        btn_incre[i]->setCheckable(true);
        if(i==5)
        {
            btn_incre[i]->setText(tr("选项"));
            btn_incre[i]->setGeometry(40,295,60,45);
            continue;
        }
        btn_incre[i]->setStyleSheet("QPushButton:checked{background-color:rgb(57,185,255)}"\
                    "QPushButton{background-color:rgb(248,248,248);border-top:1px solid black;border-left:1px solid black}");
        btn_incre[i]->setCheckable(true);
        GbtnIncre->addButton(btn_incre[i],i);
    }
    connect(GbtnIncre,SIGNAL(buttonClicked(int)),this,SLOT(slot_BtnGIncre(int)));
    btn_incre[0]->setText(tr("持续的"));
    btn_incre[1]->setText(tr("100mm/10"));
    btn_incre[2]->setText(tr("10mm/3"));
    btn_incre[3]->setText(tr("1mm/1"));
    btn_incre[4]->setText(tr("0.1mm/0.005"));
}


/*
** 函数功能：创建右侧栏三个菜单按钮控件
** 函数参数：parent：父类控件；xy：坐标
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_BuildActmod(QWidget *parent, int x, int y)
{
    frame_right=new QLabel(parent);
    frame_right->setGeometry(x,y,53,730);
    frame_right->setStyleSheet("background-color:rgb(160,165,170)");

    Gbtn1 = new QButtonGroup(this);
    Gbtn1->setExclusive(true);
    for(int i=0;i<3;i++)
    {
        btn_right[i]=new QPushButton(frame_right);
        btn_right[i]->setFocusPolicy(Qt::NoFocus);
        btn_right[i]->show();
        btn_right[i]->setCheckable(true);
        connect(btn_right[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnG1Clicked(bool)));
    }
    btn_right[0]->setGeometry(0,0,54,33);
    btn_right[0]->setStyleSheet("QPushButton:checked{border-top: 3px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}"\
                                                   "QPushButton{background-color:rgb(160,165,170);border-top:1px solid rgb(128,128,128)}");
    btn_right[1]->setGeometry(0,33,54,92);
    btn_right[1]->setStyleSheet("QPushButton:checked{border-top: 3px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}"\
                                "QPushButton{background-color:rgb(160,165,170);border:none;}");
    btn_right[2]->setGeometry(0,125,54,37);
    btn_right[2]->setStyleSheet("QPushButton:checked{border-top: 3px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}"\
                                "QPushButton{background-color:rgb(160,165,170);border-top:1px solid rgb(128,128,128);}");

    btn_right[0]->setIconSize(QSize(36,33));
    btn_right[0]->setIcon(QIcon(":/image/image/mouse.PNG"));
    btn_right[1]->setIconSize(QSize(36,92));
    btn_right[1]->setIcon(QIcon(":/image/image/key.PNG"));
    btn_right[2]->setIconSize(QSize(36,37));
    btn_right[2]->setIcon(QIcon(":/image/image/anix2.PNG"));
    /*显示A1-A6和速度标签*/
    QFont font /*("Microsoft YaHei", 18, 99)*/; //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    font.setPointSize(160); //实际上是16的字号，但设成16却不行
    font.setFamily(("wenquanyi"));
    font.setBold(true);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(211,211,211));
    for(int i=0;i<8;i++)
    {
        lab_right[i]=new QLabel(frame_right);
        lab_right[i]->show();
        lab_right[i]->setGeometry(15,165+70*i,34,35);
        if(i < 6)
        {
            lab_right[i]->setText("A"+QString::number(i+1));
            lab_right[i]->setFont(font);
            lab_right[i]->setPalette(pa);
            lab_right[i]->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
    lab_right[0]->setGeometry(10,180,34,35);
    lab_right[1]->setGeometry(10,250,34,35);
    lab_right[2]->setGeometry(10,325,34,35);
    lab_right[3]->setGeometry(10,395,34,35);
    lab_right[4]->setGeometry(10,465,34,35);
    lab_right[5]->setGeometry(10,540,34,35);
    QString strJU[6]={":/image/image/A1U.png",":/image/image/A2U.png",":/image/image/A3U.png",\
                              ":/image/image/A4U.png",":/image/image/A5U.png",":/image/image/A6U.png"};
    for(int i = 0 ; i < 6 ; i++)
        lab_right[i]->setPixmap(QPixmap(strJU[i]));
    lab_right[6]->setGeometry(5,615,34,35);
    lab_right[6]->setPixmap(QPixmap(":/image/image/codeu.png"));
    lab_right[7]->setGeometry(5,685,34,35);
    lab_right[7]->setPixmap(QPixmap(":/image/image/handu.png"));

    func_BuildMActmod();
    func_BuildKActmod();
    func_hideMActmod();
    func_hideKActmod();
}

void MenuBar::func_BuildMActmod()
{
    frame_rightmenu[0]=new MenuWidget(frame_right->parentWidget()->parentWidget());
    frame_rightmenu[0]->setGeometry(453,53,90,405);
    frame_rightmenu[0]->setObjectName("frm0");
    frame_rightmenu[0]->setStyleSheet("#frm0{background-color:rgb(230,230,230);border-left: 5px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}");
    connect(frame_rightmenu[0],SIGNAL(aboutToHide()),this,SLOT(slot_aboutToHide()));
    connect(frame_rightmenu[0],SIGNAL(aboutToShow(MenuWidget*)),this,SLOT(slot_aboutToShow(MenuWidget*)));

    QLabel *lab_temp[5];
    for(int i=0;i<5;i++)
    {
        lab_temp[i] = new QLabel(frame_rightmenu[0]);
        lab_temp[i]->setGeometry(15,85+75*i,60,20);
        lab_temp[i]->show();
        lab_temp[i]->setAlignment(Qt::AlignHCenter);
    }
    lab_temp[0]->setText(tr("轴"));
    lab_temp[1]->setText(tr("全局"));
    lab_temp[2]->setText(tr("基坐标"));
    lab_temp[3]->setText(tr("工具"));

    lab_temp[4]->setGeometry(5,0,85,30);
    lab_temp[4]->setText(tr("鼠标"));
    lab_temp[4]->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    lab_temp[4]->setStyleSheet("background-color:rgb(112,123,137);font:bold;font-size:14px;color:white");

    for(int i=0;i<5;i++)
    {
        btn_mouse[i]=new QPushButton(frame_rightmenu[0]);
        btn_mouse[i]->setGeometry(10,40+75*i,70,45);
        btn_mouse[i]->setFocusPolicy(Qt::NoFocus);
        btn_mouse[i]->show();
        btn_mouse[i]->setIconSize(QSize(40,40));
        connect(btn_mouse[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnClicked(bool)));
    }
    btn_mouse[0]->setIcon(QIcon(":/image/image/mouse1.png"));
    btn_mouse[1]->setIcon(QIcon(":/image/image/mouse2.png"));
    btn_mouse[2]->setIcon(QIcon(":/image/image/mouse3.png"));
    btn_mouse[3]->setIcon(QIcon(":/image/image/mouse4.png"));
    btn_mouse[4]->setText(tr("选项"));

    GbtnMouse = new QButtonGroup(frame_rightmenu[0]);
    GbtnMouse->setExclusive(true);
    for(int i=0;i<4;i++)
    {
        rbtn_mouse[i]=new QRadioButton(btn_mouse[i]);
        rbtn_mouse[i]->setGeometry(2,12,20,20);
        rbtn_mouse[i]->show();
        GbtnMouse->addButton( rbtn_mouse[i] , i );
    }
    connect(GbtnMouse,SIGNAL(buttonClicked(int)),this,SLOT(slot_BtnGMCor(int)));
}

void MenuBar::func_BuildKActmod()
{
    frame_rightmenu[2]=new MenuWidget(frame_right->parentWidget()->parentWidget());
    frame_rightmenu[2]->setGeometry(453,53,90,405);
    frame_rightmenu[2]->setObjectName("frm2");
    frame_rightmenu[2]->setStyleSheet("#frm2{background-color:rgb(230,230,230);border-left: 5px solid rgb(0,170,255);border-bottom: 5px solid rgb(0,170,255);}");
    connect(frame_rightmenu[2],SIGNAL(aboutToHide()),this,SLOT(slot_aboutToHide()));
    connect(frame_rightmenu[2],SIGNAL(aboutToShow(MenuWidget*)),this,SLOT(slot_aboutToShow(MenuWidget*)));

    QLabel* lab_temp[5];
    for(int i=0;i<5;i++)
    {
        lab_temp[i] = new QLabel(frame_rightmenu[2]);
        lab_temp[i]->setGeometry(15,85+75*i,60,20);
        lab_temp[i]->show();
        lab_temp[i]->setAlignment(Qt::AlignHCenter);
    }
    lab_temp[0]->setText(tr("轴"));
    lab_temp[1]->setText(tr("全局"));
    lab_temp[2]->setText(tr("基坐标"));
    lab_temp[3]->setText(tr("工具"));
    lab_temp[4]->setGeometry(5,0,85,30);
    lab_temp[4]->setText(tr("  按键"));
    lab_temp[4]->setStyleSheet("background-color:rgb(112,123,137);font:bold;font-size:14px;color:white");

    for(int i=0;i<5;i++)
    {
        btn_key[i]=new QPushButton(frame_rightmenu[2]);
        btn_key[i]->setGeometry(10,40+75*i,70,45);
        btn_key[i]->setFocusPolicy(Qt::NoFocus);
        btn_key[i]->show();
        btn_key[i]->setIconSize(QSize(40,40));
        connect(btn_key[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnClicked(bool)));
    }
    btn_key[0]->setIcon(QIcon(":/image/image/mouse1.png"));
    btn_key[1]->setIcon(QIcon(":/image/image/mouse2.png"));
    btn_key[2]->setIcon(QIcon(":/image/image/mouse3.png"));
    btn_key[3]->setIcon(QIcon(":/image/image/mouse4.png"));
    btn_key[4]->setText(tr("选项"));

    GbtnKey=new QButtonGroup(frame_rightmenu[2]);
    GbtnKey->setExclusive(true);
    for(int i=0;i<4;i++)
    {
        rbtn_key[i]=new QRadioButton(btn_key[i]);
        rbtn_key[i]->setGeometry(2,12,20,20);
        rbtn_key[i]->show();
        GbtnKey->addButton(rbtn_key[i],i);
    }
    connect(GbtnKey,SIGNAL(buttonClicked(int)),this,SLOT(slot_BtnGKCor(int)));
}

/*
** 函数功能：展开显示各类报警信息数目的详情页
** 函数参数：alarm[4]，各类报警信息数目
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_showAlarm(int alarm[4])
{
    frame_alarm->show();
    frame_alarm->raise();
    for(int i = 0 ; i < 4 ; i++)
        lab_alarm[i]->setText(QString::number(alarm[i]));
}

void MenuBar::func_hideAlarm()
{
    frame_alarm->hide();
}

/*
** 函数功能：展开Submit功能页面
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_showSubmit()
{
    frame_upmenu[0]->show();
    frame_upmenu[0]->raise();
}

void MenuBar::func_hideSubmit()
{
    frame_upmenu[0]->hide();
}

/*
** 函数功能：展开显示驱动装置状态页面
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_showDriver()
{
    frame_upmenu[1]->show();
    frame_upmenu[1]->raise();
    func_UpdateDriver(KukaRob.Motor);
}

void MenuBar::func_hideDriver()
{
    frame_upmenu[1]->hide();
}

/*
** 函数功能：展开显示程序功能详细功能页面
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_showCode()
{
    frame_upmenu[2]->show();
    frame_upmenu[2]->raise();
}

void MenuBar::func_hideCode()
{
    frame_upmenu[2]->hide();
}

/*
** 函数功能：展开显示程序和手动速度控制页面
** 函数参数：code：程序速度；hand：手动速度
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_showSpd(int code, int hand)
{
    frame_upmenu[4]->show();
    frame_upmenu[4]->raise();
    lab_spd[0]->setText(QString::number(code)+"%");
    lab_spd[1]->setText(QString::number(hand)+"%");
    slider[0]->setValue(code);
    slider[1]->setValue(hand);
}

void MenuBar::func_hideSpd()
{
    frame_upmenu[4]->hide();
}

/*
** 函数功能：展开选择程序运行模式页面
** 函数参数：mod：当前程序运行模式
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_showMod(int mod)
{
    frame_upmenu[5]->show();
    frame_upmenu[5]->raise();
    if(mod == 0)
    {
        btn_up[5]->setIcon(QPixmap(":/2.1.PNG"));
        rbtn_codemod[0]->setChecked(true);
    }
    else if(mod == 1)
    {
        btn_up[5]->setIcon(QPixmap(":/2.2.PNG"));
        rbtn_codemod[1]->setChecked(true);
    }
}

void MenuBar::func_hideMod()
{
    frame_upmenu[5]->hide();
}

/*
** 函数功能：展开设置工具、基座标、IPO功能页面
** 函数参数：indext：当前工具号；indexb：当前基座标号；ipo
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_showTB(int indext, int indexb, int ipo)
{
    frame_upmenu[6]->show();
    frame_upmenu[6]->raise();
    set_Cb_Tool(cb_tool);
    set_Cb_Base(cb_base);
    cb_tool->setCurrentIndex(indext);
    cb_base->setCurrentIndex(indexb);
    if( ipo == 0)
        rbtn_ipo[0]->setChecked(true);
    else
        rbtn_ipo[1]->setChecked(true);
}

void MenuBar::func_hideTB()
{
    frame_upmenu[6]->hide();
}

/*
** 函数功能：展开设置增量页面
** 函数参数：incre：当前增量
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_showIncre(int incre)
{
    frame_upmenu[7]->show();
    frame_upmenu[7]->raise();
    if(incre < 0 || incre > 4)
        return;
    btn_incre[incre]->setChecked(true);
}

void MenuBar::func_hideIncre()
{
    frame_upmenu[7]->hide();
}

/*
** 函数功能：展开选择摇杆下运动模式功能页面
** 函数参数：mod：当前摇杆下的运动模式
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_showMActmod(int mod)
{
    frame_rightmenu[0]->show();
    frame_rightmenu[0]->raise();
    if(mod < 0 || mod > 3)
        return;
    rbtn_mouse[mod]->setChecked(true);
}

void MenuBar::func_hideMActmod()
{
    frame_rightmenu[0]->hide();
}

/*
** 函数功能：展开选择按键下运动模式功能页面
** 函数参数：mod：当前按键下的运动模式
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_showKActmod(int mod)
{
    frame_rightmenu[2]->show();
    frame_rightmenu[2]->raise();
    if(mod < 0 || mod > 3)
        return;
    rbtn_key[mod]->setChecked(true);
}

void MenuBar::func_hideKActmod()
{
    frame_rightmenu[2]->hide();
}

void MenuBar::func_UpdateSubmitState(int state)
{
    btn_up[0]->setText("S");
}

void MenuBar::func_UpdateDriver(int state)
{
    btn_up[1]->setText("l");
    if(state)
        btn_driver[5]->setStyleSheet("background-color:green");
    else
        btn_driver[5]->setStyleSheet("background-color:rgb(208,207,207)");
}

/*
** 函数功能：更新机器人模式，手动、自动等
** 函数参数：mod，当前模式
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_UpdateRobMod(int mod)
{
    if(mod == 0)
        btn_up[3]->setText("T1");
}

/*
** 函数功能：当前程序速度或手动速度发生变化时需要进行的更新操作
** 函数参数：hand：当前程序速度；code：当前手动速度
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_UpdateSpd(int hand, int code)
{
    lab_spd[0]->setText(QString::number(code)+"%");
    lab_spd[1]->setText(QString::number(hand)+"%");
    lab_icon[1]->setText(QString::number(code));
    lab_icon[2]->setText(QString::number(hand));
    slider[0]->setValue(code);
    slider[1]->setValue(hand);
    KukaRob.Spd_hand = hand;
    KukaRob.Spd_code = code;
}

/*
** 函数功能：当前程序运行模式发生变化时需要进行的更新操作
** 函数参数：mod：当前程序运行模式
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_UpdateCMod(int mod)
{
    if( mod > 2 || mod < 0)
        return;
    KukaRob.CodeMod = (CModel)(mod);
    if(mod == 0)
        btn_up[5]->setIcon(QPixmap(":/2.1.PNG"));
    else if(mod == 1)
        btn_up[5]->setIcon(QPixmap(":/2.2.PNG"));
    rbtn_codemod[mod]->setChecked(true);
}

/*
** 函数功能：当前工具、基座标发生变化时需要进行的更新操作
** 函数参数：mod：当前程序运行模式
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_UpdateTB(int indext, int indexb, int ipo)
{
    KukaRob.indext = indext;
    KukaRob.indexb = indexb;
    KukaRob.ipo = ipo;
    lab_icon[4]->setText(tr("W")+QString::number(indext));
    lab_icon[5]->setText(tr("B")+QString::number(indexb));
    cb_tool->setCurrentIndex(indext);
    cb_base->setCurrentIndex(indexb);
    if(ipo == 0)
        lab_icon[3]->setPixmap(QPixmap("://3.1.PNG"));
    else if(ipo == 1)
        lab_icon[3]->setPixmap(QPixmap("://3.2.PNG"));
    else
        return;
    rbtn_ipo[ipo]->setChecked(true);
}

/*
** 函数功能：当前增量发生变化时需要进行的更新操作
** 函数参数：incre：当前增量
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_UpdateIncre(int incre)
{
    switch (incre) {
    case 0:
        lab_icon[7]->setText("∞");
        break;
    case 1:
        lab_icon[7]->setText("100 mm\n  10°");
        break;
    case 2:
        lab_icon[7]->setText("10 mm\n  3°");
        break;
    case 3:
        lab_icon[7]->setText("1 mm\n  1°");
        break;
    case 4:
        lab_icon[7]->setText("0.1 mm\n  0.005°");
        break;
    default:
        break;
    }
    KukaRob.Incre = incre;
    btn_incre[incre]->setChecked(true);
}

/*
** 函数功能：当前摇杆下运动模式发生变化时需要进行的更新操作
** 函数参数：mod：当前摇杆下的运动模式
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_UpdateMActmod(int mod)
{
    if(mod < 0 || mod > 3)
        return;
    KukaRob.Cor_Mouse = mod;
    rbtn_mouse[mod]->setChecked(true);
    QString icon[4]={":/image/image/mouse1.png",":/image/image/mouse2.png",\
                                 ":/image/image/mouse3.png",":/image/image/mouse4.png"};
    btn_right[0]->setIcon(QIcon(icon[mod]));
    /*更新标签上的文字*/
    QString strJU[6]={":/image/image/A1U.png",":/image/image/A2U.png",":/image/image/A3U.png",\
                                  ":/image/image/A4U.png",":/image/image/A5U.png",":/image/image/A6U.png"};
    QString strU[6]={":/image/image/AU.png",":/image/image/BU.png",":/image/image/CU.png",\
                                  ":/image/image/XU.png",":/image/image/YU.png",":/image/image/ZU.png"};
    if(mod == 0)
    {
        for(int i = 0 ; i < 6 ; i++)
            lab_right[i]->setPixmap(QPixmap(strJU[i]));
        flag_mod = 0;
    }
    else
    {
        for(int i = 0 ; i < 6 ; i++)
            lab_right[i]->setPixmap(QPixmap(strU[i]));
        flag_mod = 1;
    }
}

/*
** 函数功能：当前按键下运动模式发生变化时需要进行的更新操作
** 函数参数：mod：当前摇杆下的运动模式
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_UpdateKActmod(int mod)
{
    if(mod < 0 || mod > 3)
        return;
    KukaRob.Cor_Key = mod;
    rbtn_key[mod]->setChecked(true);
    QString icon[4]={":/image/image/mouse1.png",":/image/image/mouse2.png",\
                                 ":/image/image/mouse3.png",":/image/image/mouse4.png"};
    btn_right[2]->setIcon(QIcon(icon[mod]));
    /*更新标签上的文字*/
    QString strJU[6]={":/image/image/A1U.png",":/image/image/A2U.png",":/image/image/A3U.png",\
                                  ":/image/image/A4U.png",":/image/image/A5U.png",":/image/image/A6U.png"};
    QString strU[6]={":/image/image/AU.png",":/image/image/BU.png",":/image/image/CU.png",\
                                  ":/image/image/XU.png",":/image/image/YU.png",":/image/image/ZU.png"};
    if(mod == 0)
    {
        for(int i = 0 ; i < 6 ; i++)
            lab_right[i]->setPixmap(QPixmap(strJU[i]));
        flag_mod = 0;
    }
    else
    {
        for(int i = 0 ; i < 6 ; i++)
            lab_right[i]->setPixmap(QPixmap(strU[i]));
        flag_mod = 1;
    }
    if(mod == 0)    //轴
    {
        NcktCmd(&xImdT.aImdT , NCK_COOR , NCK_COOR_JNT , NULL);
        xImdT.flag = 1;
    }
    else if(mod == 1)   //全局
    {
        NcktCmd(&xImdT.aImdT , NCK_COOR , NCK_COOR_RECT , NULL);
        xImdT.flag = 1;
    }
    else if(mod == 2)   //基座标
    {
        NcktCmd(&xImdT.aImdT , NCK_COOR , NCK_COOR_USER , NULL);
        xImdT.flag = 1;
    }
    else if(mod == 3)   //工具
    {
        NcktCmd(&xImdT.aImdT , NCK_COOR , NCK_COOR_TOOL , NULL);
        xImdT.flag = 1;
    }
}

/*
** 函数功能：电机上电时更新右侧6个轴标签颜色变化
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_UpdateRobState()
{
    QString strJU[6]={":/image/image/A1U.png",":/image/image/A2U.png",":/image/image/A3U.png",\
                              ":/image/image/A4U.png",":/image/image/A5U.png",":/image/image/A6U.png"};
    QString strJE[6]={":/image/image/A1E.png",":/image/image/A2E.png",":/image/image/A3E.png",\
                              ":/image/image/A4E.png",":/image/image/A5E.png",":/image/image/A6E.png"};

    QString strU[6]={":/image/image/AU.png",":/image/image/BU.png",":/image/image/CU.png",\
                              ":/image/image/XU.png",":/image/image/YU.png",":/image/image/ZU.png"};
    QString strE[6]={":/image/image/AE.png",":/image/image/BE.png",":/image/image/CE.png",\
                              ":/image/image/XE.png",":/image/image/YE.png",":/image/image/ZE.png"};

    if(KukaRob.Motor == 0)
    {
        if(flag_mod == 0)  //关节运动
        {
            for(int i = 0 ; i < 6 ; i++)
                lab_right[i]->setPixmap(QPixmap(strJU[i]));
        }
        else
        {
            for(int i = 0 ; i < 6 ; i++)
                lab_right[i]->setPixmap(QPixmap(strU[i]));
        }
    }
    else if(KukaRob.Motor == 1 && KukaRob.EMG == 1)
    {
        if(flag_mod == 0)  //关节运动
        {
            for(int i = 0 ; i < 6 ; i++)
                lab_right[i]->setPixmap(QPixmap(strJE[i]));
        }
        else
        {
            for(int i = 0 ; i < 6 ; i++)
                lab_right[i]->setPixmap(QPixmap(strE[i]));
        }
    }
}

/*
** 函数功能：当前各类报警信息数目发生变化时需要进行的更新操作
** 函数参数：mod：当前摇杆下的运动模式
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::func_UpdateAlarm(int alarm[4])
{
    for(int i = 0 ; i < 4 ; i++)
        lab_alarm[i]->setText(QString::number(alarm[i]));
    for(int i = 0 ; i < 4 ; i++)
        lab_leftb[i]->setText(QString::number(alarm[i]));
    memcpy(KukaRob.alarm,alarm,16);

    if( alarm[0] > 0 )
        timer_err->start(600);
    else if( alarm[0] == 0)
    {
        timer_err->stop();
        lab_leftd[0]->show();
    }
}

/*
** 函数功能：工具栏上8个按钮按下槽函数
** 函数参数：id：按下按钮id
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::slot_BtnGClicked(bool state)
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if( !btn )
        return;
    if( btn == btn_up[0])
    {
        if(state)
            func_showSubmit();
        else
            func_hideSubmit();
    }
    else if( btn == btn_up[1])
    {
        if(state)
            func_showDriver();
        else
            func_hideDriver();
    }
    else if( btn == btn_up[2])
    {
        if(state)
            func_showCode();
        else
            func_hideCode();
    }
    else if( btn == btn_up[3])
    {
        if(state)
            ;
        else
            ;
    }
    else if( btn == btn_up[4])
    {
        if(state)
            func_showSpd(KukaRob.Spd_code,KukaRob.Spd_hand);
        else
            func_hideSpd();
    }
    else if( btn == btn_up[5])
    {
        if(state)
            func_showMod(KukaRob.CodeMod);
        else
            func_hideMod();
    }
    else if( btn == btn_up[6])
    {
        if(state)
            func_showTB(KukaRob.indext,KukaRob.indexb,KukaRob.ipo);
        else
            func_hideTB();
    }
    else if( btn == btn_up[7])
    {
        if(state)
            func_showIncre(KukaRob.Incre);
        else
            func_hideIncre();
    }
}

/*
** 函数功能：右侧栏上三个按钮按下槽函数
** 函数参数：id：按钮id
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::slot_BtnG1Clicked(bool state)
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if( !btn )
        return;
    if( btn == btn_right[0])
    {
        if(state)
            func_showMActmod(KukaRob.Cor_Mouse);
        else
            func_hideMActmod();
    }
    else if( btn == btn_right[2])
    {
        if(state)
            func_showKActmod(KukaRob.Cor_Key);
        else
            func_hideKActmod();
    }
}

/*
** 函数功能：程序运行模式按钮按下槽函数
** 函数参数：id：按钮id
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::slot_BtnGCodeModCliked(int id)
{
    func_UpdateCMod(id);
    frame_upmenu[5]->hide();
}

/*
** 函数功能：ipo选择按钮按下槽函数
** 函数参数：id：按钮id
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::slot_btnGIPOClicked(int ipo)
{
    func_UpdateTB(KukaRob.indext,KukaRob.indexb,ipo);
    frame_upmenu[6]->hide();
}

/*
** 函数功能：增量选择按钮按下槽函数
** 函数参数：id：按钮id
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::slot_BtnGIncre(int id)
{
    func_UpdateIncre(id);
    frame_upmenu[7]->hide();
}

/*
** 函数功能：摇杆运动模式下按钮按下槽函数
** 函数参数：id：按钮id
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::slot_BtnGMCor(int id)
{
    func_UpdateMActmod(id);
    frame_rightmenu[0]->hide();
}

/*
** 函数功能：按键运动模式下按钮按下槽函数
** 函数参数：id：按钮id
** 函数返回值：无
** 函数说明：无
*/
void MenuBar::slot_BtnGKCor(int id)
{
    func_UpdateKActmod(id);
    frame_rightmenu[2]->hide();
}

void MenuBar::slot_BtnClicked(bool state)
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if( !btn )
        return;
    if( btn == frame_left)
    {
        if(state)
            func_showAlarm(KukaRob.alarm);
        else
            func_hideAlarm();
    }
    else if( btn == btn_code[0])
    {
        emit Sig_CodeDeselect();
        frame_upmenu[2]->hide();
    }
    else if( btn == btn_code[1])
    {
        emit Sig_CodeReset();
        frame_upmenu[2]->hide();
    }
    /*程序和手动调节量*/
    else if( btn ==btn_spd[0] )
    {
        if(KukaRob.Spd_code < 5)
            return;
        KukaRob.Spd_code -= 5;
        func_UpdateSpd(KukaRob.Spd_hand , KukaRob.Spd_code);
    }
    else if( btn ==btn_spd[1] )
    {
        if(KukaRob.Spd_code >95)
            return;
        KukaRob.Spd_code += 5;
        func_UpdateSpd(KukaRob.Spd_hand , KukaRob.Spd_code);
    }
    else if( btn ==btn_spd[2] )
    {
        if(KukaRob.Spd_hand < 5)
            return;
        KukaRob.Spd_hand -= 5;
        func_UpdateSpd(KukaRob.Spd_hand , KukaRob.Spd_code);
    }
    else if( btn ==btn_spd[3] )
    {
        if(KukaRob.Spd_hand > 95)
            return;
        KukaRob.Spd_hand += 5;
        func_UpdateSpd(KukaRob.Spd_hand , KukaRob.Spd_code);
    }
    /*选择程序运行方式*/
    else if( btn == btn_codemod[0] )
    {
        func_UpdateCMod(0);
        frame_upmenu[5]->hide();
    }
    else if( btn == btn_codemod[1] )
    {
        func_UpdateCMod(1);
        frame_upmenu[5]->hide();
    }
    /*选择IPO*/
    else if( btn == btn_ipo[0])
    {
        func_UpdateTB(KukaRob.indext,KukaRob.indexb,0);
        frame_upmenu[6]->hide();
    }
    else if( btn == btn_ipo[1])
    {
        func_UpdateTB(KukaRob.indext,KukaRob.indexb,1);
        frame_upmenu[6]->hide();
    }
    /*选择6D摇杆的运动模式*/
    else if( btn == btn_mouse[0] )
    {
        func_UpdateMActmod(0);
        frame_rightmenu[0]->hide();
    }
    else if( btn == btn_mouse[1] )
    {
        func_UpdateMActmod(1);
        frame_rightmenu[0]->hide();
    }
    else if( btn == btn_mouse[2] )
    {
        func_UpdateMActmod(2);
        frame_rightmenu[0]->hide();
    }
    else if( btn == btn_mouse[3] )
    {
        func_UpdateMActmod(3);
        frame_rightmenu[0]->hide();
    }
    /*选择按键的运动模式*/
    else if( btn == btn_key[0])
    {
        func_UpdateKActmod(0);
        frame_rightmenu[2]->hide();
    }
    else if( btn == btn_key[1])
    {
        func_UpdateKActmod(1);
        frame_rightmenu[2]->hide();
    }
    else if( btn == btn_key[2])
    {
        func_UpdateKActmod(2);
        frame_rightmenu[2]->hide();
    }
    else if( btn == btn_key[3])
    {
        func_UpdateKActmod(3);
        frame_rightmenu[2]->hide();
    }
}

void MenuBar::slot_Timerout()
{
    if(lab_leftd[0]->isHidden())
        lab_leftd[0]->show();
    else
        lab_leftd[0]->hide();
}

void MenuBar::slot_Slider(int val)
{
    int code = slider[0]->value();
    int hand = slider[1]->value();
    lab_spd[0]->setText(QString::number(code)+"%");
    lab_spd[1]->setText(QString::number(hand)+"%");
    lab_icon[1]->setText(QString::number(code));
    lab_icon[2]->setText(QString::number(hand));
    KukaRob.Spd_hand = hand;
    KukaRob.Spd_code = code;
}

void MenuBar::slot_CbClicked(int index)
{
    QComboBox *cb = qobject_cast<QComboBox*>(sender());
    if( !cb )
        return;
    if( cb == cb_tool)    //选择工具坐标系
    {
        KukaRob.CurTool = codedata[14][index];
        KukaRob.indext = index;
    }
    else if( cb == cb_base )   //选择基座标系
    {
        KukaRob.CurBase = codedata[12][index];
        KukaRob.indexb = index;
    }
    func_UpdateTB(KukaRob.indext , KukaRob.indexb , KukaRob.ipo);
}

void MenuBar::slot_aboutToHide()
{
    MenuWidget *wid = qobject_cast<MenuWidget*>(sender());
    if( !wid )
        return;
    if(wid == frame_alarm)
    {
        frame_left->setChecked(false);
        return;
    }
    for(int i = 0 ; i < 8 ; i++)
    {
        if(wid == frame_upmenu[i])
        {
            btn_up[i]->setChecked(false);
            return;
        }
    }
    for(int i = 0 ; i < 3 ; i++)
    {
        if(wid == frame_rightmenu[i])
        {
            btn_right[i]->setChecked(false);
            return;
        }
    }
}

void MenuBar::slot_aboutToShow(MenuWidget *wid)
{
    if(wid == frame_alarm)
    {
        for(int i = 0 ; i < 8 ; i++)
        {
            if(frame_upmenu[i] != NULL)
                frame_upmenu[i]->hide();
        }
        return;
    }
    for(int i = 0 ; i < 8 ; i++)
    {
        if(wid != frame_upmenu[i])
        {
            if(frame_upmenu[i] != NULL)
                frame_upmenu[i]->hide();
        }
    }
    for(int i = 0 ; i < 3 ; i++)
    {
        if(wid != frame_rightmenu[i])
        {
            if(frame_rightmenu[i] != NULL)
                frame_rightmenu[i]->hide();
        }
    }
    if(frame_alarm != NULL)
        frame_alarm->hide();
}
