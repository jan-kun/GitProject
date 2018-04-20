#include "build_base.h"

Build_Base::Build_Base(QWidget *parent)
    : QWidget(parent)
{
    frame_base = new QFrame(this);
    frame_base->setGeometry(0,0,494,694);
    frame_base->setStyleSheet("background-color:rgb(230,230,230)");
    frame_base->show();
    frame_base->raise();

    QLabel *lab_tip = new QLabel(frame_base);
    lab_tip->setGeometry(0,0,494,30);
    lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
    lab_tip->setText("  测量-基座标-3 点");

    for(int i=0;i<7;i++)   //整个TCP下方的功能按钮
    {
        btn_base[i]=new QPushButton(frame_base);
        btn_base[i]->setGeometry(2+70*i,647,68,42);
        btn_base[i]->setEnabled(false);
        btn_base[i]->setFocusPolicy(Qt::NoFocus);
        btn_base[i]->show();
        btn_base[i]->setStyleSheet("background-color:rgb(198,198,198);border:2px solid rgb(100,100,100);border-radius:3px");
        connect(btn_base[i],SIGNAL(clicked()),this,SLOT(slot_BtnClicked()));
    }
}

/*
** 函数功能：显示选择待测量基坐标系页面
** 函数参数：无
** 函数返回值：无
*/
void Build_Base::frame_SelectBase()
{
    if(frame_child[0] == NULL)
    {
        frame_child[0]=new QFrame(frame_base);
        frame_child[0]->setGeometry(0,30,494,617);
        frame_child[0]->setStyleSheet("background-color:rgb(230,230,230)");
        frame_child[0]->show();
        frame_child[0]->raise();

        QLabel *lab_temp[9];
        for(int i=0;i<9;i++)   //第一页里15个标签的初始化
        {
            lab_temp[i]=new QLabel(frame_child[0]);
            lab_temp[i]->show();
        }
        lab_temp[0]->setText(tr("基坐标系统号"));
        lab_temp[0]->setGeometry(10,10,100,30);
        lab_temp[1]->setText(tr("基坐标系名称:"));
        lab_temp[1]->setGeometry(10,130,100,30);
        lab_temp[2]->setText(tr("选择待测量的基坐标系统"));
        lab_temp[2]->setGeometry(10,200,150,30);
        lab_temp[3]->setText(tr("X[mm]:"));
        lab_temp[3]->setGeometry(10,300,60,30);
        lab_temp[4]->setText(tr("Y[mm]:"));
        lab_temp[4]->setGeometry(10,360,60,30);
        lab_temp[5]->setText(tr("Z[mm]:"));
        lab_temp[5]->setGeometry(10,420,60,30);
        lab_temp[6]->setText(tr("A[°]:"));
        lab_temp[6]->setGeometry(230,300,60,30);
        lab_temp[7]->setText(tr("B[°]:"));
        lab_temp[7]->setGeometry(230,360,60,30);
        lab_temp[8]->setText(tr("C[°]:"));
        lab_temp[8]->setGeometry(230,420,60,30);

        for(int i=0;i<6;i++)   //第一页里6个标签的初始化
        {
            lab_base1[i]=new QLabel(frame_child[0]);
            lab_base1[i]->show();
        }
        lab_base1[0]->setGeometry(90,300,120,30);
        lab_base1[1]->setGeometry(90,360,120,30);
        lab_base1[2]->setGeometry(90,420,120,30);
        lab_base1[3]->setGeometry(310,300,120,30);
        lab_base1[4]->setGeometry(310,360,120,30);
        lab_base1[5]->setGeometry(310,420,120,30);

        cb_base=new QComboBox(frame_child[0]);
        cb_base->setGeometry(150,10,200,35);
        cb_base->show();
        cb_base->setStyleSheet("QComboBox:editable{background: white;} QComboBox::drop-down{ width:60;height:50}");
        connect(cb_base,SIGNAL(activated(int)),this,SLOT(slot_CbClicked(int)));

        line_name = new QLineEdit(frame_child[0]);
        line_name->setGeometry(150,130,300,35);
        line_name->show();
    }
    frame_child[0]->show();
    frame_child[0]->raise();
    set_Cb_Base(cb_base);
    if(KukaRob.indexb < 0)
        return;
    cb_base->setCurrentIndex(KukaRob.indexb);
    line_name->setText(codedata[12][KukaRob.indexb].name);

    for( int i = 0 ; i < 6 ; i++ )
        lab_base1[i]->setText(QString::number(codedata[12][KukaRob.indexb].val.basedatVal.baseval[i]));

    for(int i=0;i<7;i++)   //整个Base下方的功能按钮
    {
        btn_base[i]->setText("");
        btn_base[i]->setEnabled(false);
    }
    btn_base[4]->setText(tr("返回"));
    btn_base[5]->setText(tr("继续"));
    btn_base[5]->setEnabled(true);
    count = 0;
    currentpage = 1;
}

