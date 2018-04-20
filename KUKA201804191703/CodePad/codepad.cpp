#include "codepad.h"

QMap<QString,int> TypeToIndex;
QMap<QString,int> CMDToIndex;
QMap<int,int> EditToSrc;
QList<CodeData> codedata[CNT_Type];

CodePad::CodePad(QWidget *parent)
    : QWidget(parent)
{
    init_mainframe();
}

void CodePad::init_mainframe()
{
    widget = new QWidget(this);
    widget->setGeometry(0,0,494,694);

    frame_base = new QFrame(widget);
    frame_base->setGeometry(0,0,494,694);

    lab_tip = new QLabel(frame_base);
    lab_tip->setGeometry(0,0,494,30);
    lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
    lab_tip->setText("   编辑器");

    codeeditor = new CodeEditor(frame_base);
    codeeditor->setGeometry(0,30,494,584);

    codefunc = new CodeDataFunc();   //和程序数据相关的功能
    connect(codefunc,SIGNAL(sig_hasNewData(CodeData)),this,SLOT(slot_funcNewData(CodeData)));
    connect(codefunc,SIGNAL(sig_deleteData(CodeData)),this,SLOT(slot_funcDelData(CodeData)));

    funcParser = new Parser();

    lab_ctlframe = new QLabel(frame_base);
    lab_ctlframe->setGeometry(0,614,494,80);
    lab_ctlframe->setStyleSheet("background-color:rgb(230,230,230);");
    /*显示当前程序的信息，位置，鼠标当前的行列*/
    for(int i = 0 ; i < 4 ;i++)
    {
        lab_info[i] = new QLabel(lab_ctlframe);
        lab_info[i]->setStyleSheet("background-color:rgb(200,200,200);border:2px solid rgb(132,132,132);");
    }
    lab_info[0]->setGeometry(0,0,60,30);
    lab_info[1]->setGeometry(61,0,200,30);
    lab_info[2]->setGeometry(262,0,100,30);
    lab_info[3]->setGeometry(363,0,131,30);

    for(int i = 0 ; i < 7 ;i++)
    {
        btnCtlFunc[i] = new QPushButton(lab_ctlframe);
        btnCtlFunc[i]->setGeometry(2+70*i,35,68,42);
        btnCtlFunc[i]->setFocusPolicy(Qt::NoFocus);
        connect(btnCtlFunc[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnCtlFunc(bool)));
        btnCtlFunc[i]->setIconSize(QSize(68,42));
    }
    btnCtlFunc[1]->setCheckable(true);
    btnCtlFunc[5]->setCheckable(true);
    btnCtlFunc[1]->setStyleSheet("QPushButton:checked{border: 5px solid rgb(0,170,255);}"\
                        "QPushButton:unchecked{border:none} "\
                "QPushButton{background-color:rgb(160,165,170)}");
    btnCtlFunc[5]->setStyleSheet("QPushButton:checked{border: 5px solid rgb(0,170,255);}"\
                        "QPushButton:unchecked{border:none} "\
                "QPushButton{background-color:rgb(160,165,170)}");
    btnCtlFunc[0]->setIcon(QIcon(":/image/image/btn_change.png"));
    btnCtlFunc[1]->setIcon(QIcon(":/image/image/btn_cmd.png"));
    btnCtlFunc[2]->setIcon(QIcon(":/image/image/btn_act.png"));
    btnCtlFunc[3]->setIcon(QIcon(":/image/image/btn_expand.png"));
    btnCtlFunc[4]->setIcon(QIcon(":/image/image/btn_selectcmd.png"));
    btnCtlFunc[5]->setIcon(QIcon(":/image/image/btn_para.png"));
    btnCtlFunc[6]->setIcon(QIcon(":/image/image/btn_edit.png"));
    func_BuildCmdFrame();
    func_hideCmdFrame();
    func_BuildEditFrame();
    func_hideEditFrame();

    old_pa=btnCtlFunc[0]->palette();
    old_stysheet="background-color:rgb(249,249,249)";
    new_pa.setColor(QPalette::WindowText,QColor(30,144,255,0));
    new_stylesheet="background-color:rgb(57,185,255)";

    cmdline = new Cmd_Line(codeeditor);
    connect(cmdline,SIGNAL(Sig_PointFrame()),this,SLOT(slot_resizeFrame()));
    connect(cmdline,SIGNAL(sig_cmdchange(int)),this,SLOT(slot_changeBtn(int)));
}


void CodePad::func_ChangeCMD(QTextCursor temptc)
{
    temptc = codeeditor->textCursor();
    temptc.movePosition(QTextCursor::StartOfBlock , QTextCursor::MoveAnchor);
    temptc.movePosition(QTextCursor::EndOfBlock , QTextCursor::KeepAnchor);
    QString selectstr = temptc.selectedText();
    if(selectstr.isEmpty())
    {
        QMessageBox::warning(this,"更改指令","该处的更改被拒绝，因为不能进行句选择。","确定");
        return;
    }
    selectstr = selectstr.simplified();
    qDebug()<<selectstr;
    QStringList selectlist = selectstr.split(" ");
    if(selectlist.count() < 6)
    {
        QMessageBox::warning(this,"更改指令","该处的更改被拒绝，因为不能进行句选择。","确定");
        return;
    }
    QString cmd = selectlist.at(0);
    int res = cmdline->cb_cmd->findText(cmd);
    if(res < 0)
    {
        QMessageBox::warning(this,"更改指令","该处的更改被拒绝，因为不能进行句选择。","确定");
        return;
    }
    cmdline->cb_cmd->setCurrentIndex(res);

}

void CodePad::func_BuildCmdFrame()
{
    frame_cmd = new MenuWidget(this->parentWidget()->parentWidget());
    QSizePolicy sizepolicy = frame_cmd->sizePolicy();
    sizepolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    frame_cmd->setSizePolicy(sizepolicy);
    frame_cmd->move(0,330);
    frame_cmd->setMinimumSize(169,314);
    frame_cmd->setObjectName("fc");
    frame_cmd->setStyleSheet("#fc{border:4px solid rgb(0,170,255)}");
    connect(frame_cmd,SIGNAL(aboutToHide()),this,SLOT(slot_aboutToHide()));
    connect(frame_cmd,SIGNAL(aboutToShow(MenuWidget*)),this,SLOT(slot_aboutToShow(MenuWidget*)));

    QLabel *lab_tip=new QLabel(frame_cmd);
    lab_tip->setGeometry(4,3,494,17);
    lab_tip->setStyleSheet("background-color:rgb(160,165,170);color:white;font:bold;font-size:14px");
    lab_tip->setText(tr("  指令"));
    lab_tip->show();

    init_arrow();
    for(int i=0;i<3;i++)
    {
        frameBtn[i]=new QFrame(frame_cmd);
        frameBtn[i]->setGeometry(4+166*i,20,160,290);
        frameBtn[i]->show();
        frameBtn[i]->setStyleSheet("background-color:rgb(230,230,230)");
    }
    QString str[6] = {"上一条指令","运动","移动参数","逻辑","模拟输出","注释"};
    for(int i=0;i<6;i++)
    {
        btnFrame0[i]=new QPushButton(frameBtn[0]);
        btnFrame0[i]->setGeometry(0,42*i,161,42);
        btnFrame0[i]->setText(str[i]);
        btnFrame0[i]->setFocusPolicy(Qt::NoFocus);
        btnFrame0[i]->setStyleSheet(old_stysheet);
        btnFrame0[i]->show();
        connect(btnFrame0[i],SIGNAL(clicked()),this,SLOT(slot_BtnFrame0()));
        connect(btnFrame0[i],SIGNAL(clicked()),this,SLOT(setColor0()));
    }
    for(int i=1 ; i<6 ; i++)
        setArrow(lab0[i] , btnFrame0[i]);
    /*第二栏中的按钮*/
    for(int i=0;i<7;i++)
    {
        btnFrame1[i]=new QPushButton(frameBtn[1]);
        btnFrame1[i]->setGeometry(0,42*i,161,42);
        btnFrame1[i]->show();
        btnFrame1[i]->setFocusPolicy(Qt::NoFocus);
        btnFrame1[i]->setStyleSheet(old_stysheet);
        connect(btnFrame1[i],SIGNAL(clicked()),this,SLOT(slot_BtnFrame1()));
        connect(btnFrame1[i],SIGNAL(clicked()),this,SLOT(setColor1()));
    }
    /*第三栏中的按钮*/
    for(int i=0;i<4;i++)
    {
        btnFrame2[i]=new QPushButton(frameBtn[2]);
        btnFrame2[i]->setGeometry(0,42*i,161,42);
        btnFrame2[i]->show();
        btnFrame2[i]->setFocusPolicy(Qt::NoFocus);
        btnFrame2[i]->setStyleSheet(old_stysheet);
        connect(btnFrame2[i],SIGNAL(clicked()),this,SLOT(slot_BtnFrame2()));
    }
}

void CodePad::func_showCmdFrame()
{
    frame_cmd->show();
    frame_cmd->resize(169,314);
}

void CodePad::func_hideCmdFrame()
{
    frame_cmd->hide();
}


