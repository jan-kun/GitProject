#include "tcp_in.h"

TCP_IN::TCP_IN(QWidget *parent) :
    QWidget(parent)
{
    frame_base = new QFrame(this);
    frame_base->setGeometry(0,0,494,694);
    frame_base->setStyleSheet("background-color:rgb(230,230,230)");
    frame_base->show();
    frame_base->raise();

    QLabel *lab_tip = new QLabel(frame_base);
    lab_tip->setGeometry(0,0,494,30);
    lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
    lab_tip->setText("  测量-工具-数字输入");
    lab_tip->show();

    validator = new QDoubleValidator(this);

    for(int i=0;i<7;i++)   //整个Base下方的功能按钮
    {
        btn_func[i]=new QPushButton(frame_base);
        btn_func[i]->setGeometry(2+70*i,647,68,42);
        btn_func[i]->setEnabled(false);
        btn_func[i]->setFocusPolicy(Qt::NoFocus);
        btn_func[i]->show();
        btn_func[i]->setStyleSheet("background-color:rgb(198,198,198);border:2px solid rgb(100,100,100);border-radius:3px");
        connect(btn_func[i],SIGNAL(clicked()),this,SLOT(slot_BtnClicked()));
    }
}

/*
** 函数功能：初始界面，选择工具号，输入工具名
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void TCP_IN::frame_SelectTool()
{
    if(frame_child[0] == NULL)
    {
        frame_child[0]=new QFrame(frame_base);
        frame_child[0]->setGeometry(0,30,494,617);
        frame_child[0]->setStyleSheet("background-color:rgb(230,230,230)");

        QLabel *lab_temp[9];
        for(int i=0;i<9;i++)
        {
            lab_temp[i]=new QLabel(frame_child[0]);
            lab_temp[i]->show();
        }
        lab_temp[0]->setText(tr("工具号"));
        lab_temp[0]->setGeometry(10,10,60,30);
        lab_temp[1]->setText(tr("工具名:"));
        lab_temp[1]->setGeometry(10,130,60,30);
        lab_temp[2]->setText(tr("选择需更改的工具"));
        lab_temp[2]->setGeometry(10,200,150,30);
        lab_temp[3]->setText(tr("X[mm]:"));
        lab_temp[3]->setGeometry(10,300,60,30);
        lab_temp[4]->setText(tr("Y[mm]:"));
        lab_temp[4]->setGeometry(10,360,60,30);
        lab_temp[5]->setText(tr("Z[mm]:"));
        lab_temp[5]->setGeometry(10,420,60,30);
        lab_temp[6]->setText(tr("A[°]:"));
        lab_temp[6]->setGeometry(200,300,60,30);
        lab_temp[7]->setText(tr("B[°]:"));
        lab_temp[7]->setGeometry(200,360,60,30);
        lab_temp[8]->setText(tr("C[°]:"));
        lab_temp[8]->setGeometry(200,420,60,30);

        for(int i = 0 ; i < 6 ; i++ )
        {
            lab_data1[i] = new QLabel(frame_child[0]);
            lab_data1[i]->show();
        }
        lab_data1[0]->setGeometry(90,300,60,30);
        lab_data1[1]->setGeometry(90,360,60,30);
        lab_data1[2]->setGeometry(90,420,60,30);
        lab_data1[3]->setGeometry(270,300,60,30);
        lab_data1[4]->setGeometry(270,360,60,30);
        lab_data1[5]->setGeometry(270,420,60,30);

        cb_tool=new QComboBox(frame_child[0]);
        cb_tool->setGeometry(150,10,200,35);
        cb_tool->show();
        cb_tool->setStyleSheet("QComboBox:editable{background: white;} QComboBox::drop-down{ width:60;height:50}");
        connect(cb_tool,SIGNAL(activated(int)),this,SLOT(slot_CbClicked(int)));

        line_name = new QLineEdit(frame_child[0]);
        line_name->setGeometry(150,130,300,35);
        line_name->show();
    }
    frame_child[0]->show();
    frame_child[0]->raise();

    set_Cb_Tool(cb_tool);
    if(KukaRob.indext < 0 )
        return;
    cb_tool->setCurrentIndex(KukaRob.indext);
    line_name->setText(codedata[14][KukaRob.indext].name);

    for( int i = 0 ; i < 6 ; i++)
        lab_data1[i]->setText( QString::number(codedata[14][KukaRob.indext].val.tooldatVal.toolval[i] ) );

    for(int i=0;i<7;i++)   //整个TCP下方的功能按钮
    {
        btn_func[i]->setEnabled(false);
        btn_func[i]->setText("");
    }
    btn_func[4]->setText(tr("返回"));
    btn_func[5]->setText(tr("继续"));
    btn_func[5]->setEnabled(true);

    currentPage = 1;
}

/*
** 函数功能：输入工具数据界面
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void TCP_IN::frame_ToolData()
{
    if(frame_child[1] == NULL)
    {
        frame_child[1]=new QFrame(frame_base);
        frame_child[1]->setGeometry(0,30,494,617);
        frame_child[1]->setStyleSheet("background-color:rgb(230,230,230)");

        QLabel *lab_temp[9];
        for(int i=0;i<9;i++)
        {
            lab_temp[i]=new QLabel(frame_child[1]);
            lab_temp[i]->show();
        }
        lab_temp[0]->setText(tr("工具号"));
        lab_temp[0]->setGeometry(10,10,60,30);
        lab_temp[1]->setText(tr("工具名："));
        lab_temp[1]->setGeometry(10,100,60,30);
        lab_temp[2]->setText(tr("输入工具数据"));
        lab_temp[2]->setGeometry(10,160,300,30);
        lab_temp[3]->setText(tr("X[mm]："));
        lab_temp[3]->setGeometry(10,300,60,30);
        lab_temp[4]->setText(tr("Y[mm]："));
        lab_temp[4]->setGeometry(10,360,60,30);
        lab_temp[5]->setText(tr("Z[mm]："));
        lab_temp[5]->setGeometry(10,420,60,30);
        lab_temp[6]->setText(tr("A[°]："));
        lab_temp[6]->setGeometry(200,300,60,30);
        lab_temp[7]->setText(tr("B[°]："));
        lab_temp[7]->setGeometry(200,360,60,30);
        lab_temp[8]->setText(tr("C[°]："));
        lab_temp[8]->setGeometry(200,420,60,30);

        for(int i=0;i<2;i++)
        {
            lab_data2[i]=new QLabel(frame_child[1]);
            lab_data2[i]->show();
        }
        /*显示工具号标签*/
        lab_data2[0]->setGeometry(100,10,60,30);
        /*显示工具名标签*/
        lab_data2[1]->setGeometry(100,100,60,30);

        for(int i=0;i<6;i++)
        {
            line_tool[i]=new QLineEdit(frame_child[1]);
            line_tool[i]->show();
            line_tool[i]->setValidator(validator);    //只能输入数字
        }
        line_tool[0]->setGeometry(90,300,70,30);
        line_tool[1]->setGeometry(90,360,70,30);
        line_tool[2]->setGeometry(90,420,70,30);
        line_tool[3]->setGeometry(270,300,70,30);
        line_tool[4]->setGeometry(270,360,70,30);
        line_tool[5]->setGeometry(270,420,70,30);
    }
    frame_child[1]->show();
    frame_child[1]->raise();

    lab_data2[0]->setText(QString::number(TcpNum));
    lab_data2[1]->setText(TcpName);

    for(int i = 0 ; i < 6 ; i++)
        line_tool[i]->setText( QString::number( codedata[14][TcpNum].val.tooldatVal.toolval[i] ) );

    for(int i=0;i<7;i++)   //整个TCP下方的功能按钮
    {
        btn_func[i]->setEnabled(false);
        btn_func[i]->setText("");
    }
    btn_func[4]->setText(tr("返回"));
    btn_func[4]->setEnabled(true);
    btn_func[5]->setText(tr("继续"));
    btn_func[5]->setEnabled(true);

    currentPage = 2;
}

