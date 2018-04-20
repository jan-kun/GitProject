#include "updatesys.h"

UpdateSys::UpdateSys(QWidget *parent) :
    QWidget(parent)
{
    frame_base = new QFrame(this);
    frame_base->setGeometry(0,0,494,694);
    frame_base->setStyleSheet("background-color:rgb(230,230,230)");
    frame_base->show();
    frame_base->raise();
}

void UpdateSys::frame_updateSys()
{
    if(frame_size != NULL)
        frame_size->hide();
    if(frame_update == NULL)
    {
        frame_update = new QFrame(frame_base);
        frame_update->setGeometry(0,0,494,694);
        frame_update->setStyleSheet("background-color: rgb(255,255,255);");
        frame_update->show();
        frame_update->raise();

        QLabel *lab_tip = new QLabel(frame_update);
        lab_tip->setGeometry(0,0,494,30);
        lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
        lab_tip->setText("  系统升级");
        lab_tip->show();

        QLabel *lab_tip1 = new QLabel(frame_update);
        lab_tip1->setGeometry(150,150,200,30);
        lab_tip1->setText("系统升级，请输入密码");
        lab_tip1->show();

        line_mima = new QLineEdit(frame_update);
        line_mima->setGeometry(150,200,150,30);
        line_mima->show();

        btn_updatefunc = new QPushButton("确定",frame_update);
        btn_updatefunc->setGeometry(200,280,60,30);
        btn_updatefunc->show();
        connect(btn_updatefunc,SIGNAL(clicked()),this,SLOT(slot_btn()));
    }
    frame_update->show();
    frame_update->raise();
    line_mima->clear();
}

int UpdateSys::func_mountdisk()
{
    /*先判断是否已经挂载上了U盘*/
    int res;
    QDir jdir("/run/media/sda1");
    QDir mdir(KUKA_USB);
    if(jdir.count() == 2 && mdir.count() == 2)   //没发现U盘
        res = 0;
    else    //发现U盘
        res = 1;
    if(res == 1)
    {
        if(mdir.count() != 2)    //已经挂载上usb目录中
            return 1;
        else    //已经挂载上media目录中
        {
            QStringList umountargu;
            umountargu<<"/run/media/sda1";
            int res1 = QProcess::execute("umount",umountargu);
            if(res1 == 0)    //卸载U盘成功
            {
                QStringList mountargu;
                mountargu<<"/dev/sda1"<<KUKA_USB;
                int res2 = QProcess::execute("mount",mountargu);
                if(res2 == 0)   return 1;
                else                return -1;
            }
            else    return -1;   //卸载U盘失败
        }
    }
    else
    {
        QStringList mountargu;
        mountargu<<"/dev/sda1"<<KUKA_USB;
        int res2 = QProcess::execute("mount",mountargu);
        if(res2 == 0)   return 1;
        else                return -1;
    }
}

void UpdateSys::updatesys(QString mima)
{
    int res = func_mountdisk();
    if(res < 0 )
    {
        QMessageBox::warning(this,"升级系统","升级系统失败！未找到可移动磁盘","确定");
        return;
    }
    QString source = KUKA_USB+tr("/KUKA_USER");
    QString dest = "/home/root";
    if(mima == "123456")
    {
        QStringList arguments;
        arguments<<"-rf"<<source<<dest;

        int res = QProcess::execute("cp",arguments);
        if(res != 0)
        {
            QMessageBox::warning(this,"升级系统","升级系统失败！未找到可升级程序","确定");
            return;
        }
        int resss = QProcess::execute("sync");
        if(resss != 0)
        {
            QMessageBox::warning(this,"升级系统","sync fail","确定");
            return;
        }
        int res1 = QProcess::execute("reboot");
        if(res1 != 0)
        {
            QMessageBox::warning(this,"升级系统","重启系统失败，请手动重启","确定");
            return;
        }
    }
    else
    {
        QMessageBox::warning(this,"升级失败","密码错误，请重试","确定");
        return;
    }
}