void CodePad::func_BuildEditFrame()
{
    frame_edit = new MenuWidget(this->parentWidget()->parentWidget());
    QSizePolicy sizepolicy = frame_edit->sizePolicy();
    sizepolicy.setHorizontalPolicy(QSizePolicy::Expanding);
    frame_edit->setSizePolicy(sizepolicy);
    frame_edit->move(326,60);
    frame_edit->setMinimumSize(168,584);
    frame_edit->setObjectName("fe");
    frame_edit->setStyleSheet("#fe{border:4px solid rgb(0,170,255)}");
    connect(frame_edit,SIGNAL(aboutToHide()),this,SLOT(slot_aboutToHide()));
    connect(frame_edit,SIGNAL(aboutToShow(MenuWidget*)),this,SLOT(slot_aboutToShow(MenuWidget*)));

    QLabel *lab_tip=new QLabel(frame_edit);
    lab_tip->setGeometry(4,3,322,17);
    lab_tip->setStyleSheet("background-color:rgb(160,165,170);color:white;font:bold;font-size:14px");
    lab_tip->setText(tr("  编辑"));
    lab_tip->show();

    init_editarrow();
    for(int i=0;i<2;i++)
    {
        frameBtnEdit[i]=new QFrame(frame_edit);
        frameBtnEdit[i]->setGeometry(4+162*i,20,160,560);
        frameBtnEdit[i]->show();
        frameBtnEdit[i]->setStyleSheet("background-color:rgb(230,230,230)");
    }
    QString str[14] = {"FOLD","清理数据列表","剪切","复制","添加","删除","打印","查找",\
                                           "替换", "标记的区域","视图","取消选择程序","程序复位","导航器"};
    for(int i=0;i<14;i++)
    {
        btn_edit0[i]=new QPushButton(frameBtnEdit[0]);
        btn_edit0[i]->setGeometry(0,40*i,160,40);
        btn_edit0[i]->show();
        btn_edit0[i]->setText(str[i]);
        btn_edit0[i]->setFocusPolicy(Qt::NoFocus);
        btn_edit0[i]->setStyleSheet(old_stysheet);
        connect(btn_edit0[i],SIGNAL(clicked()),this,SLOT(slot_EditBtn()));
        connect(btn_edit0[i],SIGNAL(clicked()),this,SLOT(setEditColor()));
    }
    setArrow(labedit[0] , btn_edit0[0]);
    setArrow(labedit[1] , btn_edit0[9]);
    setArrow(labedit[2] , btn_edit0[10]);

    for(int i=0;i<6;i++)
    {
        btn_edit1[i]=new QPushButton(frameBtnEdit[1]);
        btn_edit1[i]->setGeometry(0,42*i,161,42);
        btn_edit1[i]->show();
        btn_edit1[i]->setFocusPolicy(Qt::NoFocus);
        btn_edit1[i]->setStyleSheet(old_stysheet);
        connect(btn_edit1[i],SIGNAL(clicked()),this,SLOT(slot_EditBtn()));
    }
}

void CodePad::func_showEditFrame()
{
    frame_edit->show();
    frame_edit->resize(168,584);
}

void CodePad::func_hideEditFrame()
{
    frame_edit->hide();
}


void CodePad::func_SelectRow()
{
    int x = codeeditor->lineNumberAreaWidth()-2;
    int y = codeeditor->func_GetArrowY(codeeditor->textCursor());
    codeeditor->lab_tc->move(x,y);
    codeeditor->lab_tc->show();
    codeeditor->func_SelectLine();
}

void CodePad::init_arrow()
{
    QPixmap pixmap;
    pixmap.load(":/arrow.PNG");
    for(int i=0;i<6;i++)
    {
        lab0[i]=new QLabel(this);
        lab0[i]->hide();
        lab0[i]->resize(20,15);
        lab0[i]->setStyleSheet("background: transparent");
        lab0[i]->setPixmap(pixmap);
    }
    lab1=new QLabel(this);
    lab1->hide();
    lab1->resize(20,15);
    lab1->setStyleSheet("background: transparent");
    lab1->setPixmap(pixmap);
}

void CodePad::init_editarrow()
{
    QPixmap pixmap;
    pixmap.load(":/arrow.PNG");
    for(int i=0;i<3;i++)
    {
        labedit[i]=new QLabel(this);
        labedit[i]->hide();
        labedit[i]->resize(20,15);
        labedit[i]->setStyleSheet("background: transparent");
        labedit[i]->setPixmap(pixmap);
    }
}
void CodePad::setArrow(QLabel *lab, QPushButton *btn)
{
    lab->setParent(btn);
    lab->move(130,15);
    lab->show();
    lab->raise();
}

void CodePad::Btn_Change(int state)
{
    switch (state) {
    case 0:

    case 1:

    case 3:
        for(int i = 0 ; i < 7 ; i++)
        {
            btnCtlFunc[i]->setIcon(QIcon(":/image/image/btn_kb.png"));
            btnCtlFunc[i]->setEnabled(false);
        }
        btnCtlFunc[0]->setEnabled(true);
        btnCtlFunc[0]->setIcon(QIcon(":/image/image/btn_zd.png"));

        btnCtlFunc[5]->setEnabled(true);
        btnCtlFunc[5]->setIcon(QIcon(":/image/image/btn_para.png"));

        btnCtlFunc[6]->setEnabled(true);
        btnCtlFunc[6]->setIcon(QIcon(":/image/image/btn_cmdok.png"));
        break;
    case 2:

    case 4:
        for(int i = 0 ; i < 7 ; i++)
        {
            btnCtlFunc[i]->setIcon(QIcon(":/image/image/btn_kb.png"));
            btnCtlFunc[i]->setEnabled(false);
        }
        btnCtlFunc[0]->setEnabled(true);
        btnCtlFunc[0]->setIcon(QIcon(":/image/image/btn_zd.png"));

        btnCtlFunc[4]->setEnabled(true);
        btnCtlFunc[4]->setIcon(QIcon(":/image/image/btn_auxpoint.png"));

        btnCtlFunc[5]->setEnabled(true);
        btnCtlFunc[5]->setIcon(QIcon(":/image/image/btn_targetpoint.png"));

        btnCtlFunc[6]->setEnabled(true);
        btnCtlFunc[6]->setIcon(QIcon(":/image/image/btn_cmdok.png"));
        break;
    case -2:
        for(int i = 0 ; i < 7 ; i++)
        {
            btnCtlFunc[i]->setText("");
            btnCtlFunc[i]->setEnabled(true);
        }
        btnCtlFunc[0]->setIcon(QIcon(":/image/image/btn_change.png"));
        btnCtlFunc[1]->setIcon(QIcon(":/image/image/btn_cmd.png"));
        btnCtlFunc[2]->setIcon(QIcon(":/image/image/btn_act.png"));
        btnCtlFunc[3]->setIcon(QIcon(":/image/image/btn_expand.png"));
        btnCtlFunc[4]->setIcon(QIcon(":/image/image/btn_selectcmd.png"));
        btnCtlFunc[5]->setIcon(QIcon(":/image/image/btn_para.png"));
        btnCtlFunc[6]->setIcon(QIcon(":/image/image/btn_edit.png"));
        break;
    default:
        break;
    }
}

/*
** 函数功能：构建一个变量名
** 函数参数：type，变量类型
** 函数返回值：存在返回1，不存在返回-1
** 函数说明：此函数应该还要考虑到构建的变量名是否会重名
*/
QString func_BuildDataName(QString type)
{
    QString point;
    QString fdata;
    QString pdata;
    QString ldata;
    int cnt = 1;
    int cmdtype = TypeToIndex.value(type);
    if(cmdtype == 8)   //E6POS
    {
        point = "P"+QString::number(codedata[cmdtype].count()+cnt);  //目标点名
        while(func_JudgeDataExist(point) == 1)
        {
            cnt++;
            point = "P"+QString::number(codedata[cmdtype].count()+cnt);  //目标点名
            if(cnt > 100)   //防止陷入死循环
                break;
        }
        return point;
    }
    else if( cmdtype == 9)   //FDAT
    {
        cnt = 1;
        fdata = "FP" + QString::number(codedata[cmdtype].count() + cnt);
        while(func_JudgeDataExist(fdata) == 1)
        {
            cnt++;
            fdata = "FP" + QString::number(codedata[cmdtype].count() + cnt);  //FDAT
            if(cnt > 100)   //防止陷入死循环
                break;
        }
        return fdata;
    }
    else if( cmdtype == 10)   //PDAT
    {
        cnt = 1;
        pdata = "PDATA"+QString::number(codedata[cmdtype].count()+cnt);
        while(func_JudgeDataExist(pdata) == 1)
        {
            cnt++;
            pdata = "PDATA"+QString::number(codedata[cmdtype].count()+cnt);  //PDAT
            if(cnt > 100)   //防止陷入死循环
                break;
        }
        return pdata;
    }
    else if( cmdtype == 11)   //LDAT
    {
        cnt = 1;
        ldata = "CPDATA"+QString::number(codedata[cmdtype].count()+cnt);
        while(func_JudgeDataExist(ldata) == 1)
        {
            cnt++;
            ldata = "CPDATA"+QString::number(codedata[cmdtype].count()+cnt);  //LDAT
            if(cnt > 100)   //防止陷入死循环
                break;
        }
        return ldata;
    }
}

int CodePad::func_changeData(CodeData olddata, CodeData newdata)
{
    QString srcpath,datpath,oldstr,newstr;
    QPlainTextEdit *tempedit;
    QTextCursor temptc;
    srcpath = olddata.module+".src";;
    datpath = olddata.module+".dat";
    oldstr = func_calcDataDecl(olddata);
    newstr = func_calcDataDecl(newdata);

    tempedit = new QPlainTextEdit(this);
    tempedit->hide();
    tempedit->clear();
    QFile file(datpath);
    //如果数据文件存在并且打开成功，则将数据文件中的内容读进tempedit中
    if( file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&file);
        while(!in.atEnd())
            tempedit->appendPlainText(in.readLine());
    }
    else
        return -1;
    int res = func_CursorGoStr(tempedit,oldstr);
    if(res == -1)
        return -1;
    temptc = tempedit->textCursor();
    temptc.movePosition(QTextCursor::EndOfBlock,QTextCursor::KeepAnchor);
    temptc.insertText(newstr);
    func_SaveToFile(tempedit,datpath);

    QString name ;
    int idtype,index;
    QString type;
    name = olddata.name;
    idtype = (CodeDataType)olddata.dtype;
    if(idtype < 0 || idtype >= CNT_Type)
        return -1;
    type = TypeToIndex.key(idtype);
    index = func_getDataIndex(name,type);
    if(index == -1)
        return -1;
    codedata[idtype][index] = newdata;
}