/*
** 函数功能：输入刀具负载数据界面
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void TCP_IN::frame_Load()
{
    if(frame_child[2] == NULL)
    {
        frame_child[2]=new QFrame(frame_base);
        frame_child[2]->setGeometry(0,30,494,617);
        frame_child[2]->setStyleSheet("background-color:rgb(230,230,230)");
        frame_child[2]->show();
        frame_child[2]->raise();

        QLabel *lab_temp[13];
        for(int i=0;i<13;i++)
        {
            lab_temp[i]=new QLabel(frame_child[2]);
            lab_temp[i]->show();
        }
        lab_temp[0]->setText(tr("工具号"));
        lab_temp[0]->setGeometry(10,10,60,30);
        lab_temp[1]->setText(tr("工具名："));
        lab_temp[1]->setGeometry(10,60,60,30);
        lab_temp[2]->setText(tr("请输入刀具负载的数据\n(质量(M)、质量重心(X,Y,Z)和姿态(A,B,C)以及转动惯量(JX,JY,JZ))"));
        lab_temp[2]->setGeometry(10,120,490,50);
        lab_temp[3]->setText(tr("M[kg]:"));
        lab_temp[3]->setGeometry(10,200,60,30);
        lab_temp[4]->setText(tr("X[mm]:"));
        lab_temp[4]->setGeometry(10,250,60,30);
        lab_temp[5]->setText(tr("Y[mm]:"));
        lab_temp[5]->setGeometry(10,300,60,30);
        lab_temp[6]->setText(tr("Z[mm]:"));
        lab_temp[6]->setGeometry(10,350,60,30);
        lab_temp[7]->setText(tr("A[°]:"));
        lab_temp[7]->setGeometry(150,250,60,30);
        lab_temp[8]->setText(tr("B[°]:"));
        lab_temp[8]->setGeometry(150,300,60,30);
        lab_temp[9]->setText(tr("C[°]:"));
        lab_temp[9]->setGeometry(150,350,60,30);
        lab_temp[10]->setText(tr("JX[kg/m2]:"));
        lab_temp[10]->setGeometry(290,250,80,30);
        lab_temp[11]->setText(tr("JY[kg/m2]:"));
        lab_temp[11]->setGeometry(290,300,80,30);
        lab_temp[12]->setText(tr("JZ[kg/m2]:"));
        lab_temp[12]->setGeometry(290,350,80,30);

        for(int i=0;i<2;i++)
        {
            lab_data3[i]=new QLabel(frame_child[2]);
            lab_data3[i]->show();
        }
        /*显示工具号标签*/
        lab_data3[0]->setGeometry(100,10,60,30);
        /*显示工具名标签*/
        lab_data3[1]->setGeometry(100,60,60,30);
        for(int i=0;i<10;i++)
        {
            line_load[i] = new QLineEdit(frame_child[2]);
            line_load[i]->show();
            line_load[i]->setValidator(validator);
        }
        line_load[0]->setGeometry(80,200,60,30);
        line_load[1]->setGeometry(80,250,60,30);
        line_load[2]->setGeometry(80,300,60,30);
        line_load[3]->setGeometry(80,350,60,30);
        line_load[4]->setGeometry(210,250,60,30);
        line_load[5]->setGeometry(210,300,60,30);
        line_load[6]->setGeometry(210,350,60,30);
        line_load[7]->setGeometry(380,250,60,30);
        line_load[8]->setGeometry(380,300,60,30);
        line_load[9]->setGeometry(380,350,60,30);
    }
    frame_child[2]->show();
    frame_child[2]->raise();
    lab_data3[0]->setText(QString::number(TcpNum));
    lab_data3[1]->setText(TcpName);

    for(int i = 0 ; i < 10 ; i++)
        line_load[i]->setText(QString::number(codedata[14][TcpNum].val.tooldatVal.Load.loadval[i]) );

    for(int i=0;i<7;i++)   //整个TCP下方的功能按钮
    {
        btn_func[i]->setEnabled(false);
        btn_func[i]->setText("");
    }
    btn_func[3]->setText(tr("确认"));
    btn_func[3]->setEnabled(true);
    btn_func[4]->setText(tr("返回"));
    btn_func[4]->setEnabled(true);
    btn_func[5]->setText(tr("继续"));
    btn_func[5]->setEnabled(true);

    currentPage = 3;
}