/*
** 函数功能：显示所使用的参考工具页面
** 函数参数：无
** 函数返回值：无
*/
void Build_Base::frame_SelectTool()
{
    if(frame_child[1] == NULL)
    {
        frame_child[1]=new QFrame(frame_base);
        frame_child[1]->setGeometry(0,30,494,617);
        frame_child[1]->setStyleSheet("background-color:rgb(230,230,230)");
        frame_child[1]->show();
        frame_child[1]->raise();

        QLabel *lab_temp[9];
        for( int i = 0 ; i < 9 ; i++ )   //第二页里16个标签的初始化
        {
            lab_temp[i]=new QLabel(frame_child[1]);
            lab_temp[i]->show();
        }
        lab_temp[0]->setText(tr("参考工具编号"));
        lab_temp[0]->setGeometry(10,10,100,30);
        lab_temp[1]->setText(tr("工具名:"));
        lab_temp[1]->setGeometry(10,130,80,30);
        lab_temp[2]->setText(tr("请选择所使用的参考工具"));
        lab_temp[2]->setGeometry(10,200,200,30);
        lab_temp[3]->setText(tr("X[mm]:"));
        lab_temp[3]->setGeometry(10,300,60,30);
        lab_temp[4]->setText(tr("Y[mm]:"));
        lab_temp[4]->setGeometry(10,360,60,30);
        lab_temp[5]->setText(tr("Z[mm]:"));
        lab_temp[5]->setGeometry(10,420,60,30);
        lab_temp[6]->setText(tr("A[°]:"));
        lab_temp[6]->setGeometry(230,300,60,30);
        lab_temp[7]->setText(tr("B[°]:"));
        lab_temp[7]->setGeometry(230,360,60,30);
        lab_temp[8]->setText(tr("C[°]:"));
        lab_temp[8]->setGeometry(230,420,60,30);

        for( int i = 0 ; i < 6 ; i++ )
        {
            lab_base2[i]=new QLabel(frame_child[1]);
            lab_base2[i]->show();
        }
        lab_base2[0]->setGeometry(90,300,120,30);
        lab_base2[1]->setGeometry(90,360,120,30);
        lab_base2[2]->setGeometry(90,420,120,30);
        lab_base2[3]->setGeometry(310,300,120,30);
        lab_base2[4]->setGeometry(310,360,120,30);
        lab_base2[5]->setGeometry(310,420,120,30);
        /*显示工具编号名称*/
        lab_toolname = new QLabel(frame_child[1]);
        lab_toolname->show();
        lab_toolname->setGeometry(150,130,150,30);

        cb_tool = new QComboBox(frame_child[1]);
        cb_tool->setGeometry(150,10,200,35);
        cb_tool->show();
        cb_tool->setStyleSheet("QComboBox:editable{background: white;} QComboBox::drop-down{ width:60;height:50}");
        connect(cb_tool,SIGNAL(activated(int)),this,SLOT(slot_CbClicked(int)));
    }
    frame_child[1]->show();
    frame_child[1]->raise();
    set_Cb_Tool(cb_tool);
    if(KukaRob.indext < 0)
        return;
    cb_tool->setCurrentIndex(KukaRob.indext);
    lab_toolname->setText(codedata[14][KukaRob.indext].name);

    for( int i = 0 ; i < 6 ; i++ )
        lab_base2[i]->setText(QString::number(codedata[14][KukaRob.indext].val.tooldatVal.toolval[i]));

    for(int i=0;i<7;i++)   //整个Base下方的功能按钮
    {
        btn_base[i]->setText("");
        btn_base[i]->setEnabled(false);
    }
    btn_base[4]->setText(tr("返回"));
    btn_base[4]->setEnabled(true);
    btn_base[5]->setText(tr("继续"));
    btn_base[5]->setEnabled(true);

    currentpage = 2;
}