/*
** 函数功能：创建一条指令中用到的所有数据
** 函数参数：type，指令类型
** 函数返回值：无
** 函数说明：无
*/
void CodePad::func_BuildCMDData(int type)
{
    QString auxpointdata;   //辅助点
    QString pointdata;   //目标点
    QString fdata;   //FDAT
    QString pdata;  //PDAT
    QString ldata;   //LDAT

    auxpointdata = cmdline->lineAuxPoint->text();
    pointdata = cmdline->linePoint->text();
    fdata = "F"+pointdata;   //所有的指令都有FDAT，PTP是PDAT，其余都是LDAT
    pdata = cmdline->lineData->text();
    ldata = cmdline->lineData->text();

    if(type == 2 || type == 4 )   //CIRC和SCIRC有辅助点
    {
        QString auxpointsvalue;
        QStringList auxpointvallist = func_GetPointInfo(&auxpointsvalue);
        if( func_JudgeDataExist( auxpointdata ) == 1)   //名称已存在
        {
            bool ok;
            CodeData olddata,newdata;
            olddata = func_getDataByName( auxpointdata , &ok);
            if( !ok )
                return;
            newdata.name = auxpointdata;
            newdata.Svalue = auxpointsvalue;
            newdata.dtype = e6postype;
            newdata.module = KukaRob.CurMName;
            newdata.code = "";
            newdata.dimension = 1;
            newdata.val.e6posVal.sete6posval(auxpointvallist);
            func_changeData(olddata,newdata);
        }
        else
            codefunc->func_BuildData(auxpointdata  , KukaRob.CurMName , "" , 8 , auxpointvallist , auxpointsvalue);
    }

    QString pointsvalue;   //目标点
    QStringList pointvallist = func_GetPointInfo(&pointsvalue);
    if( func_JudgeDataExist( pointdata ) == 1)   //名称已存在
    {
        bool ok;
        CodeData olddata,newdata;
        olddata = func_getDataByName( pointdata , &ok);
        if( !ok )
            return;
        newdata.name = pointdata;
        newdata.Svalue = pointsvalue;
        newdata.dtype = e6postype;
        newdata.module = KukaRob.CurMName;
        newdata.code = "";
        newdata.dimension = 1;
        newdata.val.e6posVal.sete6posval(pointvallist);
        func_changeData(olddata,newdata);
    }
    else
        codefunc->func_BuildData(pointdata  , KukaRob.CurMName , "" , 8 , pointvallist , pointsvalue);

    QString fdatvalue;   //FDAT
    QStringList fdatvallist = func_GetFDATInfo(&fdatvalue);
    if( func_JudgeDataExist( fdata ) == -1)   //名称不存在
        codefunc->func_BuildData(fdata , KukaRob.CurMName ,"" , 9 , fdatvallist , fdatvalue);

    if(type == 0)    //PTP是PDAT
    {
        QString pdatvalue;
        QStringList pdatvallist = func_GetPDATInfo(&pdatvalue);
        if( func_JudgeDataExist( pdata ) == -1)   //名称不存在
            codefunc->func_BuildData(pdata , KukaRob.CurMName , "" , 10 , pdatvallist , pdatvalue);
    }
    else    //LIN、CIRC、SLIN、SCIRC是LDAT
    {
        QString ldatvalue;
        QStringList ldatvallist = func_GetLDATInfo(&ldatvalue);
        if( func_JudgeDataExist( ldata ) == -1)   //名称不存在
            codefunc->func_BuildData(ldata , KukaRob.CurMName , "" ,11 , ldatvallist , ldatvalue);
    }
}

void CodePad::func_PTP()
{
    QString point = func_BuildDataName("E6POS");
    QString pdat = func_BuildDataName("PDAT");
    cmdline->func_AdjustCmdLine( tr("PTP") , "" , point , true , QString::number(KukaRob.PVel) , pdat);
    int y = codeeditor->func_GetCMDY(codeeditor->textCursor());
    cmdline->move(0,y);
    Btn_Change(0);
}

void CodePad::func_LIN()
{
    QString point = func_BuildDataName("E6POS");
    QString ldat = func_BuildDataName("LDAT");
    cmdline->func_AdjustCmdLine( tr("LIN") , "" , point , true , QString::number(KukaRob.MVel) , ldat);
    int y = codeeditor->func_GetCMDY(codeeditor->textCursor());
    cmdline->move(0,y);
    Btn_Change(1);
}


void CodePad::func_CIRC()
{
    int cnt = 1;
    QString auxpoint = tr("P")+QString::number(codedata[8].count()+cnt);  //目标点名
    while(func_JudgeDataExist(auxpoint) == 1)
    {
        cnt++;
        auxpoint = tr("P")+QString::number(codedata[8].count()+cnt);  //目标点名
        if(cnt > 100)   //防止陷入死循环
            break;
    }
    cnt = 1;
    QString point = tr("P")+QString::number(codedata[8].count()+cnt+1);  //目标点名
    while(func_JudgeDataExist(point) == 1)
    {
        cnt++;
        point = tr("P")+QString::number(codedata[8].count()+cnt+1);  //目标点名
        if(cnt > 100)   //防止陷入死循环
            break;
    }
    QString ldat = func_BuildDataName("LDAT");
    cmdline->func_AdjustCmdLine( tr("CIRC") , auxpoint , point , true , QString::number(KukaRob.MVel) , ldat);
    int y = codeeditor->func_GetCMDY(codeeditor->textCursor());
    cmdline->move(0,y);
    Btn_Change(2);
}

void CodePad::func_SLIN()
{
    QString point = func_BuildDataName("E6POS");
    QString ldat = func_BuildDataName("LDAT");
    cmdline->func_AdjustCmdLine( tr("SLIN") , "" , point , true , QString::number(KukaRob.MVel) , ldat);
    int y = codeeditor->func_GetCMDY(codeeditor->textCursor());
    cmdline->move(0,y);
    Btn_Change(3);
}

void CodePad::func_SCIRC()
{
    int cnt = 1;
    QString auxpoint = tr("P")+QString::number(codedata[8].count()+cnt);  //目标点名
    while(func_JudgeDataExist(auxpoint) == 1)
    {
        cnt++;
        auxpoint = tr("P")+QString::number(codedata[8].count()+cnt);  //目标点名
        if(cnt > 100)   //防止陷入死循环
            break;
    }
    cnt = 1;
    QString point = tr("P")+QString::number(codedata[8].count()+cnt+1);  //目标点名
    while(func_JudgeDataExist(point) == 1)
    {
        cnt++;
        point = tr("P")+QString::number(codedata[8].count()+cnt+1);  //目标点名
        if(cnt > 100)   //防止陷入死循环
            break;
    }
    QString ldat = func_BuildDataName("LDAT");
    cmdline->func_AdjustCmdLine( tr("SCIRC") , auxpoint , point , true , QString::number(KukaRob.MVel) , ldat);
    int y = codeeditor->func_GetCMDY(codeeditor->textCursor());
    cmdline->move(0,y);
    Btn_Change(4);
}

/*
** 函数功能：获取坐标点的数据信息
** 函数参数：svalue，指针参数，返回点的数据信息（字符串形式）
** 函数返回值：QStringList，返回点的数据信息
** 函数说明：无
*/
QStringList CodePad::func_GetPointInfo(QString* svalue)
{
    QStringList valuelist;
    valuelist.clear();
    double pos[14];
    //此处填充获取位置信息的函数
    func_getE6Pos(pos);
    for(int i = 0 ; i < 14 ; i++)
        valuelist<<QString::number(pos[i]);
    *svalue = "{ X "+QString::number(pos[0])+" , Y "+QString::number(pos[1])+" , Z "+QString::number(pos[2])+\
                     " , A "+QString::number(pos[3])+ " , B "+QString::number(pos[4])+" , C "+QString::number(pos[5])+\
                     " , S "+QString::number(pos[12])+" , T "+QString::number(pos[13])+" , E1 "+QString::number(pos[6])+\
                     " , E2 "+QString::number(pos[7])+" , E3 "+QString::number(pos[8])+" , E4 "+QString::number(pos[9])+
                     " , E5 "+QString::number(pos[10])+" , E6 "+QString::number(pos[11]) + " }";
    return valuelist;
}

/*
** 函数功能：获取FDAT的数据信息
** 函数参数：svalue，指针参数，返回FDAT的数据信息（字符串形式）
** 函数返回值：QStringList，返回FDAT的数据信息
** 函数说明：无
*/
QStringList CodePad::func_GetFDATInfo(QString *svalue)
{
    int tool,base,ipo;  //工具坐标，基座标，外部TCP
    QString ipo_frame;   //ipo模式
    QString auxpoint;   //辅助点坐标
    QString tq_state;  //外部碰撞
    QStringList valuelist;

    tool = cmdline->cb_TB[0]->currentIndex();
    base = cmdline->cb_TB[1]->currentIndex();
    ipo = cmdline->cb_TB[2]->currentIndex();
    if(ipo == 0)  //true
        ipo_frame = "#TCP";
    else  //false
        ipo_frame = "#BASE";
    if(cmdline->lineAuxPoint->isHidden())
        auxpoint = "\" \"";
    else
        auxpoint = " \"X"+cmdline->lineAuxPoint->text()+"\" ";
    tq_state = cmdline->cb_TB[3]->currentText();
    valuelist<<QString::number(tool)<<QString::number(base)<<QString::number(ipo)<<auxpoint<<tq_state;
    *svalue = "{ TOOL_NO "+QString::number(tool)+" , BASE_NO "+QString::number(base)+" , IPO_FRAME "+ipo_frame\
                                                               +" , POINT2[] "+auxpoint+" , TQ_STATE "+tq_state+ " }";
    return valuelist;
}

/*
** 函数功能：获取PDAT的数据信息
** 函数参数：svalue，指针参数，返回PDAT的数据信息（字符串形式）
** 函数返回值：QStringList，返回PDAT的数据信息
** 函数说明：无
*/
QStringList CodePad::func_GetPDATInfo(QString *svalue)
{
    QString vel,acc,dist;  //运动速度，加速度，圆滑过度距离
    QStringList valuelist;
    int apo_mode;
    vel = cmdline->lineSpd->text();
    acc = cmdline->lineAcce->text();
    if(cmdline->cb_cont->currentIndex() == 0)
        dist = "100.0";
    else
        dist = cmdline->lineDist->text();
    apo_mode = cmdline->cb_cont->currentIndex();

    valuelist<<vel<<acc<<dist<<QString::number(apo_mode);
    if(cmdline->cb_cont->currentIndex() == 1)
        *svalue = "{ VEL "+vel+" , ACC "+acc+" , APO_DIST "+dist+" , APO_MODE #CDIS }";
    else
        *svalue = "{ VEL "+vel+" , ACC "+acc+" , APO_DIST "+dist+" }";
    return valuelist;
}

