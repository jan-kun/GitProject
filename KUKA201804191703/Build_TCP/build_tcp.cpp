#include "build_tcp.h"

Build_TCP::Build_TCP(QWidget *parent)
    : QWidget(parent)
{
    frame_base = new QFrame(this);
    frame_base->setGeometry(0,0,494,694);
    frame_base->setObjectName("fbt");
    frame_base->setStyleSheet("#fbt{background-color:rgb(230,230,230)}");
    frame_base->show();
    frame_base->raise();

    QLabel *lab_tip = new QLabel(frame_base);
    lab_tip->setGeometry(0,0,494,30);
    lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
    lab_tip->setText("  测量-工具-XYZ 4 点法");
    lab_tip->show();

    validator = new QDoubleValidator(this);

    for(int i=0;i<7;i++)   //整个TCP下方的功能按钮
    {
        btn_tcp[i]=new QPushButton(frame_base);
        btn_tcp[i]->setGeometry(2+70*i,647,68,42);
        btn_tcp[i]->setEnabled(false);
        btn_tcp[i]->setFocusPolicy(Qt::NoFocus);
        btn_tcp[i]->show();
        btn_tcp[i]->setStyleSheet("background-color:rgb(198,198,198);border:2px solid rgb(100,100,100);border-radius:3px");
        connect(btn_tcp[i],SIGNAL(clicked()),this,SLOT(slot_BtnClicked()));
    }
}

/*
** 函数功能：显示选择待测量工具坐标系页面
** 函数参数：无
** 函数返回值：无
*/
void Build_TCP::frame_SelectTool()
{
    if(frame_child[0] == NULL)
    {
        frame_child[0]=new QFrame(frame_base);
        frame_child[0]->setGeometry(0,30,494,617);
        frame_child[0]->setStyleSheet("background-color:rgb(230,230,230)");

        for(int i=0;i<15;i++)   //第一页里15个标签的初始化
        {
            lab_tcp1[i]=new QLabel(frame_child[0]);
            lab_tcp1[i]->show();
        }
        lab_tcp1[0]->setText(tr("工具号"));
        lab_tcp1[0]->setGeometry(10,10,60,30);
        lab_tcp1[1]->setText(tr("工具名:"));
        lab_tcp1[1]->setGeometry(10,130,60,30);
        lab_tcp1[2]->setText(tr("选定待测量的工具"));
        lab_tcp1[2]->setGeometry(10,200,150,30);

        lab_tcp1[3]->setText(tr("X[mm]:"));
        lab_tcp1[3]->setGeometry(10,300,60,30);
        lab_tcp1[4]->setText(tr("Y[mm]:"));
        lab_tcp1[4]->setGeometry(10,360,60,30);
        lab_tcp1[5]->setText(tr("Z[mm]:"));
        lab_tcp1[5]->setGeometry(10,420,60,30);

        /*显示XYZ数值标签*/
        lab_tcp1[6]->setGeometry(90,300,80,30);
        lab_tcp1[7]->setGeometry(90,360,80,30);
        lab_tcp1[8]->setGeometry(90,420,80,30);

        lab_tcp1[9]->setText(tr("A[°]:"));
        lab_tcp1[9]->setGeometry(200,300,60,30);
        lab_tcp1[10]->setText(tr("B[°]:"));
        lab_tcp1[10]->setGeometry(200,360,60,30);
        lab_tcp1[11]->setText(tr("C[°]:"));
        lab_tcp1[11]->setGeometry(200,420,60,30);

        /*显示ABC数值标签*/
        lab_tcp1[12]->setGeometry(300,300,80,30);
        lab_tcp1[13]->setGeometry(300,360,80,30);
        lab_tcp1[14]->setGeometry(300,420,80,30);

        cb_tool_no=new QComboBox(frame_child[0]);
        cb_tool_no->setGeometry(150,10,200,35);
        cb_tool_no->show();
        cb_tool_no->setStyleSheet("QComboBox:editable{background: white;} QComboBox::drop-down{ width:60;height:50}");
        connect(cb_tool_no,SIGNAL(activated(int)),this,SLOT(slot_CbClicked(int)));

        line_name = new QLineEdit(frame_child[0]);
        line_name->setGeometry(150,130,300,35);
        line_name->show();
    }
    frame_child[0]->show();
    frame_child[0]->raise();
    set_Cb_Tool(cb_tool_no);
    if(KukaRob.indext < 0)
        return;
    cb_tool_no->setCurrentIndex(KukaRob.indext);
    line_name->setText(codedata[14][KukaRob.indext].name);

    for(int i =0 ; i < 3 ; i++)
        lab_tcp1[i+6]->setText(QString::number(codedata[14][KukaRob.indext].val.tooldatVal.toolval[i]));
    for(int i = 0 ; i < 3 ; i++)
        lab_tcp1[i+12]->setText(QString::number(codedata[14][KukaRob.indext].val.tooldatVal.toolval[i+3]));

    for(int i=0;i<7;i++)   //整个TCP下方的功能按钮
    {
        btn_tcp[i]->setEnabled(false);
        btn_tcp[i]->setText("");
    }
    btn_tcp[4]->setText(tr("返回"));
    btn_tcp[5]->setText(tr("继续"));
    btn_tcp[5]->setEnabled(true);
    count = 0;  //count用于显示当前测量到第第几个点，初始化为0,每测量一个点+1,返回则-1
    currentPage = 1;
}

