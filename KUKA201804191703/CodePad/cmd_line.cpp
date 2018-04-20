#include "cmd_line.h"


Cmd_Line::Cmd_Line(QWidget *parent) :
    QWidget(parent)
{
    this->resize(494,52);
    func_BuildCmdLine();
    func_BuildTcpFrame();
    func_BuildSpdDisFrame();
    func_BuildSpdDireFrame();
}


void Cmd_Line::func_BuildCmdLine()
{
    frame_cmd = new QFrame(this);
    frame_cmd->resize(494,52);
    frame_cmd->setObjectName("fc");
    frame_cmd->setStyleSheet("#fc{background-color:rgb(230,230,230)}");
    frame_cmd->hide();

    cb_cmd = new QComboBox(frame_cmd);
    cb_cmd->setGeometry(2,1,60,50);
    cb_cmd->setStyleSheet("border:1px solid; border-radius: 4px;");
    cb_cmd->show();
    cb_cmd->addItem("PTP");
    cb_cmd->addItem("LIN");
    cb_cmd->addItem("CIRC");
    cb_cmd->addItem("SLIN");
    cb_cmd->addItem("SCIRC");
    connect(cb_cmd,SIGNAL(activated(int)),this,SLOT(slot_cbClicked(int)));

    lineAuxPoint = new QLineEdit(frame_cmd);
    lineAuxPoint->setGeometry(64,1,50,50);
    lineAuxPoint->setStyleSheet("border:1px solid; border-radius: 4px;");
    lineAuxPoint->show();

    linePoint = new QLineEdit(frame_cmd);
    linePoint->setGeometry(116,1,90,50);
    linePoint->setStyleSheet("border:1px solid; border-radius: 4px;");
    linePoint->show();

    Btn[0] = new QPushButton(linePoint);
    Btn[0]->setGeometry(50,1,40,50);
    Btn[0]->setStyleSheet("border:1px solid;background-color:rgb(231,231,231);");
    Btn[0]->setFocusPolicy(Qt::NoFocus);
    Btn[0]->show();
    Btn[0]->setIconSize(QSize(35,45));
    Btn[0]->setIcon(QIcon(":/image/image/btn_pd.png"));
    connect(Btn[0],SIGNAL(clicked()),this,SLOT(slot_Btn()));

    cb_cont = new QComboBox(frame_cmd);
    cb_cont->setGeometry(208,1,60,50);
    cb_cont->setStyleSheet("border:1px solid; border-radius: 4px;");
    cb_cont->show();
    cb_cont->addItem("   ");
    cb_cont->addItem("CONT");

    lab[0] = new QLabel(frame_cmd);
    lab[0]->setGeometry(270,1,30,50);
    lab[0]->show();
    lab[0]->setText("Vel=");

    lineSpd = new QLineEdit(frame_cmd);
    lineSpd->setGeometry(302,1,50,50);
    lineSpd->setStyleSheet("border:1px solid; border-radius: 4px;");
    lineSpd->show();

    lab[1] = new QLabel(frame_cmd);
    lab[1]->setGeometry(352,1,25,50);
    lab[1]->show();
    lab[1]->setText("%");

    lineData = new QLineEdit(frame_cmd);
    lineData->setGeometry(382,1,110,50);
    lineData->setStyleSheet("border:1px solid; border-radius: 4px;");
    lineData->show();

    Btn[1] = new QPushButton(lineData);
    Btn[1]->setGeometry(70,1,40,50);
    Btn[1]->setStyleSheet("border:1px solid;background-color:rgb(231,231,231);");
    Btn[1]->show();
    Btn[1]->setFocusPolicy(Qt::NoFocus);
    Btn[1]->setIconSize(QSize(35,45));
    Btn[1]->setIcon(QIcon(":/image/image/btn_pd.png"));
    connect(Btn[1],SIGNAL(clicked()),this,SLOT(slot_Btn()));
}