/*
** 函数功能：获取LDAT的数据信息
** 函数参数：svalue，指针参数，返回LDAT的数据信息（字符串形式）
** 函数返回值：QStringList，返回LDAT的数据信息
** 函数说明：无
*/
QStringList CodePad::func_GetLDATInfo(QString *svalue)
{
    QString vel,acc,dist,apo_fac,axis_vel,axis_acc,ori_type,circ_type,jerk_fac,gear_jerk,exax_ign;
    QStringList valuelist;
    vel = cmdline->lineSpd->text();
    acc = cmdline->lineAcce->text();
    if(cmdline->cb_cont->currentIndex() == 0)
        dist = "100.0";
    else
        dist = cmdline->lineDist->text();
    apo_fac = "50.0";
    axis_vel =  "100.0";
    axis_acc = "100.0";
    ori_type = "#VAR";
    circ_type = "#BASE";
    jerk_fac = "50.0";
    gear_jerk = "50.0";
    exax_ign = "0";
    valuelist<<vel<<acc<<dist<<apo_fac<<axis_vel<<axis_acc<<ori_type<<circ_type<<jerk_fac<<gear_jerk<<exax_ign;
    *svalue = "{ VEL "+vel+" , ACC "+acc+" , APO_DIST "+dist+" , APO_FAC "+apo_fac+" , AXIS_VEL "+axis_vel\
                      +" , AXIS_ACC "+axis_acc+" , ORI_TYP "+ori_type+" , CIRC_TYP "+circ_type+" , JERK_FAC "+jerk_fac\
                      +" ,GEAR_JERK "+gear_jerk+" , EXAX_IGN "+exax_ign+" }";
    return valuelist;
}

/*
** 函数功能：更改指令
** 函数参数：当前光标
** 函数返回值：无
** 函数说明：无
*/
void CodePad::func_CMDChange(QTextCursor temptc)
{
    QString cmdcontent;   //整条指令的内容
    QStringList cmdlist;    //将指令内容按空格分开
    QString cmdtype;  //指令类型
    QString auxpoint;  //辅助点
    QString point;   //目标定
    bool cont = false;   //圆滑过渡
    QString vel;   //运动速度标签
    QString velspd;   //运动速度
    QString unit;  //速度单位
    QString data;  //pdata或ldata
    bool ok;

    temptc.movePosition(QTextCursor::StartOfBlock , QTextCursor::MoveAnchor);
    temptc.movePosition(QTextCursor::EndOfBlock , QTextCursor::KeepAnchor);
    cmdcontent = temptc.selectedText().simplified();
    codeeditor->setTextCursor(temptc);
    codeeditor->horizontalScrollBar()->setValue(0);

    cmdlist = cmdcontent.split(" ");
    if(cmdlist.isEmpty())
        return;

    cmdtype = cmdlist.at(0);
    if(cmdtype != "PTP" &&cmdtype != "LIN" &&cmdtype != "CIRC" &&cmdtype != "SLIN" &&cmdtype != "SCIRC")
    {
        QMessageBox::warning(this,"更改指令","该处的更改被拒绝，因为不能进行句选择。","确定");
        return;
    }
    int index = CMDToIndex.value(cmdtype);
    if(index >=0 && index < CNT_Cmd)
        cmdline->cb_cmd->setCurrentIndex(index);
    else
        return;

    int res,res1;
    int flag = 0;
    foreach (QString cmd, cmdlist) {
        /*首先判断其中的数据成员有哪些*/
        res = func_JudgeData(cmd);
        if(res == 8)   //坐标点数据
        {
            if(cmdtype == "CIRC" || cmdtype == "SCIRC")
            {
                if(flag == 0)
                {
                    auxpoint = cmd;
                    flag = 1;
                    continue;
                }
            }
            point = cmd;
            continue;
        }
        else if( res == 10 || res == 11)  //PDAT LDAT
        {
            data = cmd;
            continue;
        }
        /*判断标签控件中的内容*/
        res1 = func_JudgeLab(cmd);
        if(res1 == 5)
            vel = cmd;
        else if(res1 == 6)
            unit = cmd;
        /*判断速度值*/
        cmd.toDouble(&ok);
        if(ok)
            velspd = cmd;
    }
    /*判断有无圆滑过度*/
    if(cmdcontent.contains("CONT"))
        cont = true;
    cmdline->func_AdjustCmdLine(cmdtype,auxpoint,point,cont,velspd,data);
    int y = codeeditor->func_GetCMDY(codeeditor->textCursor());
    cmdline->move(0,y);
    Btn_Change(index);
}

/*
** 函数功能：判断此名称是否是程序数据
** 函数参数：当前光标
** 函数返回值：是，返回数据类型；否，-1
** 函数说明：无
*/
int CodePad::func_JudgeData(QString name)
{
    for(int i = 0 ; i < CNT_Type ; i++)
    {
        for(int j = 0 ; j < codedata[i].count() ; j++)
        {
            if(QString::compare(name , codedata[i][j].name , Qt::CaseSensitive) == 0)
                return i;
        }
    }
    return -1;
}

int CodePad::func_JudgeLab(QString name)
{
    if(name == "Vel=")
        return 5;
    else if(name == "m/s" || name == "%")
        return 6;
}

/*
** 函数功能：新建一个模块文件时，模块文件里的默认内容
** 函数参数：path，模块文件路径
** 函数返回值：无
** 函数说明：无
*/
void CodePad::init_ModuleFile(QString path)
{
    QFile file(path);
    if( !file.exists() )
        return;
    file.open(QIODevice::ReadWrite);
    QString str = tr("INI\n\nPTP HOME Vel= 100 % DEFAULT\n\nPTP HOME Vel= 100 % DEFAULT\n");
    QByteArray ba = str.toLatin1();
    char *mm = ba.data();
    file.write(mm);
    file.close();
}


/*
** 函数功能：将磁盘上的文件读入到程序编辑器中
** 函数参数：path，文件路径
** 函数返回值：读入成功返回1，失败返回0
** 函数说明：无
*/
int CodePad::func_ReadFileToEdit(QString path)
{
    QFile file(path);
    //如果模块文件存在并且打开成功，则将模块文件中的内容读进text中
    if( file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&file);
        codeeditor->clear();
        while(!in.atEnd())
        {
            codeeditor->appendPlainText(in.readLine());
        }
        return 1;
    }
    else
        return 0;
}

/*
** 函数功能：将程序编辑器text中的内容保存到路径为path的文件中
** 函数参数：path，保存的文件路径
** 函数返回值：无
** 函数说明：该函数将会首先清空该文件的所有内容，再保存text中的内容
*/
void func_SaveToFile(QPlainTextEdit *edit, QString path)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    file.close();

    file.resize(0);

    file.open(QIODevice::WriteOnly);
    out<<edit->toPlainText();
    file.close();
}

/*
** 函数功能：计算数据在文件中的声明字符串
** 函数参数：data，程序数据
** 函数返回值：数据的声明
** 函数说明：无
*/
QString CodePad::func_strInsertDataDecl(CodeData data)
{
    CodeDataType type;
    QString name,value;
    QString insertstr = "\rDECL ";
    type = data.dtype;
    name = data.name;
    value = data.Svalue;
    switch (type) {
    case booltype:

        break;
    case inttype:
        insertstr += tr("INT  ");
        break;
    case realtype:

        break;
    case chartype:

        break;
    case axistype:

        break;
    case e6axistype:

        break;
    case frametype:

        break;
    case postype:

        break;
    case e6postype:
        insertstr += "E6POS X"+name+" = ";
        break;
    case fdattype:
        insertstr += "FDAT "+name+" = ";
        break;
    case pdattype:
        insertstr += "PDAT P"+name+" = ";
        break;
    case ldattype:
        insertstr += "LDAT L"+name+" = ";
        break;
    case basedattype:

        break;
    case loaddattype:

        break;
    case tooldatatype:

        break;
    default:
        break;
    }
    insertstr += value;
    return insertstr;
}

/*
** 函数功能：根据程序数据找到其在文件中的声明
** 函数参数：程序数据
** 函数返回值：返回其声明
** 函数说明：无
*/
QString CodePad::func_calcDataDecl(CodeData data)
{
    CodeDataType type;
    QString name,value;
    QString insertstr = "DECL ";
    type = data.dtype;
    name = data.name;
    value = data.Svalue;
    switch (type) {
    case booltype:

        break;
    case inttype:
        insertstr += tr("INT  ");
        break;
    case realtype:

        break;
    case chartype:

        break;
    case axistype:

        break;
    case e6axistype:

        break;
    case frametype:

        break;
    case postype:

        break;
    case e6postype:
        insertstr += "E6POS X"+name+" = ";
        break;
    case fdattype:
        insertstr += "FDAT "+name+" = ";
        break;
    case pdattype:
        insertstr += "PDAT P"+name+" = ";
        break;
    case ldattype:
        insertstr += "LDAT L"+name+" = ";
        break;
    case basedattype:

        break;
    case loaddattype:

        break;
    case tooldatatype:

        break;
    default:
        break;
    }
    insertstr += value;
    return insertstr;
}

/*
** 函数功能：有新数据产生时，将数据声明写到数据文件中
** 函数参数：data，新程序数据
** 函数返回值：无
** 函数说明：无
*/
void CodePad::func_NewDataDecl(CodeData data)
{
    QString datpath,strInsert;
    QTextCursor temptc;
    QPlainTextEdit *tempedit;
    datpath = data.module+".dat";
    strInsert = func_strInsertDataDecl(data);
    tempedit = new QPlainTextEdit(this);
    tempedit->hide();
    tempedit->clear();
    QFile file(datpath);
    //如果数据文件存在并且打开成功，则将数据文件中的内容读进tempedit中
    if( file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&file);
        while(!in.atEnd())
            tempedit->appendPlainText(in.readLine());
    }
    else
        return;
    file.close();
    temptc = tempedit->textCursor();
    temptc.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
    tempedit->setTextCursor(temptc);
    tempedit->insertPlainText(strInsert);

    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    file.close();
    file.resize(0);
    file.open(QIODevice::WriteOnly);
    out<<tempedit->toPlainText();
    file.close();
}