/*
** 函数功能：显示校准四个方向界面
** 函数参数：无
** 函数返回值：无
*/
void Build_TCP::frame_Orient()
{
    if(frame_child[1] == NULL)
    {
        frame_child[1]=new QFrame(frame_base);
        frame_child[1]->setGeometry(0,30,494,617);
        frame_child[1]->setStyleSheet("background-color:rgb(230,230,230)");
        frame_child[1]->show();
        frame_child[1]->raise();

        for(int i=0;i<6;i++)
        {
            lab_tcp2[i]=new QLabel(frame_child[1]);
            lab_tcp2[i]->show();
        }

        lab_tcp2[0]->setText(tr("工具号"));
        lab_tcp2[0]->setGeometry(10,10,60,30);

        /*显示工具坐标下标标签*/
        lab_tcp2[1]->setGeometry(100,10,60,30);

        lab_tcp2[2]->setText(tr("工具名："));
        lab_tcp2[2]->setGeometry(10,100,60,30);

        /*显示工具坐标名称标签*/
        lab_tcp2[3]->setGeometry(100,100,60,30);

        lab_tcp2[4]->setText(tr("将不同方向的工具向一个参照点校准"));
        lab_tcp2[4]->setGeometry(10,160,300,30);

        lab_tcp2[5]->setText(tr("（方向1）"));
        lab_tcp2[5]->setGeometry(10,190,300,30);

    }
    frame_child[1]->show();
    frame_child[1]->raise();
    lab_tcp2[1]->setText(QString::number(TCPNum));
    lab_tcp2[3]->setText(TCPName);
    for(int i=0;i<7;i++)   //整个TCP下方的功能按钮
    {
        btn_tcp[i]->setEnabled(false);
        btn_tcp[i]->setText("");
    }
    btn_tcp[0]->setText(tr("测量"));
    btn_tcp[0]->setEnabled(true);
    btn_tcp[4]->setText(tr("返回"));
    btn_tcp[4]->setEnabled(true);
    btn_tcp[5]->setText(tr("继续"));
    currentPage = 2;
}