void TCP_IN::frame_Res()
{
    if(frame_child[3] == NULL)
    {
        frame_child[3]=new QFrame(frame_base);
        frame_child[3]->setGeometry(0,30,494,617);
        frame_child[3]->setStyleSheet("background-color:rgb(230,230,230)");
        frame_child[3]->show();
        frame_child[3]->raise();

        QLabel *lab_temp[9];
        for(int i=0;i<9;i++)
        {
            lab_temp[i]=new QLabel(frame_child[3]);
            lab_temp[i]->show();
        }
        lab_temp[0]->setText(tr("工具号"));
        lab_temp[0]->setGeometry(10,10,60,30);
        lab_temp[1]->setText(tr("工具名:"));
        lab_temp[1]->setGeometry(10,130,60,30);
        lab_temp[2]->setText(tr("按下“保存”后，数据才被采用。"));
        lab_temp[2]->setGeometry(10,200,450,50);
        lab_temp[3]->setText(tr("X[mm]:"));
        lab_temp[3]->setGeometry(10,300,60,30);
        lab_temp[4]->setText(tr("Y[mm]:"));
        lab_temp[4]->setGeometry(10,360,60,30);
        lab_temp[5]->setText(tr("Z[mm]:"));
        lab_temp[5]->setGeometry(10,420,60,30);
        lab_temp[6]->setText("A[°]");
        lab_temp[6]->setGeometry(200,300,60,30);
        lab_temp[7]->setText("B[°]");
        lab_temp[7]->setGeometry(200,360,60,30);
        lab_temp[8]->setText("C[°]");
        lab_temp[8]->setGeometry(200,420,60,30);

        for(int i=0;i<2;i++)
        {
            lab_tool[i]=new QLabel(frame_child[3]);
            lab_tool[i]->show();
        }
        /*显示工具号标签*/
        lab_tool[0]->setGeometry(120,10,60,30);
        /*显示工具名标签*/
        lab_tool[1]->setGeometry(120,130,60,30);
        for(int i=0;i<6;i++)
        {
            lab_data4[i]=new QLabel(frame_child[3]);
            lab_data4[i]->show();
        }
        lab_data4[0]->setGeometry(90,300,60,30);
        lab_data4[1]->setGeometry(90,360,60,30);
        lab_data4[2]->setGeometry(90,420,60,30);
        lab_data4[3]->setGeometry(280,300,60,30);
        lab_data4[4]->setGeometry(280,360,60,30);
        lab_data4[5]->setGeometry(280,420,60,30);
    }
    frame_child[3]->show();
    frame_child[3]->raise();
    lab_tool[0]->setText(QString::number(TcpNum));
    lab_tool[1]->setText(TcpName);

    for(int i = 0 ; i < 6 ; i++)
        lab_data4[i]->setText(line_tool[i]->text());

    for(int i=0;i<7;i++) //整个TCP下方的功能按钮
    {
        btn_func[i]->setEnabled(false);
        btn_func[i]->setText("");
    }
    btn_func[4]->setText(tr("返回"));
    btn_func[4]->setEnabled(true);
    btn_func[5]->setText(tr("保存"));
    btn_func[5]->setEnabled(true);

    currentPage = 4;
}