/*
** 函数功能： 新建一条指令时，建立指令中的所有数据
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void CodePad::func_ConfirmCmdData()
{
    int type = cmdline->func_GetCmdType();
    if(cmdline->cb_TB[1]->currentIndex() == 0)
    {
        QMessageBox::warning(this,"插入指令","带基座标[0]的固定工具是不允许的。","确定");
        return;
    }
    func_BuildCMDData(type);
    QString cmdstr = cmdline->func_GetCmdStr(type);
    QTextCursor temptc = codeeditor->textCursor();
    temptc.movePosition(QTextCursor::EndOfBlock,QTextCursor::MoveAnchor);
    temptc.insertText("\n"+cmdstr);
    codeeditor->horizontalScrollBar()->setValue(0);
    func_SaveToFile(codeeditor,KukaRob.CurMName);
    Btn_Change(-2);
    /*关闭插入指令控件*/
    if(cmdline != NULL)
    {
        cmdline->hide();
        cmdline->frame_TcpBase->hide();
        cmdline->frame_SpdZone->hide();
        cmdline->frame_SpdDire->hide();
        if(listwidget.first() == cmdline->frame_TcpBase ||\
            listwidget.first() == cmdline->frame_SpdZone ||\
            listwidget.first() == cmdline->frame_SpdDire)
            listwidget.removeFirst();
        func_restoreFrame();
    }
}

/*
** 函数功能： 将一条指令转化成其在src中的存储样式
** 函数参数：cmd，指令类型；auxpoint，辅助点；point，目标点；pldat，PDAT或LDAT；fdat，FDAT
** 函数返回值：在src中的存储字符串
** 函数说明：无
*/
QString CodePad::func_CalcCmdDecl(int cmd, QString auxpoint, QString point,QString pldat,QString fdat)
{
    QString res;
    if(cmd == 0)  //PTP
        res = "PDAT_ACT=P"+pldat+"\nFDAT_ACT="+fdat+"\nPTP X"+point;
    else if(cmd == 1)  //LIN
        res = "LDAT_ACT=L"+pldat+"\nFDAT_ACT="+fdat+"\nLIN X"+point;
    else if(cmd == 2)  //CIRC
        res = "LDAT_ACT=L"+pldat+"\nFDAT_ACT="+fdat+"\nCIRC X"+auxpoint+" , X"+point;
    return res;
}

/*
** 函数功能： 将一条指令拆分成元数据
** 函数参数：cmdline，整条指令内容；cmdtype，指令类型；auxpoint，辅助点；point，目标点；pldat，PDAT或LDAT；fdat，FDAT
** 函数返回值：成功，1；失败，-1
** 函数说明：无
*/
int CodePad::func_extractCmdData(QString cmdline, QString *cmdtype, QString *auxpoint, QString *point, QString *pldata, QString *fdat)
{
    QStringList cmdlist;    //将指令内容按空格分开
    QRegExp rg("(PTP|LIN|CIRC|SLIN|SCIRC)");
    if(!cmdline.contains(rg))
        return -1;
    cmdlist = cmdline.split(" ");
    if(cmdlist.isEmpty())
        return -1;
    *cmdtype = cmdlist.at(0);
    if(!(*cmdtype).contains(rg))
        return -1;
    int res;
    int flag = 0;
    foreach (QString cmd, cmdlist) {
        /*首先判断其中的数据成员有哪些*/
        res = func_JudgeData(cmd);
        if(res == 8)   //坐标点数据
        {
            if(*cmdtype == "CIRC" || *cmdtype == "SCIRC")
            {
                if(flag == 0)
                {
                    *auxpoint = cmd;
                    flag = 1;
                    continue;
                }
            }
            *point = cmd;
            *fdat = "F"+*point;
            continue;
        }
        else if( res == 10 || res == 11)  //PDAT LDAT
        {
            *pldata = cmd;
            continue;
        }
    }
    return 1;
}

/*
** 函数功能： 将编辑器中的内容转换成src文件
** 函数参数：无
** 函数返回值：无
** 函数说明：每次译码之前先执行此函数
*/
void CodePad::func_editFileToSrcFile()
{
    QString srcpath,strline,basename;
    QString cmdinsert;
    int lineedit,linesrc;

    EditToSrc.clear();
    QString strtemp = KukaRob.CurMName;
    if(strtemp.contains(".dat"))
        return;
    srcpath = KukaRob.CurMName+".src";
    QFileInfo tempfile(srcpath);
    basename = tempfile.baseName();
    QPlainTextEdit *tempedit;  //记载转换出来的src文件编辑器
    QTextCursor temptc,tempsrctc;  //当前程序编辑器中的指针和记载src文件编辑器中的指针
    temptc = codeeditor->textCursor();
    temptc.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
    temptc.movePosition(QTextCursor::EndOfBlock,QTextCursor::KeepAnchor);
    strline = temptc.selectedText();
    tempedit = new QPlainTextEdit(this);
    tempedit->clear();
    tempedit->hide();
    tempedit->appendPlainText("DEF "+basename+"( )");
    while(!temptc.atEnd())
    {
        cmdinsert = func_editStrToSrcStr(strline);
        if(cmdinsert.compare("NULL") == 0)
            return;
        tempsrctc = tempedit->textCursor();
        lineedit = temptc.blockNumber()+1;
        linesrc = tempsrctc.blockNumber()+2;
        qDebug()<<lineedit<<linesrc;
        tempedit->appendPlainText(cmdinsert);
        temptc.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor);
        temptc.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
        temptc.movePosition(QTextCursor::EndOfBlock,QTextCursor::KeepAnchor);
        strline = temptc.selectedText();
        EditToSrc.insert(linesrc,lineedit);
    }
    cmdinsert = func_editStrToSrcStr(strline);
    if(cmdinsert.isEmpty())
        return;
    tempsrctc = tempedit->textCursor();
    lineedit = temptc.blockNumber()+1;
    linesrc = tempsrctc.blockNumber()+2;
    qDebug()<<lineedit<<linesrc;
    tempedit->appendPlainText(cmdinsert);
    tempedit->appendPlainText("END");
    func_SaveToFile(tempedit,srcpath);
}

QString CodePad::func_editStrToSrcStr(QString strline)
{
    QRegExp rg("(PTP|LIN|CIRC|SLIN|SCIRC)");
    QString cmdtype,auxpoint,point,pldata,fdat;
    QString strsrc,cmddecl,cmdinsert;
    int res,cmd;

    strsrc = "NULL";
    if(strline.contains(rg))
    {
        res = func_extractCmdData(strline,&cmdtype,&auxpoint,&point,&pldata,&fdat);
        if(res == -1)
            return strsrc;
        cmd = CMDToIndex.value(cmdtype);
        cmddecl = func_CalcCmdDecl(cmd,auxpoint,point,pldata,fdat);
        cmdinsert = ";FOLD "+strline+"\n"+cmddecl+"\n;ENDFOLD";
    }
    else if(strline == "INI")
        cmdinsert = ";FOLD "+strline+"\n;ENDFOLD (INI)";
    else
        cmdinsert = strline.simplified();
    return cmdinsert;
}

int func_srcLineToEdit(int srcline)
{
    if(EditToSrc.contains(srcline))
        return srcline;
    int srcedit;
    srcedit = EditToSrc.value(srcline);
    return srcedit;
}

void CodePad::func_ChangeCmdData()
{
    int type = cmdline->func_GetCmdType();
    if(cmdline->cb_TB[1]->currentIndex() == 0)
    {
        QMessageBox::warning(this,"插入指令","带基座标[0]的固定工具是不允许的。","确定");
        return;
    }
    func_BuildCMDData(type);
    QString cmdstr = cmdline->func_GetCmdStr(type);
    QTextCursor temptc = codeeditor->textCursor();
    temptc.insertText(cmdstr);
    codeeditor->horizontalScrollBar()->setValue(0);
    Btn_Change(-2);
    /*关闭插入指令控件*/
    if(cmdline != NULL)
    {
        cmdline->hide();
        cmdline->frame_TcpBase->hide();
        cmdline->frame_SpdZone->hide();
        cmdline->frame_SpdDire->hide();
        if(listwidget.first() == cmdline->frame_TcpBase ||\
            listwidget.first() == cmdline->frame_SpdZone ||\
            listwidget.first() == cmdline->frame_SpdDire)
            listwidget.removeFirst();
        func_restoreFrame();
    }
}