/*
** 函数功能：显示输入工具坐标系下负载数据的界面
** 函数参数：无
** 函数返回值：无
*/
void Build_TCP::frame_Load()
{
    if(frame_child[2] == NULL)
    {
        frame_child[2]=new QFrame(frame_base);
        frame_child[2]->setGeometry(0,30,494,617);
        frame_child[2]->setStyleSheet("background-color:rgb(230,230,230)");

        for(int i=0;i<18;i++)
        {
            lab_tcp3[i]=new QLabel(frame_child[2]);
            lab_tcp3[i]->show();
        }
        lab_tcp3[0]->setText(tr("工具号"));
        lab_tcp3[0]->setGeometry(10,10,60,30);

        /*显示工具坐标下标标签*/
        lab_tcp3[1]->setGeometry(100,10,60,30);

        lab_tcp3[2]->setText(tr("工具名："));
        lab_tcp3[2]->setGeometry(10,50,60,30);

         /*显示工具坐标名称标签*/
        lab_tcp3[3]->setGeometry(100,50,60,30);

        lab_tcp3[4]->setText(tr("请输入刀具负载的数据\n(质量(M)、质量重心(X,Y,Z)和姿态(A,B,C)以及转动惯量(JX,JY,JZ))"));
        lab_tcp3[4]->setGeometry(10,135,484,50);

        lab_tcp3[5]->setText(tr("M[kg]:"));
        lab_tcp3[5]->setGeometry(10,220,60,30);

        lab_tcp3[6]->setText(tr("X[mm]:"));
        lab_tcp3[6]->setGeometry(10,255,60,30);
        lab_tcp3[7]->setText(tr("Y[mm]:"));
        lab_tcp3[7]->setGeometry(10,290,60,30);
        lab_tcp3[8]->setText(tr("Z[mm]:"));
        lab_tcp3[8]->setGeometry(10,325,60,30);

        lab_tcp3[9]->setText(tr("A[°]:"));
        lab_tcp3[9]->setGeometry(150,255,60,30);
        lab_tcp3[10]->setText(tr("B[°]:"));
        lab_tcp3[10]->setGeometry(150,290,60,30);
        lab_tcp3[11]->setText(tr("C[°]:"));
        lab_tcp3[11]->setGeometry(150,325,60,30);

        lab_tcp3[12]->setText(tr("JX[kg/m2]:"));
        lab_tcp3[12]->setGeometry(290,255,80,30);
        lab_tcp3[13]->setText(tr("JY[kg/m2]:"));
        lab_tcp3[13]->setGeometry(290,290,80,30);
        lab_tcp3[14]->setText(tr("JZ[kg/m2]:"));
        lab_tcp3[14]->setGeometry(290,325,80,30);

        lab_tcp3[15]->setText(tr("在线负载数据检查的配置"));
        lab_tcp3[15]->setGeometry(10,370,200,30);

        lab_tcp3[16]->setText(tr("过载时的反应"));
        lab_tcp3[16]->setGeometry(70,460,90,30);

        lab_tcp3[17]->setText(tr("欠载时的反应"));
        lab_tcp3[17]->setGeometry(70,510,90,30);

        for(int i=0;i<10;i++)
        {
            line_weight[i] = new QLineEdit(frame_child[2]);
            line_weight[i]->show();
            line_weight[i]->setValidator(validator);    //行编辑器内只能输入数值字符
        }

        line_weight[0]->setGeometry(70,220,60,30);
        line_weight[1]->setGeometry(70,255,60,30);
        line_weight[2]->setGeometry(70,290,60,30);
        line_weight[3]->setGeometry(70,325,60,30);

        line_weight[4]->setGeometry(210,255,60,30);
        line_weight[5]->setGeometry(210,290,60,30);
        line_weight[6]->setGeometry(210,325,60,30);

        line_weight[7]->setGeometry(380,255,60,30);
        line_weight[8]->setGeometry(380,290,60,30);
        line_weight[9]->setGeometry(380,325,60,30);

        QCheckBox *check_fz = new QCheckBox(tr("负载数据检查"),frame_child[2]);
        check_fz->setGeometry(20,410,120,30);
        check_fz->show();

        QComboBox *cb_reaction[2];
        cb_reaction[0] = new QComboBox(frame_child[2]);
        cb_reaction[0]->setGeometry(170,460,150,30);
        cb_reaction[0]->insertItem(0,tr("停止机器人"));
        cb_reaction[0]->show();

        cb_reaction[1] = new QComboBox(frame_child[2]);
        cb_reaction[1]->setGeometry(170,510,150,30);
        cb_reaction[1]->insertItem(0,tr("警告"));
        cb_reaction[1]->show();
    }
    frame_child[2]->show();
    frame_child[2]->raise();
    lab_tcp3[1]->setText(QString::number(TCPNum));
    lab_tcp3[3]->setText(TCPName);

    for(int i = 0 ; i < 10 ; i++)
        line_weight[i]->setText( QString::number(codedata[14][TCPNum].val.tooldatVal.Load.loadval[i] ) );

    for(int i=0;i<7;i++)   //整个TCP下方的功能按钮
    {
        btn_tcp[i]->setEnabled(false);
        btn_tcp[i]->setText("");
    }
    btn_tcp[3]->setText(tr("确认"));
    btn_tcp[3]->setEnabled(true);
    btn_tcp[4]->setText(tr("返回"));
    btn_tcp[4]->setEnabled(true);
    btn_tcp[5]->setText(tr("继续"));
    btn_tcp[5]->setEnabled(true);

    currentPage = 3;
}