void TCP_IN::func_back()
{
    if(currentPage == 4)   //当前正在第四页
    {
        frame_Load();
    }
    else if(currentPage== 3)   //当前正在第三页
    {
        frame_ToolData();
    }
    else if(currentPage == 2)   //当前正在第二页
    {
        frame_SelectTool();
    }
    else if(currentPage == 1)   //当前正在第一页
    {
        return;
    }
}

void TCP_IN::func_continue()
{
    if(currentPage == 1)   //当前正在第一页
    {
        TcpNum = cb_tool->currentIndex();
        TcpName = line_name->text();  //保存工具名称
        if(!VarIsStandable(TcpName))
        {
            QMessageBox::warning(this,"创建工具坐标","工具名称非法！请检查您的工具名称","确定");
            return;
        }
        if(func_JudgeDataExist(TcpName) == 1)
        {
            QMessageBox::warning(this,"创建工具坐标","工具名称已存在！请重新输入","确定");
            return;
        }
        frame_ToolData();
    }
    else if(currentPage == 2)   //当前正在第二页
    {
        bool ok;
        for(int i = 0 ; i < 6 ; i++)
        {
            tool_data[i] = line_tool[i]->text().toDouble(&ok);   //保存输入的工具坐标信息
            if( !ok )
            {
                QMessageBox::warning(this,"创建工具坐标","输入数据不合法","确定");
                return;
            }
        }
        frame_Load();
    }
    else if(currentPage == 3)   //当前正在第三页
    {
        bool ok;
        for(int i = 0 ; i < 10 ; i++)
        {
            load_data[i] = line_load[i]->text().toDouble(&ok);   //保存输入的工具负载信息
            if( !ok )
            {
                QMessageBox::warning(this,"创建工具坐标","输入数据不合法","确定");
                return;
            }
        }
        frame_Res();
    }
    else if(currentPage == 4)   //当前正在第四页,"继续"按钮变成"保存"按钮
    {
        return;
    }
}