void Build_Base::frame_Orient()
{
    if(frame_child[2] == NULL)
    {
        frame_child[2]=new QFrame(frame_base);
        frame_child[2]->setGeometry(0,30,494,617);
        frame_child[2]->setStyleSheet("background-color:rgb(230,230,230)");
        frame_child[2]->show();
        frame_child[2]->raise();

        for(int i=0;i<7;i++)   //第三页里7个标签的初始化
        {
            lab_base3[i]=new QLabel(frame_child[2]);
            lab_base3[i]->show();
        }
        lab_base3[0]->setText(tr("参考工具编号"));
        lab_base3[0]->setGeometry(10,10,100,30);
        lab_base3[1]->setText(tr("基坐标系统号"));
        lab_base3[1]->setGeometry(10,60,100,30);
        lab_base3[2]->setText(tr("基坐标系名称："));
        lab_base3[2]->setGeometry(10,110,100,30);
        lab_base3[3]->setText(tr("将TCP移至新基坐标系的原点"));
        lab_base3[3]->setGeometry(10,160,400,30);

        lab_base3[4]->setGeometry(180,10,60,30);   //参考工具编号
        lab_base3[5]->setGeometry(180,60,60,30);   //基坐标系编号
        lab_base3[6]->setGeometry(180,110,150,30);  //基坐标系名称
    }
    frame_child[2]->show();
    frame_child[2]->raise();
    lab_base3[4]->setText(QString::number(TCPNum));    //参考工具编号
    lab_base3[5]->setText(QString::number(BASENum));    //基坐标系编号
    lab_base3[6]->setText(BASEName);    //基坐标系名称

    for(int i=0;i<7;i++)   //整个Base下方的功能按钮
    {
        btn_base[i]->setText("");
        btn_base[i]->setEnabled(false);
    }
    btn_base[0]->setText(tr("测量"));
    btn_base[0]->setEnabled(true);
    btn_base[4]->setText(tr("返回"));
    btn_base[4]->setEnabled(true);
    btn_base[5]->setText(tr("继续"));
    btn_base[5]->setEnabled(false);
    currentpage = 3;
}

void Build_Base::frame_Res()
{
    if(frame_child[3] == NULL)
    {
        frame_child[3]=new QFrame(frame_base);
        frame_child[3]->setGeometry(0,30,494,617);
        frame_child[3]->setStyleSheet("background-color:rgb(230,230,230)");
        frame_child[3]->show();
        frame_child[3]->raise();

        for(int i=0;i<17;i++)
        {
            lab_base4[i]=new QLabel(frame_child[3]);
            lab_base4[i]->show();
        }
        lab_base4[0]->setText(tr("基坐标系统号"));
        lab_base4[0]->setGeometry(10,10,100,30);
        lab_base4[1]->setText(tr("基坐标系名称："));
        lab_base4[1]->setGeometry(10,60,100,30);
        lab_base4[2]->setText(tr("按下“保存”后，数据才被采用。"));
        lab_base4[2]->setGeometry(10,110,250,30);

        lab_base4[3]->setGeometry(180,10,60,30);   //基坐标系统号
        lab_base4[4]->setGeometry(180,60,60,30);   //基坐标系名称

        lab_base4[5]->setText(tr("X[mm]:"));
        lab_base4[5]->setGeometry(10,300,60,30);
        lab_base4[6]->setText(tr("Y[mm]:"));
        lab_base4[6]->setGeometry(10,360,60,30);
        lab_base4[7]->setText(tr("Z[mm]:"));
        lab_base4[7]->setGeometry(10,420,60,30);

        lab_base4[8]->setText(tr("A[°]:"));
        lab_base4[8]->setGeometry(230,300,60,30);
        lab_base4[9]->setText(tr("B[°]:"));
        lab_base4[9]->setGeometry(230,360,60,30);
        lab_base4[10]->setText(tr("C[°]:"));
        lab_base4[10]->setGeometry(230,420,60,30);

        lab_base4[11]->setGeometry(90,300,120,30);
        lab_base4[12]->setGeometry(90,360,120,30);
        lab_base4[13]->setGeometry(90,420,120,30);
        lab_base4[14]->setGeometry(310,300,60,30);
        lab_base4[15]->setGeometry(310,360,60,30);
        lab_base4[16]->setGeometry(310,420,60,30);
    }
    else
    {
        frame_child[3]->show();
        frame_child[3]->raise();
    }
    lab_base4[3]->setText(QString::number(BASENum));   //基坐标系统号
    lab_base4[4]->setText(BASEName);    //基坐标系名称

    lab_base4[11]->setText(QString::number(baseinfo[0]));
    lab_base4[12]->setText(QString::number(baseinfo[1]));
    lab_base4[13]->setText(QString::number(baseinfo[2]));
    lab_base4[14]->setText(QString::number(baseinfo[3]));
    lab_base4[15]->setText(QString::number(baseinfo[4]));
    lab_base4[16]->setText(QString::number(baseinfo[5]));

    for(int i=0;i<7;i++)   //整个Base下方的功能按钮
    {
        btn_base[i]->setText("");
        btn_base[i]->setEnabled(false);
    }
    btn_base[3]->setText(tr("测量点"));
    btn_base[3]->setEnabled(true);
    btn_base[4]->setText(tr("返回"));
    btn_base[4]->setEnabled(true);
    btn_base[5]->setText(tr("保存"));
    btn_base[5]->setEnabled(true);

    currentpage = 4;
}