void CodePad::func_ConfirmPara()
{
    QTextCursor temptc;
    QString strcontent;
    QString auxpoint;  //辅助点
    QString point;   //目标定
    QString fdata;
    QString cmdtype;
    QStringList list;
    QString indext,indexb,tcp;
    CodeData oldpoint,oldauxpoint;
    int res;
    int flag = 0;
    bool ok;
    QRegExp rg("(PTP|LIN|CIRC|SLIN|SCIRC)");
    temptc = codeeditor->textCursor();
    strcontent = codeeditor->func_LineContent().simplified();
    if( !strcontent.contains(rg) )
        return;
    list = strcontent.split(" ");
    if(list.isEmpty())
        return;
    cmdtype = list.at(0);
    foreach (QString cmd, list) {
        res = func_JudgeData(cmd);
        if(res == 8)   //坐标点数据
        {
            if(cmdtype == "CIRC" || cmdtype == "SCIRC")
            {
                if(flag == 0)
                {
                    auxpoint = cmd;
                    flag = 1;
                    continue;
                }
            }
            point = cmd;
            continue;
        }
    }
    oldpoint = func_getDataByName(point,&ok);
    if(!ok)
        return;
    if(cmdtype == "SCIRC" || cmdtype == "CIRC")
    {
        oldauxpoint = func_getDataByName(auxpoint,&ok);
        if(!ok)
            return;
    }
    fdata = "F"+point;
    CodeData fdat = func_getDataByName(fdata,&ok);
    if(!ok)
        return;
    indext = QString::number(fdat.val.fdatVal.TOOL_NO);
    indexb = QString::number(fdat.val.fdatVal.BASE_NO);
    if(fdat.val.fdatVal.IPO_FRAME == 0)
        tcp = "#TCP)\r";
    else
        tcp = "#BASE)\r";
    if(cmdtype == "PTP" || cmdtype == "LIN" || cmdtype == "SLIN")
    {
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString str = time.toString("hh:mm:ss yyyy/MM/dd"); //设置显示格式
        int res = QMessageBox::question(this,str+"  KUKATPBASIS 102","\r确实要接受点“ "+point+" ”？"\
                                        "(TOOL_DATA["+indext+"],BASE_DATA["+indexb+"],"+tcp,"是","否");
        if(res == 1)
            return;
        else
        {
            QString pointsvalue;   //目标点
            QStringList pointvallist = func_GetPointInfo(&pointsvalue);
            CodeData newdata;
            newdata.name = point;
            newdata.Svalue = pointsvalue;
            newdata.dtype = e6postype;
            newdata.module = KukaRob.CurMName;
            newdata.code = "";
            newdata.dimension = 1;
            newdata.val.e6posVal.sete6posval(pointvallist);
            func_changeData(oldpoint,newdata);
        }
    }
    else
    {
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString str = time.toString("hh:mm:ss yyyy/MM/dd"); //设置显示格式
        int res = QMessageBox::question(this,str+"  KUKATPBASIS 103","\r是否接受目标点或辅助点？ \r","退出","辅助点","目标点");
        if(res == 0)
            return;
        else if(res == 1 )
        {
            QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
            QString str = time.toString("hh:mm:ss yyyy/MM/dd"); //设置显示格式
            int res1 = QMessageBox::question(this,str+"  KUKATPBASIS 102","\r确实要接受点“ "+auxpoint+" ”？"\
                                            "(TOOL_DATA["+indext+"],BASE_DATA["+indexb+"],"+tcp,"是","否");
            if(res1 == 1)
                return;
            else
            {
                QString auxpointsvalue;
                QStringList auxpointvallist = func_GetPointInfo(&auxpointsvalue);
                CodeData newdata;
                newdata.name = auxpoint;
                newdata.Svalue = auxpointsvalue;
                newdata.dtype = e6postype;
                newdata.module = KukaRob.CurMName;
                newdata.code = "";
                newdata.dimension = 1;
                newdata.val.e6posVal.sete6posval(auxpointvallist);
                func_changeData(oldauxpoint,newdata);
            }
        }
        else if(res == 2 )
        {
            QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
            QString str = time.toString("hh:mm:ss yyyy/MM/dd"); //设置显示格式
            int res1 = QMessageBox::question(this,str+"  KUKATPBASIS 102","\r确实要接受点“ "+point+" ”？"\
                                            "(TOOL_DATA["+indext+"],BASE_DATA["+indexb+"],"+tcp,"是","否");
            if(res1 == 1)
                return;
            else
            {
                QString pointsvalue;   //目标点
                QStringList pointvallist = func_GetPointInfo(&pointsvalue);
                CodeData newdata;
                newdata.name = point;
                newdata.Svalue = pointsvalue;
                newdata.dtype = e6postype;
                newdata.module = KukaRob.CurMName;
                newdata.code = "";
                newdata.dimension = 1;
                newdata.val.e6posVal.sete6posval(pointvallist);
                func_changeData(oldpoint,newdata);
            }
        }
    }
}


void CodePad::func_DelRow(QTextCursor temptc)
{
    temptc.movePosition(QTextCursor::StartOfBlock , QTextCursor::MoveAnchor);
    temptc.movePosition(QTextCursor::EndOfBlock , QTextCursor::KeepAnchor);
    if(temptc.selectedText().contains("INI" , Qt::CaseInsensitive))
    {
        QMessageBox::warning(this,"删除行","语句不允许被删除","确定");
        return;
    }
    int res = QMessageBox::question(this,"删除行","确实要删除行吗？","是","否","退出");
    if(res == 1 || res == 2)
        return;
    else if(res == 0)
    {
        temptc.removeSelectedText();
        temptc.movePosition(QTextCursor::Left , QTextCursor::KeepAnchor);
        temptc.removeSelectedText();
        temptc.movePosition(QTextCursor::Down , QTextCursor::MoveAnchor);
        codeeditor->setTextCursor(temptc);
        codeeditor->func_SelectLine();
        codeeditor->horizontalScrollBar()->setValue(0);
    }
}

void CodePad::func_restoreFrame()
{
    if(frame_base->height() == 372)
        return;
    frame_base->move(0,0);
    frame_base->resize(494,694);
    codeeditor->resize(494,584);
    lab_ctlframe->move(0,614);
}

void CodePad::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    if(frame_cmd != NULL)
        frame_cmd->mousePressEvent(e);
    if(frame_edit != NULL)
        frame_edit->mousePressEvent(e);
}

void CodePad::setColor0()
{
    for(int i=0;i<6;i++)
    {
        btnFrame0[i]->setPalette(old_pa);
        btnFrame0[i]->setStyleSheet(old_stysheet);
    }
    for(int i=0;i<7;i++)
    {
        btnFrame1[i]->setPalette(old_pa);
        btnFrame1[i]->setStyleSheet(old_stysheet);
    }
    for(int i=0;i<4;i++)
    {
        btnFrame2[i]->setPalette(old_pa);
        btnFrame2[i]->setStyleSheet(old_stysheet);
    }
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    for(int i=0;i<6;i++)
    {
        if(btnFrame0[i] == sender())
        {
            btnFrame0[i]->setStyleSheet(new_stylesheet);
            btnFrame0[i]->setPalette(new_pa);
            break;
        }
    }
}

void CodePad::setColor1()
{
    for(int i=0;i<7;i++)
    {
        btnFrame1[i]->setPalette(old_pa);
        btnFrame1[i]->setStyleSheet(old_stysheet);
    }
    for(int i=0;i<4;i++)
    {
        btnFrame2[i]->setPalette(old_pa);
        btnFrame2[i]->setStyleSheet(old_stysheet);
    }

    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    for(int i=0;i<7;i++)
    {
        if(btnFrame1[i] == sender())
        {
            btnFrame1[i]->setStyleSheet(new_stylesheet);
            btnFrame1[i]->setPalette(new_pa);
            break;
        }
    }
}

void CodePad::setEditColor()
{
    for(int i=0;i<14;i++)
    {
        btn_edit0[i]->setPalette(old_pa);
        btn_edit0[i]->setStyleSheet(old_stysheet);
    }
    for(int i=0;i<6;i++)
    {
        btn_edit1[i]->setPalette(old_pa);
        btn_edit1[i]->setStyleSheet(old_stysheet);
    }

    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    for(int i=0;i<14;i++)
    {
        if(btn_edit0[i] == sender())
        {
            btn_edit0[i]->setStyleSheet(new_stylesheet);
            btn_edit0[i]->setPalette(new_pa);
            break;
        }
    }
}

/*
** 函数功能：取消选择程序
** 函数参数：无
** 函数返回值：无
** 函数说明：取消选择程序将关闭程序编辑界面并进行保存和语法检查
*/
void CodePad::func_codedelselect()
{
    func_SaveToFile(codeeditor,KukaRob.CurMName);
    if(!(KukaRob.CurMName.contains(".src")))
    {
        funcParser->startParser(KukaRob.CurMName);
    }
    this->hide();
    if(listwidget.first() == this)
        listwidget.removeFirst();
}

void CodePad::slot_resizeFrame()
{
    if(frame_base->height()==372)
        return;
    frame_base->move(0,250);
    frame_base->resize(494,444);
    codeeditor->resize(494,334);
    lab_ctlframe->move(0,364);
}

void CodePad::slot_changeBtn(int i)
{
    Btn_Change(i);
}

void CodePad::slot_aboutToHide()
{
    MenuWidget *wid = qobject_cast<MenuWidget*>(sender());
    if( !wid )
        return;
    if(wid == frame_cmd)
        btnCtlFunc[1]->setChecked(false);
    else if(wid == frame_edit)
        btnCtlFunc[6]->setChecked(false);
}

void CodePad::slot_aboutToShow(MenuWidget *wid)
{
    if(wid == frame_edit)
    {
        if(frame_cmd != NULL)
            frame_cmd->hide();
    }
    else if(wid == frame_cmd)
    {
        if(frame_edit != NULL)
            frame_edit->hide();
    }
}

/*
** 函数功能：接收有新数据产生信号的槽函数
** 函数参数：data，新程序数据
** 函数返回值：无
** 函数说明：无
*/
void CodePad::slot_funcNewData(CodeData data)
{
    func_NewDataDecl(data);
}

void CodePad::slot_funcDelData(CodeData data)
{

}

void CodePad::slot_BtnCtlFunc(bool state)
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if(!btn)
        return;
    if(KukaRob.CurMName.contains(".dat"))
        return;
    if(btn == btnCtlFunc[0])
    {
        if( !btnCtlFunc[1]->isEnabled() )   //中断指令
        {
            if(cmdline != NULL)
            {
                cmdline->hide();
                cmdline->frame_TcpBase->hide();
                cmdline->frame_SpdZone->hide();
                cmdline->frame_SpdDire->hide();
                if(listwidget.first() == cmdline->frame_TcpBase ||\
                    listwidget.first() == cmdline->frame_SpdZone ||\
                    listwidget.first() == cmdline->frame_SpdDire)
                    listwidget.removeFirst();
                func_restoreFrame();
            }
            Btn_Change(-2);
        }
        else   //更改
        {
            flag_nc = 1;
            func_CMDChange(codeeditor->textCursor());
        }
    }
    else if(btn == btnCtlFunc[1])   //指令
    {
        flag_nc = 0;
        if(state)
            func_showCmdFrame();
        else
            func_hideCmdFrame();
    }
    else if(btn == btnCtlFunc[2])
    {
        func_PTP();
    }
    else if(btn == btnCtlFunc[3])
    {

    }
    else if(btn == btnCtlFunc[4])   //语句行选择
    {
        if( btnCtlFunc[1]->isEnabled() )
            func_SelectRow();
    }
    else if(btn == btnCtlFunc[5])
    {
        if(btnCtlFunc[1]->isEnabled())   //确定参数
            func_ConfirmPara();
        else   //目标点坐标
        {

        }
    }
    else if(btn == btnCtlFunc[6])
    {
        if( btnCtlFunc[1]->isEnabled())  //编辑
        {
            func_showEditFrame();
        }
        else   //指令OK
        {
            if(flag_nc == 0)
                func_ConfirmCmdData();
            else
                func_ChangeCmdData();
        }
    }
}