void Cmd_Line::func_AdjustCmdLine(QString cmd, QString auxpoint , QString point, bool cont, QString Spd, QString Data)
{
    int index = cb_cmd->findText(cmd,Qt::MatchFixedString);
    if(index > 0)
        cb_cmd->setCurrentIndex(index);
    else
        cb_cmd->setCurrentIndex(0);
    if(cmd == "PTP")
    {
        cb_cmd->move(2,1);
        lineAuxPoint->hide();
        linePoint->move(64,1);
        cb_cont->move(156,1);
        lab[0]->move(218,1);
        lineSpd->move(250,1);
        lab[1]->move(300,1);
        lab[1]->setText("%");
        lineData->move(327,1);
    }
    else if(cmd == "LIN" || cmd == "SLIN")
    {
        cb_cmd->move(2,1);
        lineAuxPoint->hide();
        linePoint->move(64,1);
        cb_cont->move(156,1);
        lab[0]->move(218,1);
        lineSpd->move(250,1);
        lab[1]->move(300,1);
        lab[1]->setText("m/s");
        lineData->move(327,1);
    }
    else if(cmd == "CIRC" ||cmd == "SCIRC" )
    {
        cb_cmd->move(2,1);
        lineAuxPoint->show();
        lineAuxPoint->setText(auxpoint);
        lineAuxPoint->move(64,1);
        linePoint->move(116,1);
        cb_cont->move(208,1);
        lab[0]->move(270,1);
        lineSpd->move(302,1);
        lab[1]->move(352,1);
        lab[1]->setText("m/s");
        lineData->move(382,1);
    }
    linePoint->setText(point);

    if(cont)
        cb_cont->setCurrentIndex(1);
    else
        cb_cont->setCurrentIndex(0);

    lineSpd->setText(Spd);
    lineData->setText(Data);
    this->show();
    frame_cmd->show();
}

void Cmd_Line::func_BuildTcpFrame()
{
    frame_TcpBase = new QFrame(this->parentWidget()->parentWidget()->parentWidget());
    frame_TcpBase->setGeometry(0,0,494,250);
    frame_TcpBase->setObjectName("ft");
    frame_TcpBase->setStyleSheet("#ft{background-color:rgb(230,230,230);border-top:1px solid black;}");
    frame_TcpBase->hide();

    QLabel *labtip[5];
    labtip[0]=new QLabel(tr("工具"),frame_TcpBase);
    labtip[0]->setGeometry(10,10,80,20);
    labtip[0]->show();

    labtip[1]=new QLabel(tr("基坐标"),frame_TcpBase);
    labtip[1]->setGeometry(250,10,80,20);
    labtip[1]->show();

    labtip[2]=new QLabel(tr("外部TCP"),frame_TcpBase);
    labtip[2]->setGeometry(10,100,80,20);
    labtip[2]->show();

    labtip[3]=new QLabel(tr("碰撞识别"),frame_TcpBase);
    labtip[3]->setGeometry(250,100,80,20);
    labtip[3]->show();

    labtip[4]=new QLabel(tr("帧"),frame_TcpBase);
    labtip[4]->setGeometry(200,225,80,20);
    labtip[4]->show();

    cb_TB[0]=new QComboBox(frame_TcpBase);   //工具
    cb_TB[0]->setGeometry(15,35,150,40);
    cb_TB[0]->show();
    set_Cb_Tool(cb_TB[0]);

    cb_TB[1]=new QComboBox(frame_TcpBase);   //基座标
    cb_TB[1]->setGeometry(255,35,150,40);
    cb_TB[1]->show();
    set_Cb_Base(cb_TB[1]);

    cb_TB[2]=new QComboBox(frame_TcpBase);   //外部TCP
    cb_TB[2]->setGeometry(15,125,150,40);
    cb_TB[2]->show();
    cb_TB[2]->insertItem(0,tr("True"));
    cb_TB[2]->insertItem(1,tr("False"));

    cb_TB[3]=new QComboBox(frame_TcpBase);  //碰撞识别
    cb_TB[3]->setGeometry(255,125,150,40);
    cb_TB[3]->show();
    cb_TB[3]->insertItem(0,tr("False"));
    cb_TB[3]->insertItem(1,tr("True"));
}

void Cmd_Line::func_showTcpFrame(int tool, int base, bool tcp, bool collision)
{
    if(tool < 0 || tool >= 16)
        return;
    if(base < 0 || base >= 32)
        return;
    if(frame_SpdZone != NULL)
        frame_SpdZone->hide();
    if(frame_SpdDire != NULL)
        frame_SpdDire->hide();
    frame_TcpBase->show();
    frame_TcpBase->raise();
    set_Cb_Tool(cb_TB[0]);
    set_Cb_Base(cb_TB[1]);
    cb_TB[0]->setCurrentIndex(tool);
    cb_TB[1]->setCurrentIndex(base);
    if(tcp)
        cb_TB[2]->setCurrentIndex(0);
    else
        cb_TB[2]->setCurrentIndex(1);
    if(collision)
        cb_TB[3]->setCurrentIndex(1);
    else
        cb_TB[3]->setCurrentIndex(1);
    emit Sig_PointFrame();
}

