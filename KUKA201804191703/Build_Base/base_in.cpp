#include "base_in.h"

Base_IN::Base_IN(QWidget *parent) :
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
    lab_tip->setText("  测量-基坐标-数字输入");
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
** 函数功能：初始界面，选择基座标号，输入基座标名称
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void Base_IN::frame_SelectBase()
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
        lab_temp[0]->setText(tr("基坐标系统号"));
        lab_temp[0]->setGeometry(10,10,100,30);
        lab_temp[1]->setText(tr("基坐标系名称:"));
        lab_temp[1]->setGeometry(10,130,100,30);
        lab_temp[2]->setText(tr("选择需更改的基座标系"));
        lab_temp[2]->setGeometry(10,200,200,30);
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
            lab_base1[i] = new QLabel(frame_child[0]);
            lab_base1[i]->show();
        }
        lab_base1[0]->setGeometry(90,300,60,30);
        lab_base1[1]->setGeometry(90,360,60,30);
        lab_base1[2]->setGeometry(90,420,60,30);
        lab_base1[3]->setGeometry(280,300,60,30);
        lab_base1[4]->setGeometry(280,360,60,30);
        lab_base1[5]->setGeometry(280,420,60,30);

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
    if(KukaRob.indexb < 0 )
        return;
    cb_base->setCurrentIndex(KukaRob.indexb);
    line_name->setText(codedata[12][KukaRob.indexb].name);

    for( int i = 0 ; i < 6 ; i++)
        lab_base1[i]->setText( QString::number(codedata[12][KukaRob.indexb].val.basedatVal.baseval[i] ) );

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
** 函数功能：输入基座标数据界面
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void Base_IN::frame_BaseData()
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
        lab_temp[0]->setText(tr("基坐标系统号"));
        lab_temp[0]->setGeometry(10,10,100,30);
        lab_temp[1]->setText(tr("基坐标系名称:"));
        lab_temp[1]->setGeometry(10,100,60,30);
        lab_temp[2]->setText(tr("输入基坐标系数据"));
        lab_temp[2]->setGeometry(10,160,300,50);
        lab_temp[3]->setText(tr("X[mm]:"));
        lab_temp[3]->setGeometry(10,300,60,30);
        lab_temp[4]->setText(tr("Y[mm]:"));
        lab_temp[4]->setGeometry(10,360,60,30);
        lab_temp[5]->setText(tr("Z[mm]:"));
        lab_temp[5]->setGeometry(10,420,60,30);
        lab_temp[6]->setText(tr("A[°]:"));
        lab_temp[6]->setGeometry(250,300,60,30);
        lab_temp[7]->setText(tr("B[°]:"));
        lab_temp[7]->setGeometry(250,360,60,30);
        lab_temp[8]->setText(tr("C[°]:"));
        lab_temp[8]->setGeometry(250,420,60,30);

        for(int i = 0 ; i < 2 ; i++)
        {
            lab_base2[i] = new QLabel(frame_child[1]);
            lab_base2[i]->show();
        }
        /*基坐标系统号*/
        lab_base2[0]->setGeometry(150,10,100,30);
        /*基坐标系名称*/
        lab_base2[1]->setGeometry(150,100,60,30);

        for(int i=0;i<6;i++)
        {
            line_data[i]=new QLineEdit(frame_child[1]);
            line_data[i]->show();
            line_data[i]->setValidator(validator);
        }
        line_data[0]->setGeometry(90,300,120,30);
        line_data[1]->setGeometry(90,360,120,30);
        line_data[2]->setGeometry(90,420,120,30);
        line_data[3]->setGeometry(330,300,120,30);
        line_data[4]->setGeometry(330,360,120,30);
        line_data[5]->setGeometry(330,420,120,30);
    }
    frame_child[1]->show();
    frame_child[1]->raise();
    lab_base2[0]->setText(QString::number(BaseNum));
    lab_base2[1]->setText(BaseName);
    for(int i = 0 ; i < 6 ; i++)
        line_data[i]->setText(QString::number(codedata[12][BaseNum].val.basedatVal.baseval[i]));

    for(int i=0;i<7;i++)   //整个Base下方的功能按钮
    {
        btn_func[i]->setText(tr(""));
        btn_func[i]->setEnabled(false);
    }
    btn_func[4]->setText(tr("返回"));
    btn_func[4]->setEnabled(true);
    btn_func[5]->setText(tr("继续"));
    btn_func[5]->setEnabled(true);
    currentPage = 2;
}