/*
** 函数功能：显示建立好的工具坐标数据界面
** 函数参数：无
** 函数返回值：无
*/
void Build_TCP::frame_Res()
{
    if(frame_child[3] == NULL)
    {
        frame_child[3]=new QFrame(frame_base);
        frame_child[3]->setGeometry(0,30,494,617);
        frame_child[3]->setStyleSheet("background-color:rgb(230,230,230)");
        frame_child[3]->show();
        frame_child[3]->raise();

        for(int i=0;i<13;i++)
        {
            lab_tcp4[i]=new QLabel(frame_child[3]);
            lab_tcp4[i]->show();
        }

        lab_tcp4[0]->setText(tr("工具号"));
        lab_tcp4[0]->setGeometry(10,10,60,30);
        lab_tcp4[1]->setText(tr("工具名:"));
        lab_tcp4[1]->setGeometry(10,130,60,30);
        lab_tcp4[2]->setText(tr("请点击“保存”来应用数据。\n选择ABC2点，ABC世界坐标法时，数据被自动应用"));
        lab_tcp4[2]->setGeometry(10,200,450,50);
        lab_tcp4[3]->setText(tr("X[mm]:"));
        lab_tcp4[3]->setGeometry(10,300,60,30);
        lab_tcp4[4]->setText(tr("Y[mm]:"));
        lab_tcp4[4]->setGeometry(10,360,60,30);
        lab_tcp4[5]->setText(tr("Z[mm]:"));
        lab_tcp4[5]->setGeometry(10,420,60,30);

        /*显示工具坐标下标标签*/
        lab_tcp4[6]->setGeometry(120,10,60,30);
        /*显示工具坐标名称标签*/
        lab_tcp4[7]->setGeometry(120,130,60,30);

        /*显示工具坐标XYZ数据标签*/
        lab_tcp4[8]->setGeometry(90,300,60,30);
        lab_tcp4[9]->setGeometry(90,360,60,30);
        lab_tcp4[10]->setGeometry(90,420,60,30);

        lab_tcp4[11]->setText(tr("故障"));
        lab_tcp4[11]->setGeometry(230,300,50,30);
        /*显示工具坐标偏差标签*/
        lab_tcp4[12]->setGeometry(280,300,50,30);
    }
    frame_child[3]->show();
    frame_child[3]->raise();

    lab_tcp4[6]->setText(QString::number(TCPNum));
    lab_tcp4[7]->setText(TCPName);

    /*显示工具坐标XYZ数据标签*/
    lab_tcp4[8]->setText(QString::number(codedata[14][TCPNum].val.tooldatVal.toolval[0]));
    lab_tcp4[9]->setText(QString::number(codedata[14][TCPNum].val.tooldatVal.toolval[1]));
    lab_tcp4[10]->setText(QString::number(codedata[14][TCPNum].val.tooldatVal.toolval[2]));

    for(int i=0;i<7;i++)   //整个TCP下方的功能按钮
    {
        btn_tcp[i]->setEnabled(false);
        btn_tcp[i]->setText("");
    }
    btn_tcp[0]->setText(tr("ABC2点"));
    btn_tcp[0]->setEnabled(true);
    btn_tcp[1]->setText(tr("ABC世界")+tr("\n")+tr(" 坐标法"));
    btn_tcp[1]->setEnabled(true);
    btn_tcp[3]->setText(tr("测量点"));
    btn_tcp[3]->setEnabled(true);
    btn_tcp[4]->setText(tr("返回"));
    btn_tcp[4]->setEnabled(true);
    btn_tcp[5]->setText(tr("保存"));
    btn_tcp[5]->setEnabled(true);

    currentPage = 4;
}

