#include "dio.h"

int DINPUT_EXIST=1;

extern QList<QWidget *> focusitem;
extern QList<QTableWidgetItem *> focustableitem;

QBitArray DInput;
QBitArray DOutput;

DIO::DIO(QWidget *parent)
    : QWidget(parent)
{
    QWidget *widget=new QWidget(this);
    widget->resize(494,694);
    widget->setObjectName("w1");
    widget->setStyleSheet("#w1{background-color:rgb(200,200,200)}");

    QLabel *lab_tip;
    lab_tip = new QLabel(widget);
    lab_tip->setGeometry(0,0,494,30);
    lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
    lab_tip->setText("   数字输入/输出端");
    /*数字输入端*/
    ditable=new QTableWidget(4096,4,widget);
    ditable->setGeometry(5,35,408,604);
    QStringList header;
    header<<tr("编号")<<tr("值")<<tr("状态")<<tr("名称");
    ditable->setHorizontalHeaderLabels(header);
    ditable->horizontalHeader()->setHighlightSections(false);
    ditable->verticalHeader()->setVisible(false);
    ditable->verticalHeader()->setDefaultSectionSize(30);
    ditable->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 50px;}");
    ditable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ditable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ditable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ditable->setSelectionMode(QAbstractItemView::SingleSelection);
    ditable->setStyleSheet("font-size:20px;color:rgb(34,187,255)");
    ditable->setIconSize(QSize(30,30));
    ditable->setStyleSheet("background-color:rgb(230,230,230);border-top:1px solid black;");
    ditable->setColumnWidth(0,51);
    ditable->setColumnWidth(1,51);
    ditable->setColumnWidth(2,53);
    ditable->setColumnWidth(3,200);

    for(int i=0;i<4096;i++)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(i+1));
        ditable->setItem(i,0,item1);
        QTableWidgetItem *item2 = new QTableWidgetItem(QIcon(":/image/image/white.png"),"",0);
        ditable->setItem(i,1,item2);
        ditable->setItem(i,3,new QTableWidgetItem("Eingang"));
    }

    /*数字输出端*/
    dotable=new QTableWidget(4096,4,widget);
    dotable->setGeometry(5,35,408,604);
    dotable->setHorizontalHeaderLabels(header);
    dotable->horizontalHeader()->setHighlightSections(false);
    dotable->verticalHeader()->setVisible(false);
    dotable->verticalHeader()->setDefaultSectionSize(30);
    dotable->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 50px;}");
    dotable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dotable->setSelectionBehavior(QAbstractItemView::SelectRows);
    dotable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    dotable->setSelectionMode(QAbstractItemView::SingleSelection);
    dotable->setStyleSheet("font-size:20px;color:rgb(34,187,255)");
    dotable->setIconSize(QSize(30,30));
    dotable->setStyleSheet("background-color:rgb(230,230,230);border-top:1px solid black;");
    dotable->setColumnWidth(0,51);
    dotable->setColumnWidth(1,51);
    dotable->setColumnWidth(2,53);
    dotable->setColumnWidth(3,200);

    for(int i=0;i<4096;i++)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(i+1));
        dotable->setItem(i,0,item1);
        QTableWidgetItem *item2 = new QTableWidgetItem(QIcon(":/image/image/white.png"),"",0);
        dotable->setItem(i,1,item2);
        dotable->setItem(i,3,new QTableWidgetItem("Ausgang"));
    }
    dotable->hide();

    DInput.resize(4096);
    DOutput.resize(4096);

    for(int i = 0 ; i < 6 ;i++)
    {
        btn_io[i]=new QPushButton(widget);
        btn_io[i]->setGeometry(421,35+55*i,68,42);
        btn_io[i]->setFocusPolicy(Qt::NoFocus);
        btn_io[i]->setIconSize(QSize(68,42));
    }
    btn_io[0]->setIcon(QIcon(":/image/image/decl100.png"));
    btn_io[1]->setIcon(QIcon(":/image/image/add100.png"));
    btn_io[2]->setIcon(QIcon(":/image/image/to.png"));
    btn_io[3]->setIcon(QIcon(":/image/image/value.png"));
    btn_io[4]->setIcon(QIcon(":/image/image/name.png"));
    btn_io[5]->setIcon(QIcon(":/image/image/aoc.jpg"));

    input_btn = new QPushButton(tr("输入端"),widget);
    input_btn->setGeometry(5,639,70,50);
    input_btn->setFocusPolicy(Qt::NoFocus);
    input_btn->setStyleSheet("background-color:rgb(230,230,230)");

    output_btn = new QPushButton(tr("输出端"),widget);
    output_btn->setGeometry(75,639,70,50);
    output_btn->setFocusPolicy(Qt::NoFocus);
    output_btn->setStyleSheet("background-color:rgb(200,200,200)");

    connect(btn_io[0],SIGNAL(clicked(bool)),this,SLOT(slot_dec()));
    connect(btn_io[1],SIGNAL(clicked(bool)),this,SLOT(slot_add()));
    connect(btn_io[2],SIGNAL(clicked(bool)),this,SLOT(find_num()));
    connect(btn_io[3],SIGNAL(clicked(bool)),this,SLOT(slot_setValue()));
    connect(btn_io[4],SIGNAL(clicked(bool)),this,SLOT(slot_name()));
    connect(ditable,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(slot_item_clicked(QTableWidgetItem *)));
    connect(dotable,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(slot_item_clicked(QTableWidgetItem *)));
    connect(input_btn,SIGNAL(clicked(bool)),this,SLOT(slot_showinput()));
    connect(output_btn,SIGNAL(clicked(bool)),this,SLOT(slot_showoutput()));
}

