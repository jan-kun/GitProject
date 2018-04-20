#include "power.h"
#include<QDebug>
Power::Power(QWidget *parent)
    :QWidget(parent)
{
    init_frame();
}

void Power::init_frame()
{
    frame_base = new QFrame(this);
    frame_base->setGeometry(0,0,494,694);
    frame_base->setStyleSheet("background-color:rgb(230,230,230)");
    frame_base->show();
    frame_base->raise();

    QLabel *lab_tip = new QLabel(frame_base);
    lab_tip->setGeometry(0,0,494,30);
    lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
    lab_tip->setText("  关机");
    lab_tip->show();

    QLabel *lab[7];
    for(int i = 0 ; i < 7 ; i++)
    {
        lab[i] = new QLabel(frame_base);
        lab[i]->show();
    }
    lab[0]->setText(tr("用主开关关断的标准设定"));
    lab[0]->setGeometry(20,35,240,20);

    lab[1]->setText("启动类型");
    lab[1]->setGeometry(40,60,120,20);

    bGroup_Stype=new QButtonGroup(frame_base);
    for(int i = 0 ; i < 5 ; i++)
    {
        btn_option[i] = new QPushButton(frame_base);
        btn_option[i]->show();
        connect(btn_option[i],SIGNAL(clicked()),this,SLOT(slot_btnClicked()));
    }
    btn_option[0]->setGeometry(60,85,100,40);
    btn_option[1]->setGeometry(180,85,100,40);

    QString stp[2] = {"冷启动","休眠"};
    for(int i = 0 ; i < 2 ; i++)
    {
        rBtn[i] = new QRadioButton(btn_option[i]);
        rBtn[i]->move(5,8);
        rBtn[i]->show();
        rBtn[i]->setText(stp[i]);
        bGroup_Stype->addButton(rBtn[i],i);
    }
    connect(bGroup_Stype,SIGNAL(buttonClicked(int)),this,SLOT(slot_rBtnClicked(int)));

    lab[2]->setText("Power-off 等待时间");
    lab[2]->setGeometry(40,130,240,20);

    btn_del=new QPushButton(frame_base);
    btn_del->setText("--");
    btn_del->setGeometry(60,155,60,40);
    btn_del->show();
    connect(btn_del,SIGNAL(clicked()),this,SLOT(slot_btnClicked()));

    btn_add=new QPushButton(frame_base);
    btn_add->setText("+");
    btn_add->setGeometry(140,155,60,40);
    btn_add->show();
    connect(btn_add,SIGNAL(clicked()),this,SLOT(slot_btnClicked()));

    lab_time=new QLabel(frame_base);
    lab_time->setGeometry(220,155,45,30);
    lab_time->setText(tr("1[s]"));
    lab_time->show();

    slider_time=new QSlider(Qt::Horizontal,frame_base);
    slider_time->setGeometry(270,155,200,30);
    slider_time->setStyleSheet("QSlider::handle:horizontal{border: 1px solid black;width:60px;height:50px;} QSlider::groove:horizontal{background:Silver;height: 35px}");
    slider_time->setMinimum(1);
    slider_time->setMaximum(300);
    slider_time->show();
    connect(slider_time,SIGNAL(valueChanged(int)),this,SLOT(slot_SliderValueChg(int)));

    lab[3]->setText("只在下次用主开关关断时适用的设定");
    lab[3]->setGeometry(20,205,250,20);

    btn_option[2]->setGeometry(60,230,170,40);
    btn_option[3]->setGeometry(60,275,170,40);
    btn_option[4]->setGeometry(60,320,170,40);

    QString str[3] = {"强制冷启动","重新读入文件","Power-off等待时间"};
    for(int i= 0 ; i < 3 ; i++)
    {
        cBtn[i] = new QCheckBox(btn_option[i+2]);
        cBtn[i]->move(5,7);
        cBtn[i]->setText(str[i]);
        cBtn[i]->show();
    }

    lab[4]->setText("立即执行的操作");
    lab[4]->setGeometry(20,370,120,20);

    lab[5]->setText("关断选项");
    lab[5]->setGeometry(40,395,80,20);

    btn_closePC = new QPushButton("关闭控制系统PC",frame_base);
    btn_closePC->setGeometry(60,420,150,40);
    btn_closePC->show();
    connect(btn_closePC,SIGNAL(clicked()),this,SLOT(slot_btnClicked()));
    btn_restartPC = new QPushButton("重新启动控制系统PC",frame_base);
    btn_restartPC->setGeometry(230,420,150,40);
    btn_restartPC->show();
    connect(btn_restartPC,SIGNAL(clicked()),this,SLOT(slot_btnClicked()));

    lab[6]->setText("驱动总线");
    lab[6]->setGeometry(40,465,100,20);

    btn_light = new QPushButton(frame_base);
    btn_light->setGeometry(60,490,40,50);
    btn_light->show();

    btn_i = new QPushButton("I",frame_base);
    btn_i->setGeometry(110,490,80,50);
    btn_i->show();
    connect(btn_i,SIGNAL(clicked()),this,SLOT(slot_btnClicked()));

    btn_o = new QPushButton("O",frame_base);
    btn_o->setGeometry(210,490,80,50);
    btn_o->show();
    connect(btn_o,SIGNAL(clicked()),this,SLOT(slot_btnClicked()));
}

void Power::func_setWaitTime(int v)
{
    time=v;
}

int Power::func_getWaitTime()
{
    return time;
}

void Power::slot_btnClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(!btn)
        return;
    if(btn == btn_option[0])
        rBtn[0]->setChecked(true);
    else if(btn == btn_option[1])
        rBtn[1]->setChecked(true);
    else if(btn == btn_option[2])
        cBtn[0]->setChecked(true);
    else if(btn == btn_option[3])
        cBtn[1]->setChecked(true);
    else if(btn == btn_option[4])
        cBtn[2]->setChecked(true);
    else if(btn == btn_del)
    {

    }
    else if(btn == btn_add)
    {

    }
    else if(btn == btn_closePC)
    {

    }
    else if(btn == btn_restartPC)
    {

    }
    else if(btn == btn_i)
    {

    }
    else if(btn == btn_o)
    {

    }
}

void Power::slot_rBtnClicked(int id)
{
    if( id == 0)
    {

    }
    else if(id == 1)
    {

    }
}


void Power::slot_cBtnClicked()
{

}

void Power::slot_SliderValueChg(int v)
{
    QString s="[s]";
    QString t=QString::number(v)+s;
    lab_time->setText(t);
    func_setWaitTime(v);
}