void Cmd_Line::func_showTcpFrame()
{
    if(frame_SpdZone != NULL)
        frame_SpdZone->hide();
    if(frame_SpdDire != NULL)
        frame_SpdDire->hide();
    frame_TcpBase->show();
    frame_TcpBase->raise();
    emit Sig_PointFrame();
}

void Cmd_Line::func_BuildSpdDisFrame()
{
    frame_SpdZone = new QFrame(this->parentWidget()->parentWidget()->parentWidget());
    frame_SpdZone->setGeometry(0,0,494,250);
    frame_SpdZone->setObjectName("fs");
    frame_SpdZone->setStyleSheet("#fs{background-color:rgb(230,230,230);border-top:1px solid black;}");
    frame_SpdZone->hide();

    QLabel *lab[3];
    QString str[3] = {"加速                                 [%]","圆滑过渡距离                [mm]","移动参数"};
    for(int i = 0 ; i < 3 ; i++)
    {
        lab[i] = new QLabel(frame_SpdZone);
        lab[i]->show();
        lab[i]->setText(str[i]);
    }
    lab[0]->setGeometry(15,30,250,20);
    lab[1]->setGeometry(260,30,250,20);
    lab[2]->setGeometry(200,230,100,20);

    for(int i = 0 ; i < 4 ;i++)
    {
        BtnAdjust[i] = new QPushButton(frame_SpdZone);
        BtnAdjust[i]->show();
        BtnAdjust[i]->setIconSize(QSize(62,42));
        connect(BtnAdjust[i],SIGNAL(clicked()),this,SLOT(slot_Btn()));
    }

    BtnAdjust[0]->setGeometry(20,50,62,42);
    BtnAdjust[0]->setIcon(QIcon(":/image/image/btn_decl.png"));
    BtnAdjust[1]->setGeometry(180,50,60,40);
    BtnAdjust[1]->setIcon(QIcon(":/image/image/btn_add.png"));
    BtnAdjust[2]->setGeometry(260,50,62,42);
    BtnAdjust[2]->setIcon(QIcon(":/image/image/btn_decl.png"));
    BtnAdjust[3]->setGeometry(420,50,62,42);
    BtnAdjust[3]->setIcon(QIcon(":/image/image/btn_add.png"));

    lineAcce = new QLineEdit(frame_SpdZone);
    lineAcce->setGeometry(80,50,100,40);
    lineAcce->setText("100");
    lineAcce->show();

    lineDist=new QLineEdit(frame_SpdZone);
    lineDist->setGeometry(320,50,100,40);
    lineDist->setText("100");
    lineDist->show();
}

void Cmd_Line::func_showSpdDisFrame(int acc, int dist)
{
    if(acc < 0 || acc > 100)
        return;
    if(dist < 0 || dist > 100)
        return;
    if(frame_TcpBase != NULL)
        frame_TcpBase->hide();
    if(frame_SpdDire != NULL)
        frame_SpdDire->hide();
    frame_SpdZone->show();
    frame_SpdZone->raise();
    lineAcce->setText(QString::number(acc));
    lineDist->setText(QString::number(dist));
    emit Sig_PointFrame();
}

void Cmd_Line::func_showSpdDisFrame()
{
    if(frame_TcpBase != NULL)
        frame_TcpBase->hide();
    if(frame_SpdDire != NULL)
        frame_SpdDire->hide();
    frame_SpdZone->show();
    frame_SpdZone->raise();
    emit Sig_PointFrame();
}