void CodePad::slot_BtnFrame0()
{
    frameBtn[2]->hide();
    lab1->hide();
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    if(btn == btnFrame0[1])
    {
        frame_cmd->resize(335,314);
        frameBtn[1]->show();
        for(int i=0;i<7;i++)
            btnFrame1[i]->show();
        btnFrame1[0]->setText(tr("PTP"));
        btnFrame1[1]->setText(tr("LIN"));
        btnFrame1[2]->setText(tr("CIRC"));
        btnFrame1[3]->setText(tr("样条组"));
        btnFrame1[4]->setText(tr("SPL"));
        btnFrame1[5]->setText(tr("SLIN"));
        btnFrame1[6]->setText(tr("SCIRC"));
    }
    else if(btn == btnFrame0[2])
    {
        frame_cmd->resize(335,314);
        frameBtn[1]->show();
        btnFrame1[0]->show();
        btnFrame1[0]->setText(tr("力矩监控"));
        for(int i=1;i<7;i++)
            btnFrame1[i]->hide();
    }
    else if(btn == btnFrame0[3])
    {
        frame_cmd->resize(335,314);
        frameBtn[1]->show();
        for(int i=0;i<5;i++)
            btnFrame1[i]->show();
        for(int i=5;i<7;i++)
            btnFrame1[i]->hide();
        btnFrame1[0]->setText(tr("WAIT"));
        btnFrame1[1]->setText(tr("WAITFOR"));
        btnFrame1[2]->setText(tr("OUT"));
        btnFrame1[3]->setText(tr("耦合/断开联络总线段"));
        btnFrame1[4]->setText(tr("样条曲线触发器"));
        setArrow(lab1,btnFrame1[2]);
    }
    else if(btn == btnFrame0[4])
    {
        frame_cmd->resize(335,314);
        frameBtn[1]->show();
        for(int i=0;i<2;i++)
            btnFrame1[i]->show();
        for(int i=2;i<7;i++)
            btnFrame1[i]->hide();
        btnFrame1[0]->setText(tr("静态"));
        btnFrame1[1]->setText(tr("动态"));
    }
    else if(btn == btnFrame0[5])
    {
        frame_cmd->resize(335,314);
        frameBtn[1]->show();
        for(int i=0;i<2;i++)
            btnFrame1[i]->show();
        for(int i=2;i<7;i++)
            btnFrame1[i]->hide();
        btnFrame1[0]->setText(tr("标准"));
        btnFrame1[1]->setText(tr("印章"));
    }
}

void CodePad::slot_BtnFrame1()
{
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    if(btn == btnFrame1[0])
    {
        frame_cmd->hide();
        if(btn->text() == tr("PTP"))
        {
            func_PTP();
        }
    }
    else if(btn == btnFrame1[1])
    {
        frame_cmd->hide();
        if(btn->text() == tr("LIN"))
        {
            func_LIN();
        }
    }
    else if(btn == btnFrame1[2])
    {
        if(btn->text() == tr("CIRC"))
        {
            frame_cmd->hide();
            func_CIRC();
        }
        else if(btn->text() == tr("OUT"))
        {
            frame_cmd->resize(494,314);
            frameBtn[2]->show();
            btnFrame2[0]->setText(tr("OUT"));
            btnFrame2[1]->setText(tr("脉冲"));
            btnFrame2[2]->setText(tr("SYN OUT"));
            btnFrame2[3]->setText(tr("SYN PULSE"));
        }
    }
    else if(btn == btnFrame1[5])
    {
        frame_cmd->hide();
        if(btn->text() == tr("SLIN"))
        {
            func_SLIN();
        }
    }
    else if(btn == btnFrame1[6])
    {
        frame_cmd->hide();
        if(btn->text() == tr("SCIRC"))
        {
            func_SCIRC();
        }
    }
}

void CodePad::slot_BtnFrame2()
{

}


void CodePad::slot_EditBtn()
{
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    if(btn == btn_edit0[0])
    {
        frame_edit->resize(330,580);
        frame_edit->move(164,60);
        frameBtnEdit[1]->show();
        for(int i = 0 ; i < 6 ;i++)
            btn_edit1[i]->hide();
        for(int i=0;i<3;i++)
            btn_edit1[i]->show();
        btn_edit1[0]->setText(tr("打开/关闭当前FOLD"));
        btn_edit1[1]->setText(tr("打开所有的FOLD"));
        btn_edit1[2]->setText(tr("关闭所有的FOLD"));
    }
    else if(btn == btn_edit0[1])
    {

    }
    else if(btn == btn_edit0[2])
    {

    }
    else if(btn == btn_edit0[3])
    {

    }
    else if(btn == btn_edit0[4])
    {

    }
    else if(btn == btn_edit0[5])    //删除
    {
        func_DelRow(codeeditor->textCursor());
    }
    else if(btn == btn_edit0[6])
    {

    }
    else if(btn == btn_edit0[7])
    {

    }
    else if(btn == btn_edit0[8])
    {

    }
    else if(btn == btn_edit0[9])
    {
        frame_edit->resize(330,580);
        frame_edit->move(164,60);
        frameBtnEdit[1]->show();
        for(int i=0;i<6;i++)
            btn_edit1[i]->show();
        btn_edit1[0]->setText(tr("笛卡尔式距离"));
        btn_edit1[1]->setText(tr("平移笛卡尔基座标系"));
        btn_edit1[2]->setText(tr("平移笛卡尔工具座标系"));
        btn_edit1[3]->setText(tr("平移笛卡尔世界座标系"));
        btn_edit1[4]->setText(tr("平移——与轴相关的"));
        btn_edit1[5]->setText(tr("轴缩影"));
    }
    else if(btn == btn_edit0[10])
    {
        frame_edit->resize(330,580);
        frame_edit->move(164,60);
        frameBtnEdit[1]->show();
        for(int i = 0 ; i < 6 ;i++)
            btn_edit1[i]->hide();
        for(int i=0;i<4;i++)
            btn_edit1[i]->show();
        btn_edit1[0]->setText(tr("DEF行"));
        btn_edit1[1]->setText(tr("详细显示（ASCII）"));
        btn_edit1[2]->setText(tr("换行"));
        btn_edit1[3]->setText(tr("普通/小行间距"));
    }
    else if(btn == btn_edit0[11])
    {

    }
    else if(btn == btn_edit0[12])
    {

    }
    else if(btn == btn_edit0[13])
    {

    }
    else if(btn == btn_edit1[0])
    {

    }
    else if(btn == btn_edit1[1])
    {

    }
    else if(btn == btn_edit1[2])
    {

    }
    else if(btn == btn_edit1[3])
    {

    }
    else if(btn == btn_edit1[4])
    {

    }
    else if(btn == btn_edit1[5])
    {

    }
}

/*
** 函数功能：判断此变量名称是否已经存在
** 函数参数：name，变量名
** 函数返回值：存在返回1，不存在返回-1
*/
int func_JudgeDataExist(QString name)
{
    for(int i = 0 ;i < CNT_Type ; i++)
    {
        for(int j = 0 ; j < codedata[i].count() ; j++)
        {
            if(QString::compare(name , codedata[i][j].name) == 0)
                return 1;
        }
    }
    return -1;
}

/*
** 函数功能：检查变量名是否合乎标准
** 函数参数：name，变量名
** 函数返回值：合法返回true，非法返回false
*/
bool VarIsStandable(QString name)
{
    if(rg_var.exactMatch(name))
        return true;
    else
        return false;
}

/*
** 函数功能：返回程序数据的下标
** 函数参数：name，数据名；type，数据类型
** 函数返回值：成功：下标；失败：-1
*/
int func_getDataIndex(QString name,QString type)
{
    if( !TypeToIndex.contains(type) )
        return -1;
    int k = TypeToIndex.value(type);
    for(int i = 0 ; i < codedata[k].count() ; i++)
    {
        if( name == codedata[k][i].name)
            return i;
    }
    return -1;
}


