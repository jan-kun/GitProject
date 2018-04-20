#include "sigvar.h"

SigVar::SigVar(QWidget *parent) :
    QWidget(parent)
{
}

void SigVar::init_frame()
{
    if(frame_base == NULL)
    {
        frame_base = new QFrame(this);
        frame_base->setGeometry(0,0,494,694);
        frame_base->setObjectName("fb");
        frame_base->setStyleSheet("#fb{background-color:rgb(230,230,230)}");
        frame_base->show();
        frame_base->raise();

        QLabel *lab_temp[5];
        lab_temp[0] = new QLabel(frame_base);
        lab_temp[0]->setGeometry(0,0,494,30);
        lab_temp[0]->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
        lab_temp[0]->setText("  单项变量显示");

        lab_temp[1]=new QLabel(tr("名称:"),frame_base);
        lab_temp[1]->setGeometry(10,40,50,30);
        lab_temp[1]->show();

        cb_var[0] = new QComboBox(frame_base);
        cb_var[0]->setGeometry(10,70,200,50);
        cb_var[0]->show();

        lab_temp[2]=new QLabel(tr("新值:"),frame_base);
        lab_temp[2]->setGeometry(10,130,50,30);
        lab_temp[2]->show();

        cb_var[1] = new QComboBox(frame_base);
        cb_var[1]->setGeometry(10,160,200,50);
        cb_var[1]->show();

        lab_temp[3]=new QLabel(tr("模块:"),frame_base);
        lab_temp[3]->setGeometry(10,220,50,30);
        lab_temp[3]->show();

        cb_var[2] = new QComboBox(frame_base);
        cb_var[2]->setGeometry(10,250,200,50);
        cb_var[2]->show();

        lab_temp[4] = new QLabel(tr("当前值:"),frame_base);
        lab_temp[4]->setGeometry(220,40,80,30);
        lab_temp[4]->show();

        lab_show = new QLabel(frame_base);
        lab_show->setGeometry(220,70,200,280);
        lab_show->show();
        lab_show->setFrameShape(QFrame::StyledPanel);
        lab_show->setFrameShadow(QFrame::Raised);

        for(int i=0 ; i<6 ; i++)
        {
            btn_var[i]=new QPushButton(frame_base);
            btn_var[i]->setGeometry(425,40+45*i,63,40);
            btn_var[i]->setFocusPolicy(Qt::NoFocus);
            btn_var[i]->show();
            btn_var[i]->setEnabled(false);
            btn_var[i]->setIconSize(QSize(68,42));
            connect(btn_var[i],SIGNAL(clicked()),this,SLOT(slot_btnClicked()));
        }
        btn_var[0]->setEnabled(true);
        btn_var[0]->setText(tr("刷新"));
        btn_var[1]->setText(tr("设定值"));
    }
    frame_base->show();
    frame_base->raise();
}

void SigVar::slot_btnClicked()
{

}