void Cmd_Line::func_BuildSpdDireFrame()
{
    frame_SpdDire = new QFrame(this->parentWidget()->parentWidget()->parentWidget());
    frame_SpdDire->setGeometry(0,0,494,250);
    frame_SpdDire->setObjectName("fs");
    frame_SpdDire->setStyleSheet("#fs{background-color:rgb(230,230,230);border-top:1px solid black;}");
    frame_SpdDire->hide();

    QLabel *lab[4];
    for(int i = 0 ; i < 4; i++)
    {
        lab[i] = new QLabel(frame_SpdDire);
        lab[i]->show();
    }
    lab[0]->setGeometry(15,30,250,20);
    lab[0]->setText(tr("轨迹加速                          [%]"));
    lab[1]->setGeometry(260,30,250,20);
    lab[1]->setText(tr("圆滑过渡距离                [mm]"));
    lab[2]->setGeometry(15,130,130,20);
    lab[2]->setText(tr("方向导引"));
    lab[3]->setGeometry(200,230,60,20);
    lab[3]->setText(tr("移动参数"));

    for(int i = 0 ; i < 4 ;i++)
    {
        btnDire[i] = new QPushButton(frame_SpdDire);
        btnDire[i]->show();
        btnDire[i]->setIconSize(QSize(62,42));
        connect(btnDire[i],SIGNAL(clicked()),this,SLOT(slot_Btn()));
    }

    btnDire[0]->setGeometry(20,50,62,42);
    btnDire[0]->setIcon(QIcon(":/image/image/btn_decl.png"));
    btnDire[1]->setGeometry(180,50,60,40);
    btnDire[1]->setIcon(QIcon(":/image/image/btn_add.png"));
    btnDire[2]->setGeometry(260,50,62,42);
    btnDire[2]->setIcon(QIcon(":/image/image/btn_decl.png"));
    btnDire[3]->setGeometry(420,50,62,42);
    btnDire[3]->setIcon(QIcon(":/image/image/btn_add.png"));

    lineDire[0] = new QLineEdit(frame_SpdDire);
    lineDire[0]->setGeometry(80,50,100,40);
    lineDire[0]->setText("100");
    lineDire[0]->show();
    lineDire[1]=new QLineEdit(frame_SpdDire);
    lineDire[1]->setGeometry(320,50,100,40);
    lineDire[1]->setText("100");
    lineDire[1]->show();

    cb_Dire = new QComboBox(frame_SpdDire);
    cb_Dire->setGeometry(20,155,150,40);
    cb_Dire->show();
    cb_Dire->insertItem(0,tr("标准"));
    cb_Dire->insertItem(1,tr("手动PTP"));
    cb_Dire->insertItem(2,tr("恒定的方向"));
}

void Cmd_Line::func_showSpdDireFrame(int acc, int dist, int dire)
{
    if(acc < 0 || acc > 100)
        return;
    if(dist < 0 || dist > 100)
        return;
    if(dire < 0 || dire > 3)
        return;
    if(frame_TcpBase != NULL)
        frame_TcpBase->hide();
    if(frame_SpdZone != NULL)
        frame_SpdZone->hide();
    frame_SpdDire->show();
    frame_SpdDire->raise();
    lineDire[0]->setText(QString::number(acc));
    lineDire[1]->setText(QString::number(dist));
    cb_Dire->setCurrentIndex(dire);
    emit Sig_PointFrame();
}

void Cmd_Line::func_showSpdDireFrame()
{
    if(frame_TcpBase != NULL)
        frame_TcpBase->hide();
    if(frame_SpdZone != NULL)
        frame_SpdZone->hide();
    frame_SpdDire->show();
    frame_SpdDire->raise();
    emit Sig_PointFrame();
}


int Cmd_Line::func_GetCmdType()
{
    int type = cb_cmd->currentIndex();
    return type;
}

QStringList Cmd_Line::func_getFDATInfo()
{
    QString pointname,fdataname;
    QStringList infolist;
    CodeData fdat;
    int TOOL_NO;   //工具坐标号
    int BASE_NO;   //基座标号
    int IPO_FRAME;    //外部TCP为TRUE时为#TCP 0，为FALSE时为#BASE 1
    int TQ_STATE;   //碰撞识别
    bool ok;

    pointname= linePoint->text();
    fdataname="F"+pointname;
    infolist.clear();
    fdat = func_getDataByName(fdataname,&ok);
    if(!ok)
        return  infolist;
    TOOL_NO = fdat.val.fdatVal.TOOL_NO;
    BASE_NO = fdat.val.fdatVal.BASE_NO;
    if(fdat.val.fdatVal.IPO_FRAME)
        IPO_FRAME = 0;
    else
        IPO_FRAME = 1;
    if(fdat.val.fdatVal.TQ_STATE)
        TQ_STATE = 1;
    else
        TQ_STATE = 0;
    infolist<<QString::number(TOOL_NO)<<QString::number(BASE_NO)<<QString::number(IPO_FRAME)<<QString::number(TQ_STATE);
    return infolist;
}