void UpdateSys::frame_setRobSize()
{
    if(frame_update != NULL)
        frame_update->hide();
    if(frame_size == NULL)
    {
        frame_size = new QFrame(frame_base);
        frame_size->setGeometry(0,0,494,694);
        frame_size->setObjectName("fsize");
        frame_size->setStyleSheet("#fsize{background-color:white;}");
        frame_size->show();
        frame_size->raise();

        QLabel *lab_tip = new QLabel(frame_update);
        lab_tip->setGeometry(0,0,494,30);
        lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
        lab_tip->setText("  设置机器人尺寸参数");
        lab_tip->show();

        QLabel *lab_tip1 = new QLabel(frame_size);
        lab_tip1->setGeometry(150,150,200,30);
        lab_tip1->show();
        lab_tip1->setText("请输入机器人的尺寸参数");

        QLabel *lab_size[6];
        QString namelab[6] = {"L1:","L2:","L3:","L4:","L5:","L6:"};
        for(int i = 0 ; i < 6 ; i++)
        {
            lab_size[i] = new QLabel(frame_size);
            lab_size[i]->setGeometry(150,200+40*i,100,30);
            lab_size[i]->show();
            lab_size[i]->setText(namelab[i]);
        }
        validator = new QDoubleValidator(this);
        for(int i = 0 ;i < 6 ; i++)
        {
            line_size[i] = new QLineEdit(frame_size);
            line_size[i]->setGeometry(250,200+40*i,150,30);
            line_size[i]->show();
            line_size[i]->setStyleSheet("color:black;font-size:20px;");
            line_size[i]->setFocusPolicy(Qt::NoFocus);
            line_size[i]->setValidator(validator);
        }

        btn_sizefunc[0] = new QPushButton(frame_size);
        btn_sizefunc[0]->setGeometry(220,310,100,40);
        btn_sizefunc[0]->show();
        btn_sizefunc[0]->setText("取消");
        btn_sizefunc[0]->setFocusPolicy(Qt::NoFocus);
        btn_sizefunc[0]->setStyleSheet("background-color:white;border:1px solid rgb(0,172,182)");
        connect(btn_sizefunc[0] , SIGNAL(clicked()),this,SLOT(slot_btn()));

        btn_sizefunc[1] = new QPushButton(frame_size);
        btn_sizefunc[1]->setGeometry(80,310,100,40);
        btn_sizefunc[1]->show();
        btn_sizefunc[1]->setText("确定");
        btn_sizefunc[1]->setFocusPolicy(Qt::NoFocus);
        btn_sizefunc[1]->setStyleSheet("background-color:white;border:1px solid rgb(0,172,182)");
        connect(btn_sizefunc[1] , SIGNAL(clicked()),this,SLOT(slot_btn()));
    }
    frame_size->show();
    frame_size->raise();
    for(int i = 0 ; i < 6 ; i++)
    {
//        line_size[i]->setText(QString::number((double)xSys.xArm.aAxis[i]/1000));
    }
}

void UpdateSys::func_savexSys()
{
    QFile file(xSysFile);
    file.resize(0);
    bool ok;
    file.open(QIODevice::WriteOnly);
    QString str = "xArm : ";
    QByteArray ba = str.toLatin1();
    char *mm = ba.data();
    file.write(mm);

    for(int i = 0 ; i < 6 ; i++)
    {
        QString strv = QString::number((int)(line_size[i]->text().toDouble(&ok)*1000))+"    ";
        QByteArray bav = strv.toLatin1();
        char *mmv = bav.data();
        file.write(mmv);
    }
    file.close();
}

void UpdateSys::slot_btn()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(!btn)
        return;
    if(btn == btn_updatefunc)
    {
        updatesys(line_mima->text());
    }
    else if(btn == btn_sizefunc[0])
        this->hide();
    else if(btn == btn_sizefunc[1])
    {
            i32 dsize[6];
            bool ok = false;
            for(int i = 0 ; i < 6 ; i++)
            {
                dsize[i] = (i32)(line_size[i]->text().toDouble(&ok)*1000);

                xSys.xArm.aAxis[i] = dsize[i];
            }
            func_savexSys();
            frame_size->hide();
    }
}