void Build_Base::slot_BtnClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if( !btn )
        return;
    if(btn == btn_base[0])
    {
        if(btn->text() == tr("测量"))
            func_measure();
    }
    else if(btn == btn_base[4])
    {
        func_back();
    }
    else if(btn == btn_base[5])
    {
        if(btn->text() == tr("继续"))
            func_continue();
        else
            func_save();
    }
}

void Build_Base::slot_CbClicked(int index)
{
    QComboBox *cb = qobject_cast<QComboBox*>(sender());
    if( !cb )
        return;
    if( cb == cb_base)
    {
        BASENum = index;
        line_name->setText(codedata[12][index].name);
    }
    else if( cb == cb_tool)
    {
        TCPNum = index;
        TCPName = codedata[14][index].name;
        lab_toolname->setText(TCPName);
    }
}

/*
** 函数功能：进入下一个界面
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void Build_Base::func_continue()
{
    if(currentpage == 1)   //当前正在第一页
    {
        BASEName = line_name->text();  //保存基坐标名称
        BASENum = cb_base->currentIndex();
        if(VarIsStandable(BASEName))
            frame_SelectTool();
        else
            QMessageBox::warning(this,"创建基坐标","基坐标非法！请检查您的基坐标名称","确定");
    }
    else if(currentpage == 2)   //当前正在第二页
    {
        TCPNum = cb_tool->currentIndex();
        TCPName = codedata[14][TCPNum].name;
        frame_Orient();
    }
    else if(currentpage == 3)   //当前正在第三页
    {
        return;
    }
    else if(currentpage == 4)   //当前正在第四页,"继续"按钮变成"保存"按钮
    {
        return;
    }
}

/*
** 函数功能：测量三点界面
** 函数参数：无
** 函数返回值：无
** 函数说明：只是界面，主要调用func_RealMeasure()函数来记录点
*/
void Build_Base::func_measure()
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
        if(func_RealMeasure() == 1)
        {
            count++;
            if(count == 1)
                lab_base3[3]->setText(tr("将TCP移至新基坐标系的X轴正向上的一点"));
            if(count == 2)
                lab_base3[3]->setText(tr("将TCP移至新基坐标系的XY平面上一个带有正Y值的点"));
            if(count == 3)
            {
                frame_Res();
                count--;
            }
        }
    }
}


/*
** 函数功能：界面返回
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void Build_Base::func_back()
{
    if(currentpage == 4)   //当前正在第四页
    {
        frame_Orient();
    }
    else if(currentpage == 3)   //当前正在第三页
    {
        count--;
        if(count == 1)
            lab_base3[3]->setText(tr("将TCP移至新基坐标系的X轴正向上的一点"));
        if(count == 0)
            lab_base3[3]->setText(tr("将TCP移至新基坐标系的原点"));
        if(count == -1)
        {
            count=0;
            frame_SelectTool();
        }
    }
    else if(currentpage == 2)   //当前正在第二页
    {
        frame_SelectBase();
    }
    else if(currentpage == 1)   //当前正在第一页
    {
        return;
    }
}

void Build_Base::func_save()
{
    QStringList valuelist;
    valuelist.clear();
    for(int i = 0 ; i< 6 ; i++)
    {
        QString value = QString::number(baseinfo[i]);
        valuelist<<value;
    }
    QString svalue;
    svalue.clear();
    QString member[6]={"X "," , Y "," , Z "," , A "," , B "," , C "};
    for(int i = 0 ; i < 6 ;i++)
    {
        svalue += member[i] + valuelist.at(i);
    }
    codedata[12][BASENum].name = BASEName;
    codedata[12][BASENum].val.basedatVal.setbasedatval(valuelist);
    codedata[12][BASENum].Svalue = svalue;
}


int Build_Base::func_RealMeasure()
{
    return 1;
}
Build_Base::~Build_Base()
{

}

void set_Cb_Base(QComboBox* cb)
{
    cb->clear();
    int baseindex = TypeToIndex.value("BASEDAT");
    for(int i=0;i<32;i++)
    {
        cb->insertItem(i,codedata[baseindex][i].name+"      ["+QString::number(i)+"]");
    }
}
