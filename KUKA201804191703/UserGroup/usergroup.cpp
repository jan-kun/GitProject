#include "usergroup.h"

UserGroup::UserGroup(QWidget *parent) :
    QWidget(parent)
{
}

void UserGroup::init_frame()
{
    if(frame_log == NULL)
    {
        frame_log = new QFrame(this);
        frame_log->setGeometry(0,0,494,694);
        frame_log->setStyleSheet("background-color:rgb(230,230,230)");
        frame_log->show();
        frame_log->raise();

        QLabel *lab_temp[4];
        lab_temp[0] = new QLabel(frame_log);
        lab_temp[0]->setGeometry(0,0,494,30);
        lab_temp[0]->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
        lab_temp[0]->setText("  登录");

        QFrame *frame_info = new QFrame(frame_log);
        frame_info->setGeometry(15,45,464,120);
        frame_info->setFrameShape(QFrame::StyledPanel);
        frame_info->setFrameShadow(QFrame::Raised);
        frame_info->show();

        lab_temp[1] = new QLabel(" 登录信息",frame_log);
        lab_temp[1]->setGeometry(40,38,70,20);
        lab_temp[1]->show();

        lab_user = new QLabel(frame_info);  //当前用户
        lab_user->setGeometry(25,25,300,20);
        lab_user->show();
        lab_user->setStyleSheet("color:black;font-size:16px");

        lab_temp[2] = new QLabel(tr("登录日期："),frame_info);
        lab_temp[2]->setGeometry(25,50,100,20);
        lab_temp[2]->show();

        lab_time = new QLabel(frame_info);  //登录时间
        lab_time->setGeometry(150,50,300,20);
        lab_time->setStyleSheet("color:black;font-size:16px");
        lab_time->show();

        lab_temp[3]=new QLabel(frame_log);
        lab_temp[3]->setGeometry(25,500,520,90);
        lab_temp[3]->setText(tr("登录,以切换当前用户。未存档的数据可能会丢失。\n\n按下<标准>，以作为标准用户登录\n按下<登录>，以登录。"));
        lab_temp[3]->show();

        for(int i=0;i<7;i++)
        {
            btn_loginctl[i]=new QPushButton(frame_log);
            btn_loginctl[i]->setGeometry(2+70*i,650,68,42);
            btn_loginctl[i]->setFocusPolicy(Qt::NoFocus);
            btn_loginctl[i]->setEnabled(false);
            btn_loginctl[i]->show();
            btn_loginctl[i]->setIconSize(QSize(68,42));
            btn_loginctl[i]->setIcon(QIcon(":/image/image/btn_kb.png"));
        }
        btn_loginctl[0]->setEnabled(true);
        btn_loginctl[0]->setIcon(QIcon(":/image/image/btn_operator.png"));
        connect(btn_loginctl[0],SIGNAL(clicked(bool)),this,SLOT(slot_btnClicked()));
        btn_loginctl[5]->setEnabled(true);
        btn_loginctl[5]->setIcon(QIcon(":/image/image/btn_login.png"));
        connect(btn_loginctl[5],SIGNAL(clicked(bool)),this,SLOT(slot_btnClicked()));
    }
    frame_log->show();
    frame_log->raise();
    switch (KukaRob.User) {
    case Proger:
        lab_user->setText("Programmer 已登录。");
        break;
    case Expert:
        lab_user->setText("Expert 已登录。");
        break;
    default:
        lab_user->setText("标准用户（Operator） 已登录。");
        break;
    }
}