void Build_TCP::slot_BtnClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if( !btn )
        return;
    if(btn == btn_tcp[0])
    {
        if(btn->text() == tr("测量"))
            func_measure();
    }
    else if(btn == btn_tcp[3] )
    {
        if(btn->text() == tr("确认"))
        {
            bool ok;
            for(int i = 0 ; i < 10 ; i++)
                toolload[i] = line_weight[i]->text().toDouble(&ok);
        }
    }
    else if(btn == btn_tcp[4] )
    {
        if(btn->text() == tr("返回") )
            func_back();
    }
    else if(btn == btn_tcp[5] )
    {
        if(btn->text() == tr("继续"))
            func_contiue();
        else if(btn->text() == tr("保存"))
            func_save();
    }
}

void Build_TCP::slot_CbClicked(int index)
{
    QComboBox* cb = qobject_cast<QComboBox*>(sender());
    if( !cb )
        return;
    if( cb == cb_tool_no )
    {
        TCPNum = index;
        line_name->setText(codedata[14][TCPNum].name);
        for(int i = 0 ; i < 3 ; i++)
            lab_tcp1[i+6]->setText( QString::number(codedata[14][index].val.tooldatVal.toolval[i]) );
        for(int i = 0 ; i < 3 ; i++)
            lab_tcp1[i+12]->setText( QString::number(codedata[14][index].val.tooldatVal.toolval[i+3]) );
    }
}

void Build_TCP::func_contiue()
{
    if(currentPage == 1)   //当前正在第一页
    {
        TCPNum = cb_tool_no->currentIndex();
        TCPName = line_name->text();  //保存工具名称
        if(!VarIsStandable(TCPName))
        {
            QMessageBox::warning(this,"创建工具坐标","工具名称非法！请检查您的工具名称","确定");
            return;
        }
        if(func_JudgeDataExist(TCPName) == 1)
        {
            QMessageBox::warning(this,"创建工具坐标","工具名称已存在！请重新输入","确定");
            return;
        }
        frame_Orient();
    }
    if(currentPage == 2)   //当前正在第二页
    {
        return;
    }
    if(currentPage == 3)   //当前正在第三页
    {
        bool ok;
        for(int i = 0 ; i < 10 ; i++)
            toolload[i] = line_weight[i]->text().toDouble(&ok);
        func_CalcToolData();
    }
    if(currentPage == 4)   //当前正在第四页,"继续"按钮变成"保存"按钮
    {
        return;
    }
}

void Build_TCP::func_measure()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("hh:mm:ss yyyy/MM/dd"); //设置显示格式

    QMessageBox *TipBox = new QMessageBox(this);
    TipBox->setFixedSize(400,250);
    TipBox->setIconPixmap(QPixmap(":/image/image/icon_question.png"));
    TipBox->setWindowTitle(str);
    TipBox->setWindowIcon(QIcon(":/image/image/robot.png"));
    TipBox->setText(tr("要采用当前位置吗？\n   继续进行测量。"));
    TipBox->addButton(tr("是"),QMessageBox::AcceptRole);
    TipBox->addButton(tr("否"),QMessageBox::RejectRole);
    int res = TipBox->exec();
    if(res == 0)   //是
    {
        if(func_RealMeasure() == 1)   //调用真正测量函数来实现4点法建立TCP
        {
            count++;
            if(count == 1)
                lab_tcp2[5]->setText(tr("（方向2）"));
            if(count == 2)
                lab_tcp2[5]->setText(tr("（方向3）"));
            if(count == 3)
                lab_tcp2[5]->setText(tr("（方向4）"));
            if(count == 4)
            {
                frame_Load();
                count--;
            }
        }
        else
            QMessageBox::warning(this,"修改位置","修改位置失败!","确定");
    }
}