void Base_IN::frame_Res()
{
    if(frame_child[2] == NULL)
    {
        frame_child[2]=new QFrame(frame_base);
        frame_child[2]->setGeometry(0,30,494,617);
        frame_child[2]->setStyleSheet("background-color:rgb(230,230,230)");
        frame_child[2]->show();
        frame_child[2]->raise();

        QLabel *lab_temp[9];
        for(int i=0;i<9;i++)
        {
            lab_temp[i]=new QLabel(frame_child[2]);
            lab_temp[i]->show();
        }
        lab_temp[0]->setText(tr("基座标系统号"));
        lab_temp[0]->setGeometry(10,10,100,30);
        lab_temp[1]->setText(tr("基座标系名称 : "));
        lab_temp[1]->setGeometry(10,130,100,30);
        lab_temp[2]->setText(tr("按下“保存”后，数据才被采用。"));
        lab_temp[2]->setGeometry(10,200,300,30);
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

        for(int i=0;i<2;i++)
        {
            lab_base[i]=new QLabel(frame_child[2]);
            lab_base[i]->show();
        }
        /*显示基座标编号*/
        lab_base[0]->setGeometry(150,10,60,30);
        /*显示基座标名称*/
        lab_base[1]->setGeometry(150,130,60,30);

        for(int i=0;i<6;i++)
        {
            lab_base3[i]=new QLabel(frame_child[2]);
            lab_base3[i]->show();
        }
        lab_base3[0]->setGeometry(90,300,120,30);
        lab_base3[1]->setGeometry(90,360,120,30);
        lab_base3[2]->setGeometry(90,420,120,30);
        lab_base3[3]->setGeometry(310,300,120,30);
        lab_base3[4]->setGeometry(310,360,120,30);
        lab_base3[5]->setGeometry(310,420,120,30);
    }
    frame_child[2]->show();
    frame_child[2]->raise();
    lab_base[0]->setText(QString::number(BaseNum));
    lab_base[1]->setText(BaseName);
    for(int i = 0 ; i < 6 ; i++)
        lab_base3[i]->setText(QString::number(basedata[i]));
    for(int i=0;i<7;i++)   //整个Base下方的功能按钮
    {
        btn_func[i]->setText(tr(""));
        btn_func[i]->setEnabled(false);
    }
    btn_func[4]->setText(tr("返回"));
    btn_func[4]->setEnabled(true);
    btn_func[5]->setText(tr("保存"));
    btn_func[5]->setEnabled(true);

    currentPage = 3;
}

void Base_IN::func_continue()
{
    if(currentPage == 1)
    {
        BaseNum = cb_base->currentIndex();
        BaseName = line_name->text();
        if(!VarIsStandable(BaseName))
        {
            QMessageBox::warning(this,"创建基坐标","基坐标非法！请检查您的基坐标名称","确定");
            return;
        }
        if(func_JudgeDataExist(BaseName) == 1)
        {
            QMessageBox::warning(this,"创建工具坐标","工具名称已存在！请重新输入","确定");
            return;
        }
        frame_BaseData();
    }
    else if(currentPage == 2)
    {
        bool ok;
        for(int i = 0 ; i < 6 ; i++)
        {
            basedata[i] = line_data[i]->text().toDouble(&ok);
            if( !ok )
            {
                QMessageBox::warning(this,"创建基坐标","基坐标数据非法！请检查您输入的数据","确定");
                return;
            }
        }
        frame_Res();
    }
    else if(currentPage == 3)
        return;
}

void Base_IN::func_back()
{
    if(currentPage == 1)
        return;
    else if(currentPage == 2)
        frame_SelectBase();
    else if(currentPage == 3)
        frame_BaseData();
}

void Base_IN::func_save()
{
    QStringList valuelist;
    valuelist.clear();
    for(int i = 0 ; i< 6 ; i++)
    {
        QString value = lab_base3[i]->text();
        valuelist<<value;
    }
    QString svalue;
    svalue.clear();
    QString member[6]={"{ X "," , Y "," , Z "," , A "," , B "," , C "};
    for(int i = 0 ; i < 6 ;i++)
    {
        svalue += member[i] + valuelist.at(i);
    }
    svalue += " }";
    codedata[12][BaseNum].name = BaseName;
    codedata[12][BaseNum].val.basedatVal.setbasedatval(valuelist);
    codedata[12][BaseNum].Svalue = svalue;
    func_CreatBaseDataDecl(codedata[12][BaseNum]);
}

void Base_IN::func_CreatBaseDataDecl(CodeData data)
{
    QTextCursor temptc;
    QPlainTextEdit *tempedit;
    QString datpath;
    QString name,value,insertstr;

    datpath = RobSysDataFile;
    name = data.name;
    value = data.Svalue;
    insertstr = "DECL BASEDAT "+QString::number(BaseNum)+" "+name+" = "+value;

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
    int res = func_CursorGoStr(tempedit,"DECL BASEDAT "+QString::number(BaseNum));
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

void Base_IN::slot_BtnClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if( !btn )
        return;
    if(btn == btn_func[4] )
    {
        if(btn->text() == tr("返回") )
            func_back();
    }
    else if(btn == btn_func[5] )
    {
        if(btn->text() == tr("继续"))
            func_continue();
        else if(btn->text() == tr("保存"))
            func_save();
    }
}

void Base_IN::slot_CbClicked(int index)
{
    BaseNum = index;
    line_name->setText(codedata[12][index].name);
    for( int i = 0 ; i < 6 ; i++)
        lab_base1[i]->setText( QString::number(codedata[12][index].val.basedatVal.baseval[i] ) );

}