void UserGroup::frame_login()
{
    if(frame_user == NULL)
    {
        frame_user = new QFrame(this);
        frame_user->setGeometry(0,0,494,694);
        frame_user->setStyleSheet("background-color:rgb(230,230,230)");
        frame_user->show();
        frame_user->raise();

        QLabel *lab_temp[4];
        lab_temp[0] = new QLabel(frame_user);
        lab_temp[0]->setGeometry(0,0,494,30);
        lab_temp[0]->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
        lab_temp[0]->setText(" 通过选择登录");
        lab_temp[0]->show();

        lab_temp[1] = new QLabel(frame_user);
        lab_temp[1]->setGeometry(20,50,200,30);
        lab_temp[1]->setText("选择用户：");
        lab_temp[1]->show();

        tab_user = new QTableWidget(5,1,frame_user);
        tab_user->setGeometry(20,85,454,120);
        tab_user->show();
        tab_user->horizontalHeader()->setVisible(false);
        tab_user->horizontalHeader()->setDefaultSectionSize(450);
        tab_user->verticalHeader()->setVisible(false);
        tab_user->verticalHeader()->setDefaultSectionSize(40);
        tab_user->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tab_user->setShowGrid(false);
        tab_user->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tab_user->setSelectionMode(QAbstractItemView::SingleSelection);  //设置只能单个选中
        tab_user->setStyleSheet("selection-background-color: rgb(34,187,255);border: 1px solid black");
        QString text[5]={"Programmer","Expert","Safety Recovery","Safety Maintenance","Administrator"};
        for(int i = 0 ; i < 5 ; i++)
            tab_user->setItem(i,0,new QTableWidgetItem(text[i]));
        connect(tab_user,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tabClicked()));

        lab_pass = new QLabel("密码：",frame_user);
        lab_pass->setGeometry(20,210,100,20);
        lab_pass->hide();

        line_pass = new QLineEdit(frame_user);
        line_pass->setGeometry(20,235,454,30);
        line_pass->hide();  //输入密码框，选择用户后方才显示出来

        lab_temp[2] = new QLabel(frame_user);
        lab_temp[2]->setGeometry(20,500,500,30);
        lab_temp[2]->setText("登录，以切换当前用户，为存档的数据可能会丢失。");
        lab_temp[2]->show();

        lab_temp[3] = new QLabel(frame_user);
        lab_temp[3]->setGeometry(20,570,500,50);
        lab_temp[3]->setText("按下 <标准>，以作为标准用户登录。\n按下 <登录>，以登录。");
        lab_temp[3]->show();

        for(int i = 0 ; i < 7 ; i++)
        {
            btn_userctl[i] = new QPushButton(frame_user);
            btn_userctl[i]->setGeometry(2+70*i,650,68,42);
            btn_userctl[i]->setFocusPolicy(Qt::NoFocus);
            btn_userctl[i]->setEnabled(false);
            btn_userctl[i]->show();
            btn_userctl[i]->setIconSize(QSize(68,42));
            btn_userctl[i]->setIcon(QIcon(":/image/image/btn_kb.png"));
            connect(btn_userctl[i],SIGNAL(clicked()),this,SLOT(slot_btnClicked()));
        }
        btn_userctl[0]->setEnabled(true);
        btn_userctl[0]->setIcon(QIcon(":/image/image/btn_operator.png"));
        btn_userctl[1]->setEnabled(true);
        btn_userctl[1]->setIcon(QIcon(":/image/image/btn_code.png"));
        btn_userctl[5]->setEnabled(true);
        btn_userctl[5]->setIcon(QIcon(":/image/image/btn_login.png"));
    }
    frame_user->show();
    frame_user->raise();
    lab_pass->hide();
    line_pass->hide();
    tab_user->selectRow(0);
}

/*
** 函数功能：用户登录
** 函数参数：user，用户名；passcode，密码
** 函数返回值：成功，1；失败，-1
** 函数说明：无
*/
int UserGroup::func_userlogin(QString user, QString passcode)
{
    if(passcode != "kuka")
    {
        emit sig_mimaerr();
        return -1;
    }
    if(user == "Operator")
        KukaRob.User = Operator;
    else if(user == "Programmer")
        KukaRob.User = Proger;
    else if(user == "Expert")
        KukaRob.User = Expert;
    else if(user == "Safety Recovery")
        KukaRob.User = Recv;
    else if(user == "Safety Maintenance")
        KukaRob.User = Maint;
    else if(user == "Administrator")
        KukaRob.User = Admin;
    else
        return -1;
    if(KukaRob.User == Operator)
        flag_codeeditor = 0 ;
    else
        flag_codeeditor = 1;
    emit sig_userlogin(KukaRob.User);
    return 1;
}

void UserGroup::slot_btnClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if( !btn )
        return;
    if(btn == btn_loginctl[0])     //作为标准用户登录
    {
        if(func_userlogin("Operator","kuka") > 0)
            this->hide();
    }
    else if(btn == btn_loginctl[5])   //登录
        frame_login();
    else if(btn == btn_userctl[0])   //作为标准用户登录
    {
        if(func_userlogin("Operator","kuka") > 0)
            this->hide();
    }
    else if(btn == btn_userctl[1])
        return;
    else if(btn == btn_userctl[5])    //登录
    {
        if(tab_user->currentItem()->row() < 0)
            return;
        if(line_pass->isHidden())
            return;
        QString user = tab_user->currentItem()->text();
        QString pass = line_pass->text();
        if(func_userlogin(user,pass) > 0)
            this->hide();
    }
}

void UserGroup::slot_tabClicked()
{
    lab_pass->show();
    line_pass->show();
}