/*
** 函数功能：测量点实现4点法确定TCP坐标
** 函数参数：无
** 函数返回值：测量计算成功返回1，失败返回-1
*/
int Build_TCP::func_RealMeasure()
{
    if(count < 0 || count >=4 )
        return -1;
    memcpy(&point[count] , &(xSys.xV20.aAxis[0]) , sizeof(TPoint));
    return 1;
}

void Build_TCP::func_CalcToolData()
{
    memset(res , 0 , 56);
    res[3] = 1;
    int ppo[11][6];
    memcpy(ppo , point , 264);
    int way = 4;
    if(GetTooldata(ppo,(int*)xSys.xArm.aAxis,way,res,cor) == 0)
    {
        memcpy(codedata[14][TCPNum].val.tooldatVal.ctoolval, res , 56);
        memcpy(codedata[14][TCPNum].val.tooldatVal.err , res , 24);
        frame_Res();
    }
    else
    {
        QMessageBox::warning(this,"警告","无法通过当前位置校准。","确定");
        return;
    }
}
void Build_TCP::func_back()
{
    if(currentPage == 4)   //当前正在第四页
    {
        frame_Load();
    }
    else if(currentPage== 3)   //当前正在第三页
    {
        frame_Orient();
    }
    else if(currentPage == 2)   //当前正在第二页
    {
        count--;
        if(count == 2)
            lab_tcp2[5]->setText(tr("（方向3）"));
        if(count == 1)
            lab_tcp2[5]->setText(tr("（方向2）"));
        if(count == 0)
            lab_tcp2[5]->setText(tr("（方向1）"));
        if(count == -1)
        {
            count=0;
            frame_SelectTool();
        }
    }
    else if(currentPage == 1)   //当前正在第一页
    {
        return;
    }
}

/*
** 函数功能：保存计算好的TCP坐标数据
** 函数参数：无
** 函数返回值：无
*/
void Build_TCP::func_save()
{
    valuelist.clear();
    for(int i = 0 ; i< 6 ; i++)
    {
        QString value = QString::number(toolinfo[i]);
        valuelist<<value;
    }
    for(int i = 0 ; i< 10 ; i++)
    {
        QString value = QString::number(toolload[i]);
        valuelist<<value;
    }
    QString svalue;
    svalue.clear();
    QString member[16]={"X "," , Y "," , Z "," , A "," , B "," , C "," , Mass "," , X "," , Y "," , Z "," , A "," , B "," , C "," , JX "," , JY "," , JZ "};
    for(int i = 0 ; i < 16 ;i++)
    {
        svalue += member[i] + valuelist.at(i);
    }

    codedata[14][TCPNum].name = TCPName;
    codedata[14][TCPNum].val.tooldatVal.settooldatval(valuelist);
    codedata[14][TCPNum].Svalue = svalue;
}

void Build_TCP::func_BuildToolData()
{

}

void Build_TCP::func_CreatToolDataDecl(CodeData data)
{
    QTextCursor temptc;
    QPlainTextEdit *tempedit;
    QString datpath;
    QString name,value,insertstr;

    datpath = RobSysDataFile;
    name = data.name;
    value = data.Svalue;
    insertstr = "DECL TOOLDAT "+QString::number(TCPNum)+" "+name+" = "+value;

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
    int res = func_CursorGoStr(tempedit,"DECL TOOLDAT "+QString::number(TCPNum));
    if( res == -1)
    {
        temptc = tempedit->textCursor();
        temptc.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
        tempedit->setTextCursor(temptc);
        tempedit->insertPlainText("\r"+insertstr);
        func_SaveToFile(tempedit,datpath);
    }
    else if(res == 0)
    {
        temptc = tempedit->textCursor();
        temptc.movePosition(QTextCursor::EndOfBlock,QTextCursor::KeepAnchor);
        temptc.insertText(insertstr);
        func_SaveToFile(tempedit,datpath);
    }
}

void set_Cb_Tool(QComboBox* cb)
{
    cb->clear();
    int toolindex = TypeToIndex.value("TOOLDAT");
    for(int i=0;i<16;i++)
    {
        cb->insertItem(i,codedata[toolindex][i].name+"      ["+QString::number(i)+"]");
    }
}

Build_TCP::~Build_TCP()
{

}