void TCP_IN::func_save()
{
    QStringList valuelist;
    valuelist.clear();
    for(int i = 0 ; i< 6 ; i++)
    {
        QString value = lab_data4[i]->text();
        valuelist<<value;
    }
    valuelist<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0"<<"0";
    QString svalue;
    svalue.clear();
    QString member[16]={"{ X "," , Y "," , Z "," , A "," , B "," , C "," , Mass "," , X "," , Y "," , Z "," , A "," , B "," , C "," , JX "," , JY "," , JZ "};
    for(int i = 0 ; i < 16 ;i++)
    {
        svalue += member[i] + valuelist.at(i);
    }
    svalue += " }";
    codedata[14][TcpNum].name = TcpName;
    codedata[14][TcpNum].val.tooldatVal.settooldatval(valuelist);
    codedata[14][TcpNum].Svalue = svalue;
    func_CreatToolDataDecl(codedata[14][TcpNum]);
}

void TCP_IN::func_CreatToolDataDecl(CodeData data)
{
    QTextCursor temptc;
    QPlainTextEdit *tempedit;
    QString datpath;
    QString name,value,insertstr;

    datpath = RobSysDataFile;
    name = data.name;
    value = data.Svalue;
    insertstr = "DECL TOOLDAT "+QString::number(TcpNum)+" "+name+" = "+value;

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
    int res = func_CursorGoStr(tempedit,"DECL TOOLDAT "+QString::number(TcpNum));
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

void TCP_IN::slot_BtnClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(!btn)
        return;
    if(btn == btn_func[4])
    {
        if(btn->text() == tr("返回") )
            func_back();
    }
    else if(btn == btn_func[5])
    {
        if(btn->text() == tr("继续") )
            func_continue();
        else if(btn->text() == tr("保存") )
            func_save();
    }
}

void TCP_IN::slot_CbClicked(int index)
{
    TcpNum = index;
    line_name->setText(codedata[14][TcpNum].name);
    for( int i = 0 ; i < 6 ; i++)
        lab_data1[i]->setText( QString::number(codedata[14][index].val.tooldatVal.toolval[i] ) );
}