/*跳转到指定行*/
void DIO::find_num()
{
    bool ok;
    QString string = QInputDialog::getText(this,tr("至:..."),tr("至:"),QLineEdit::Normal,tr("0"),&ok);
    QList<QTableWidgetItem *> item;
    if(!string.isEmpty())
    {
        if( string.toInt() >= 1 && string.toInt() <= 4096)
        {
            if(DINPUT_EXIST)
            {
                item=ditable->findItems(string,Qt::MatchExactly);
                ditable->setCurrentItem(item.at(0));
            }
            else
            {
                item=dotable->findItems(string,Qt::MatchExactly);
                dotable->setCurrentItem(item.at(0));
            }
        }
        else
            QMessageBox::warning(this,tr("查找结果"),tr("未找到指定行，请重新输入"),QMessageBox::Yes);
    }


}

void DIO::slot_add()
{
    if(DINPUT_EXIST)
    {
        int num=ditable->currentRow();
        QList<QTableWidgetItem *> item;
        if(num>=3995)
        {
            item=ditable->findItems(QString::number(4096),Qt::MatchExactly);
        }
        else
            item=ditable->findItems(QString::number(num+100+1),Qt::MatchExactly);
        ditable->setCurrentItem(item.at(0));
    }
    else
    {
        int num=dotable->currentRow();
        QList<QTableWidgetItem *> item;
        if(num>=3995)
        {
            item=dotable->findItems(QString::number(4096),Qt::MatchExactly);
        }
        else
            item=dotable->findItems(QString::number(num+100+1),Qt::MatchExactly);
        dotable->setCurrentItem(item.at(0));
    }

}

void DIO::slot_dec()
{
    if(DINPUT_EXIST)
    {
        int num=ditable->currentRow();
        QList<QTableWidgetItem *> item;
        if(num<=100)
        {
            item=ditable->findItems(QString::number(1),Qt::MatchExactly);
        }
        else
            item=ditable->findItems(QString::number(num-99),Qt::MatchExactly);
        ditable->setCurrentItem(item.at(0));
    }
    else
    {
        int num=dotable->currentRow();
        QList<QTableWidgetItem *> item;
        if(num<=100)
        {
            item=dotable->findItems(QString::number(1),Qt::MatchExactly);
        }
        else
            item=dotable->findItems(QString::number(num-99),Qt::MatchExactly);
        dotable->setCurrentItem(item.at(0));
    }

}

void DIO::slot_name()
{

}

void DIO::slot_item_clicked(QTableWidgetItem *item)
{

}



void DIO::slot_setValue()
{
    if(!dotable->isHidden())
    {
        int line = dotable->currentRow();
        if(line>=0 && line<4096)
        {
            if(DOutput.testBit(line))
            {
                dotable->setItem(line,1,new QTableWidgetItem(QIcon(":/image/image/white.png"),"",0));
                DOutput.toggleBit(line);   //反转位
            }
            else
            {
                dotable->setItem(line,1,new QTableWidgetItem(QIcon(":/image/image/green.png"),"",0));
                DOutput.toggleBit(line);   //反转位
            }
        }
        else
        {
            qDebug()<<"Out index out of range";
            QMessageBox::warning(this,tr("请选择所需要设置的行"),tr("选择所需要设置的行"),QMessageBox::Ok);
        }
    }
}


void DIO::slot_showinput()
{
    dotable->hide();
    ditable->show();
    input_btn->setStyleSheet("background-color:rgb(230,230,230)");
    output_btn->setStyleSheet("background-color:rgb(200,200,200)");
    DINPUT_EXIST = 1;
}


void DIO::slot_showoutput()
{
    ditable->hide();
    dotable->show();
    input_btn->setStyleSheet("background-color:rgb(200,200,200)");
    output_btn->setStyleSheet("background-color:rgb(230,230,230)");
    DINPUT_EXIST = 0;
}

DIO::~DIO()
{

}