QStringList Cmd_Line::func_getPDATInfo()
{
    QString name;
    QStringList infolist;
    CodeData pdat;
    double acc;   //加速
    double dist;   //圆滑过度距离
    bool ok;

    name = "P"+lineData->text();
    infolist.clear();
    pdat = func_getDataByName(name,&ok);
    if(!ok)
        return  infolist;
    acc = pdat.val.pdatVal.ACC;
    dist = pdat.val.pdatVal.APO_DIST;
    infolist<<QString::number(acc)<<QString::number(dist);
    return infolist;
}

QStringList Cmd_Line::func_getLDATInfo()
{
    QString name;
    QStringList infolist;
    CodeData ldat;
    double acc;   //加速
    double dist;   //圆滑过度距离
    int ORI_TYP;
    bool ok;

    name = "L"+lineData->text();
    infolist.clear();
    ldat = func_getDataByName(name,&ok);
    if(!ok)
        return  infolist;
    acc = ldat.val.ldatVal.ACC;
    dist = ldat.val.ldatVal.APO_DIST;
    ORI_TYP = ldat.val.ldatVal.ORI_TYP;
    infolist<<QString::number(acc)<<QString::number(dist)<<QString::number(ORI_TYP);
}

void Cmd_Line::func_UpdateCb()
{

}

/*
** 函数功能： 新建一条指令时，生成一条指令的字符串
** 函数参数：type，指令类型
** 函数返回值：一条指令的字符串
** 函数说明：无
*/
QString Cmd_Line::func_GetCmdStr(int type)
{
    QString str;
    str.clear();
    QString strcont;
    if(cb_cont->currentIndex()==0)
        strcont=" ";
    else
        strcont = " CONT ";
    switch (type) {
    case 0:  //PTP

    case 1:  //LIN

    case 3:  //SLIN
        str = cb_cmd->currentText()+" "+linePoint->text()+strcont+lab[0]->text()+" "+\
                lineSpd->text()+" "+lab[1]->text()+" "+lineData->text()+" Tool["+QString::number(cb_TB[0]->currentIndex())+"] BASE["\
                +QString::number(cb_TB[1]->currentIndex())+"]";
        if(cb_TB[2]->currentIndex() == 0)
            str += " extTCP";
        if(cb_TB[3]->currentIndex() == 1)
            str += " CD";
        break;
    case 2:   //CIRC

    case 4:  //SCIRC
        str = cb_cmd->currentText()+" "+lineAuxPoint->text()+" "+linePoint->text()+strcont+\
                lab[0]->text()+" "+lineSpd->text()+" "+lab[1]->text()+" "+lineData->text()+" Tool["+QString::number(cb_TB[0]->currentIndex())+"] BASE["\
                +QString::number(cb_TB[1]->currentIndex())+"]";
        if(cb_TB[2]->currentIndex() == 0)
            str += " extTCP";
        if(cb_TB[3]->currentIndex() == 1)
            str += " CD";
        break;
    default:
        break;
    }
    return str;
}