/*
** 函数功能：读取声明在数据文件下的程序数据
** 函数参数：数据文件的路径
** 函数返回值：无
** 函数说明：无
*/
int func_ReadCodeDataFromFile(QString path)
{
    QFile file(path);
    QString modname = path;    //数据所属模块名
    modname = modname.remove(".dat");
    if( file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&file);
        QString linestr;
        while( !in.atEnd() )
        {
            linestr.clear();
            linestr = in.readLine();
            int fvar = -1;
            if(linestr.contains("DECL E6POS"))
            {
                QString svalue;
                int indexl,indexr;
                indexl = linestr.indexOf("{");
                indexr = linestr.indexOf("}");
                if(indexl < 0 || indexr < 0)
                    continue;
                svalue = linestr.mid(indexl);
                int typeindex = TypeToIndex.value("E6POS");
                linestr = linestr.simplified();
                QStringList datalist = linestr.split(" ");
                for(int i = 0 ; i < datalist.count() ; i++)
                {
                    if(datalist[i] == "=")
                    {
                        fvar = i;
                        break;
                    }
                }
                if(fvar != 3)
                    continue;
                QString name = datalist[fvar-1].remove(0,1);
                CodeData tempdata;
                tempdata.name = name;
                tempdata.Svalue = svalue;
                tempdata.dtype = e6postype;
                tempdata.stotype = Variable;
                tempdata.module = modname;
                tempdata.dimension = 1;

                svalue = svalue.remove("{");
                svalue = svalue.remove("}");
                svalue = svalue.remove(",");
                svalue = svalue.simplified();
                QStringList ori_list = svalue.split(" ");  //原始数据
                if(ori_list.count() != 28)
                    continue;
                QStringList new_list;
                for(int i=0;i<ori_list.count();i++)
                {
                    QString temp;
                    if(i%2 == 1)
                    {
                        temp = ori_list.at(i);
                        new_list<<temp;  //经过处理后的数据，只剩下有效数据
                    }
                }
                tempdata.val.e6posVal.sete6posval(new_list);
                if(!VarIsStandable(name))
                    continue;
                if(func_JudgeDataExist(name) == 1)   //有重名数据
                    continue;
                codedata[typeindex].append(tempdata);
            }
            else if(linestr.contains("DECL FDAT"))
            {
                QString svalue;
                int indexl,indexr;
                indexl = linestr.indexOf("{");
                indexr = linestr.indexOf("}");
                if(indexl < 0 || indexr < 0)
                    continue;
                svalue = linestr.mid(indexl);
                int typeindex = TypeToIndex.value("FDAT");
                linestr = linestr.simplified();
                QStringList datalist = linestr.split(" ");
                for(int i = 0 ; i < datalist.count() ; i++)
                {
                    if(datalist[i] == "=")
                    {
                        fvar = i;
                        break;
                    }
                }
                if(fvar != 3)
                    continue;
                QString name = datalist[fvar-1];
                CodeData tempdata;
                tempdata.name = name;
                tempdata.Svalue = svalue;
                tempdata.dtype = fdattype;
                tempdata.stotype = Variable;
                tempdata.module = modname;
                tempdata.dimension = 1;

                svalue = svalue.remove("{");
                svalue = svalue.remove("}");
                svalue = svalue.simplified();
                QStringList ori_list = svalue.split(",");  //原始数据
                if(ori_list.count() != 5)
                    continue;
                QStringList new_list;
                QString str[5];
                for(int i = 0 ; i < 5 ; i++)
                    str[i] = ori_list[i];
                str[0].remove(" ");
                str[0].remove("TOOL_NO");
                new_list.append(str[0]);

                str[1].remove(" ");
                str[1].remove("BASE_NO");
                new_list.append(str[1]);

                str[2].remove(" ");
                str[2].remove("IPO_FRAME");
                if(str[2] == "#TCP")
                    new_list.append("0");
               else if(str[2] == "#BASE")
                    new_list.append("1");

                str[3].remove(" ");
                str[3].remove("POINT2[]");
                str[3].remove("\"");
                new_list.append(str[3]);

                str[4].remove(" ");
                str[4].remove("TQ_STATE");
                new_list.append(str[4]);

                tempdata.val.fdatVal.setfdatval(new_list);
                if(!VarIsStandable(name))
                    continue;
                if(func_JudgeDataExist(name) == 1)   //有重名数据
                    continue;
                codedata[typeindex].append(tempdata);
            }
            else if(linestr.contains("DECL PDAT"))
            {
                QString svalue;
                int indexl,indexr;
                indexl = linestr.indexOf("{");
                indexr = linestr.indexOf("}");
                if(indexl < 0 || indexr < 0)
                    continue;
                svalue = linestr.mid(indexl);
                int typeindex = TypeToIndex.value("PDAT");
                linestr = linestr.simplified();
                QStringList datalist = linestr.split(" ");
                for(int i = 0 ; i < datalist.count() ; i++)
                {
                    if(datalist[i] == "=")
                    {
                        fvar = i;
                        break;
                    }
                }
                if(fvar != 3)
                    continue;
                QString name = datalist[fvar-1].remove(0,1);
                CodeData tempdata;
                tempdata.name = name;
                tempdata.Svalue = svalue;
                tempdata.dtype = pdattype;
                tempdata.stotype = Variable;
                tempdata.module = modname;
                tempdata.dimension = 1;

                svalue = svalue.remove("{");
                svalue = svalue.remove("}");
                svalue = svalue.remove(",");
                svalue = svalue.simplified();
                QStringList ori_list = svalue.split(" ");  //原始数据
                if(ori_list.count() != 6 && ori_list.count() != 8)
                    continue;
                QStringList new_list;
                for(int i = 0 ; i < ori_list.count() ; i++)
                {
                    QString temp;
                    if(i%2 == 1)
                    {
                        temp = ori_list.at(i);
                        new_list<<temp;  //经过处理后的数据，只剩下有效数据
                    }
                }
                if(ori_list.count() == 6)
                    new_list<<"0";
                else
                    new_list[3] = "1";
                tempdata.val.pdatVal.setpdatval(new_list);
                if(!VarIsStandable(name))
                    continue;
                if(func_JudgeDataExist(name) == 1)   //有重名数据
                    continue;
                codedata[typeindex].append(tempdata);
            }
            else if(linestr.contains("DECL LDAT"))
            {
                QString svalue;
                int indexl,indexr;
                indexl = linestr.indexOf("{");
                indexr = linestr.indexOf("}");
                if(indexl < 0 || indexr < 0)
                    continue;
                svalue = linestr.mid(indexl);
                int typeindex = TypeToIndex.value("LDAT");
                linestr = linestr.simplified();
                QStringList datalist = linestr.split(" ");
                for(int i = 0 ; i < datalist.count() ; i++)
                {
                    if(datalist[i] == "=")
                    {
                        fvar = i;
                        break;
                    }
                }
                if(fvar != 3)
                    continue;
                QString name = datalist[fvar-1].remove(0,1);
                CodeData tempdata;
                tempdata.name = name;
                tempdata.Svalue = svalue;
                tempdata.dtype = ldattype;
                tempdata.stotype = Variable;
                tempdata.module = modname;
                tempdata.dimension = 1;

                svalue = svalue.remove("{");
                svalue = svalue.remove("}");
                svalue = svalue.remove(",");
                svalue = svalue.simplified();
                QStringList ori_list = svalue.split(" ");  //原始数据
                if(ori_list.count() != 22)
                    continue;
                QStringList new_list;
                for(int i=0;i<ori_list.count();i++)
                {
                    QString temp;
                    if(i%2 == 1)
                    {
                        temp = ori_list.at(i);
                        new_list<<temp;  //经过处理后的数据，只剩下有效数据
                    }
                }
                tempdata.val.ldatVal.setldatval(new_list);
                if(!VarIsStandable(name))
                    continue;
                if(func_JudgeDataExist(name) == 1)   //有重名数据
                    continue;
                codedata[typeindex].append(tempdata);
            }
        }
        file.close();
    }
}

int func_ReadRobSysDataFromFile(QString path)
{
    QFile file(path);
    if(!file.exists())
    {
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    if( file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&file);
        QString linestr;
        while( !in.atEnd() )
        {
            linestr.clear();
            linestr = in.readLine();
            int fvar = -1;
            if(linestr.contains("DECL TOOLDAT"))
            {
                QString svalue;
                int indexl,indexr,toolno;
                bool ok;
                indexl = linestr.indexOf("{");
                indexr = linestr.indexOf("}");
                if(indexl < 0 || indexr < 0)
                    continue;
                svalue = linestr.mid(indexl);
                int typeindex = TypeToIndex.value("TOOLDAT");
                linestr = linestr.simplified();
                QStringList datalist = linestr.split(" ");
                for(int i = 0 ; i < datalist.count() ; i++)
                {
                    if(datalist[i] == "=")
                    {
                        fvar = i;
                        break;
                    }
                }
                if(fvar != 4)
                    continue;
                QString name = datalist[fvar-1];
                toolno = datalist[fvar-2].toInt(&ok);
                if(!ok)
                    continue;
                if(toolno < 0 || toolno >=16)
                    continue;
                CodeData tempdata;
                tempdata.name = name;
                tempdata.Svalue = svalue;
                tempdata.dtype = tooldatatype;
                tempdata.stotype = Variable;
                tempdata.module = "";
                tempdata.dimension = 1;

                svalue = svalue.remove("{");
                svalue = svalue.remove("}");
                svalue = svalue.remove(",");
                svalue = svalue.simplified();
                QStringList ori_list = svalue.split(" ");  //原始数据
                if(ori_list.count() != 32)
                    continue;
                QStringList new_list;
                for(int i=0;i<ori_list.count();i++)
                {
                    QString temp;
                    if(i%2 == 1)
                    {
                        temp = ori_list.at(i);
                        new_list<<temp;  //经过处理后的数据，只剩下有效数据
                    }
                }
                tempdata.val.tooldatVal.settooldatval(new_list);
                if(!VarIsStandable(name))
                    continue;
                if(func_JudgeDataExist(name) == 1)   //有重名数据
                    continue;
                codedata[typeindex][toolno] = tempdata;
            }
            else if(linestr.contains("DECL BASEDAT"))
            {
                QString svalue;
                int indexl,indexr,baseno;
                bool ok;
                indexl = linestr.indexOf("{");
                indexr = linestr.indexOf("}");
                if(indexl < 0 || indexr < 0)
                    continue;
                svalue = linestr.mid(indexl);
                int typeindex = TypeToIndex.value("BASEDAT");
                linestr = linestr.simplified();
                QStringList datalist = linestr.split(" ");
                for(int i = 0 ; i < datalist.count() ; i++)
                {
                    if(datalist[i] == "=")
                    {
                        fvar = i;
                        break;
                    }
                }
                if(fvar != 4)
                    continue;
                QString name = datalist[fvar-1];
                baseno = datalist[fvar-2].toInt(&ok);
                if(!ok)
                    continue;
                if(baseno < 0 || baseno >=32)
                    continue;
                CodeData tempdata;
                tempdata.name = name;
                tempdata.Svalue = svalue;
                tempdata.dtype = basedattype;
                tempdata.stotype = Variable;
                tempdata.module = "";
                tempdata.dimension = 1;

                svalue = svalue.remove("{");
                svalue = svalue.remove("}");
                svalue = svalue.remove(",");
                svalue = svalue.simplified();
                QStringList ori_list = svalue.split(" ");  //原始数据
                if(ori_list.count() != 12)
                    continue;
                QStringList new_list;
                for(int i=0;i<ori_list.count();i++)
                {
                    QString temp;
                    if(i%2 == 1)
                    {
                        temp = ori_list.at(i);
                        new_list<<temp;  //经过处理后的数据，只剩下有效数据
                    }
                }
                tempdata.val.basedatVal.setbasedatval(new_list);
                if(!VarIsStandable(name))
                    continue;
                if(func_JudgeDataExist(name) == 1)   //有重名数据
                    continue;
                codedata[typeindex][baseno] = tempdata;
            }
         }
        file.close();
    }
}
void func_getE6Pos(double* val)
{
    for(int i = 0 ;i < 6 ;i++)
    {
        val[i] = xSys.xV20.aAxis[8+i]/100000;
    }
    for(int i = 6 ; i < 12 ; i++)
        val[i] = 0;
    val[12] = 2;
    val[13] = 0;
}

CodePad::~CodePad()
{

}