void Cmd_Line::slot_Btn()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if(!btn)
        return;
    if(btn == Btn[0])
    {
        QStringList infolist;
        infolist.clear();
        infolist = func_getFDATInfo();
        if(infolist.isEmpty())
            func_showTcpFrame();
        else
        {
            bool ok;
            int tool,base;
            bool tcp, collision;
            tool = infolist.at(0).toInt(&ok);
            base = infolist.at(1).toInt(&ok);
            if(infolist.at(2) == "1")
                tcp = true;
            else
                tcp = false;
            if(infolist.at(3) == "1")
                collision = true;
            else
                collision = false;
            func_showTcpFrame(tool, base, tcp, collision);
        }
        listwidget.prepend(frame_TcpBase);
    }
    else if(btn == Btn[1])
    {
        if(cb_cmd->currentIndex() == 0)  //PTP
        {
            QStringList infolist;
            infolist.clear();
            infolist = func_getPDATInfo();
            if(infolist.isEmpty())
                func_showSpdDisFrame();
            else
            {
                bool ok;
                int acc,dist;
                acc = infolist.at(0).toInt(&ok);
                dist = infolist.at(1).toInt(&ok);
                func_showSpdDisFrame(acc,dist);
            }
            listwidget.prepend(frame_SpdZone);
        }
        else  //LIN CIRC
        {
            QStringList infolist;
            infolist.clear();
            infolist = func_getLDATInfo();
            if(infolist.isEmpty())
                func_showSpdDireFrame();
            else
            {
                bool ok;
                int acc,dist,dire;
                acc = infolist.at(0).toInt(&ok);
                dist = infolist.at(1).toInt(&ok);
                dire = infolist.at(2).toInt(&ok);
                func_showSpdDireFrame(acc,dist,dire);
            }
            listwidget.prepend(frame_SpdDire);
        }
    }
    else if(btn == btnDire[0])
    {
        int value;
        bool ok;
        value = lineDire[0]->text().toInt(&ok);
        if(!ok)
            return;
        if(value <= 0)
            return;
        --value;
        lineDire[0]->setText(QString::number(value));
    }
    else if(btn == btnDire[1])
    {
        int value;
        bool ok;
        value = lineDire[0]->text().toInt(&ok);
        if(!ok)
            return;
        if(value >= 100)
            return;
        ++value;
        lineDire[0]->setText(QString::number(value));
    }
    else if(btn == btnDire[2])
    {
        int value;
        bool ok;
        value = lineDire[1]->text().toInt(&ok);
        if(!ok)
            return;
        if(value <= 0)
            return;
        --value;
        lineDire[1]->setText(QString::number(value));
    }
    else if(btn == btnDire[3])
    {
        int value;
        bool ok;
        value = lineDire[1]->text().toInt(&ok);
        if(!ok)
            return;
        if(value >= 100)
            return;
        ++value;
        lineDire[1]->setText(QString::number(value));
    }
    else if(btn == BtnAdjust[0])
    {
        int value;
        bool ok;
        value = lineAcce->text().toInt(&ok);
        if(!ok)
            return;
        if(value <= 0)
            return;
        --value;
        lineAcce->setText(QString::number(value));
    }
    else if(btn == BtnAdjust[1])
    {
        int value;
        bool ok;
        value = lineAcce->text().toInt(&ok);
        if(!ok)
            return;
        if(value >= 100)
            return;
        ++value;
        lineAcce->setText(QString::number(value));
    }
    else if(btn == BtnAdjust[2])
    {
        int value;
        bool ok;
        value = lineDist->text().toInt(&ok);
        if(!ok)
            return;
        if(value <= 0)
            return;
        --value;
        lineDist->setText(QString::number(value));
    }
    else if(btn == BtnAdjust[3])
    {
        int value;
        bool ok;
        value = lineDist->text().toInt(&ok);
        if(!ok)
            return;
        if(value >= 100)
            return;
        ++value;
        lineDist->setText(QString::number(value));
    }
}

void Cmd_Line::slot_cbClicked(int i)
{
    QComboBox *cb = qobject_cast<QComboBox*>(sender());
    if( !cb )
        return;
    if(cb == cb_cmd)
    {
        QString cmd,auxpoint,point,spd,data;
        bool cont = false;
        bool ok;
        CodeData pldata;
        cmd = cb_cmd->currentText();
        if( !lineAuxPoint->isHidden() )
            auxpoint = lineAuxPoint->text();
        else
            auxpoint = func_BuildDataName("E6POS");
        if( cb_cont->currentIndex() == 1)
            cont = true;
        if( i == 0)
            spd = "100";
        else
            spd = "2";
        point = linePoint->text();
        data = lineData->text();
        pldata = func_getDataByName(data,&ok);
        if(!ok)
            return;
        if(cmd == "PTP" && pldata.dtype != pdattype)
            data = func_BuildDataName("PDAT");
        if( cmd != "PTP" && pldata.dtype != ldattype)
            data = func_BuildDataName("LDAT");
        func_AdjustCmdLine(cmd,auxpoint,point,cont,spd,data);
        emit sig_cmdchange(i);
    }
}

CodeData func_getDataByName(QString name,bool *ok)
{
    CodeData cdata;
    for(int i=0;i<CNT_Type;i++)
    {
        foreach (cdata, codedata[i]) {
            if(cdata.name == name)
            {
                *ok = true;
                return cdata;
            }
        }
    }
    *ok = false;
    return cdata;
}
