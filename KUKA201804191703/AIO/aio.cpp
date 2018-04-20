#include "aio.h"

int AINPUT_EXIST=1;

extern QList<QWidget *> focusitem;
extern QList<QTableWidgetItem *> focustableitem;

AIO::AIO(QWidget *parent)
    : QWidget(parent)
{
    QWidget *widget=new QWidget(this);
    widget->resize(494,694);
    widget->setObjectName("w2");
    widget->setStyleSheet("#w2{background-color:rgb(200,200,200)}");

    QLabel *lab_tip;
    lab_tip = new QLabel(widget);
    lab_tip->setGeometry(0,0,494,30);
    lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
    lab_tip->setText("   模拟输入/输出端");

    itable=new QTableWidget(32,3,widget);
    itable->setGeometry(5,35,408,604);
    QStringList header;
    header<<tr("编号")<<tr("电压[伏]")<<tr("名称");
    itable->setHorizontalHeaderLabels(header);
    itable->horizontalHeader()->setHighlightSections(false);
    itable->verticalHeader()->setVisible(false);
    itable->verticalHeader()->setDefaultSectionSize(30);
    itable->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 50px;}");
    itable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    itable->setSelectionBehavior(QAbstractItemView::SelectRows);
    itable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    itable->setSelectionMode(QAbstractItemView::SingleSelection);
    itable->setIconSize(QSize(30,30));
    itable->setStyleSheet("font-size:15px;color:black;background-color:rgb(230,230,230);border-top:1px solid black;");
    itable->setColumnWidth(0,60);
    itable->setColumnWidth(1,115);
    itable->setColumnWidth(2,180);

    for(int i=0;i<32;i++)
    {
        itable->setItem(i,0,new QTableWidgetItem(QString::number(i+1)));
        itable->setItem(i,1,new QTableWidgetItem(QString::number(0.000)));
        itable->setItem(i,2,new QTableWidgetItem("Analogeingang"));
    }

    otable=new QTableWidget(32,3,widget);
    otable->setGeometry(5,35,408,604);
    otable->setHorizontalHeaderLabels(header);
    otable->horizontalHeader()->setHighlightSections(false);
    otable->verticalHeader()->setVisible(false);
    otable->verticalHeader()->setDefaultSectionSize(30);
    otable->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 50px;}");
    otable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    otable->setSelectionBehavior(QAbstractItemView::SelectRows);
    otable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    otable->setSelectionMode(QAbstractItemView::SingleSelection);
    otable->setIconSize(QSize(30,30));
    otable->setStyleSheet("font-size:15px;color:black;background-color:rgb(230,230,230);border-top:1px solid black;");
    otable->setColumnWidth(0,60);
    otable->setColumnWidth(1,115);
    otable->setColumnWidth(2,180);

    for(int i=0;i<32;i++)
    {
        otable->setItem(i,0,new QTableWidgetItem(QString::number(i+1)));
        otable->setItem(i,1,new QTableWidgetItem(QString::number(0.000)));
        otable->setItem(i,2,new QTableWidgetItem("Analogausgang"));
    }
    otable->hide();

    for(int i = 0 ; i < 6 ;i++)
    {
        btn_aio[i]=new QPushButton(widget);
        btn_aio[i]->setGeometry(421,35+55*i,68,42);
        btn_aio[i]->setFocusPolicy(Qt::NoFocus);
        btn_aio[i]->setIconSize(QSize(68,42));
        btn_aio[i]->setEnabled(false);
    }
    btn_aio[0]->setIcon(QIcon(":/image/image/btn_kb.png"));
    btn_aio[1]->setIcon(QIcon(":/image/image/btn_kb.png"));
    btn_aio[2]->setIcon(QIcon(":/image/image/to.png"));
    btn_aio[2]->setEnabled(true);
    btn_aio[3]->setIcon(QIcon(":/image/image/name.png"));
    btn_aio[3]->setEnabled(true);
    btn_aio[4]->setIcon(QIcon(":/image/image/btn_kb.png"));
    btn_aio[5]->setIcon(QIcon(":/image/image/btn_kb.png"));

    input_btn = new QPushButton(tr("输入端"),widget);
    input_btn->setGeometry(5,639,70,50);
    input_btn->setFocusPolicy(Qt::NoFocus);
    input_btn->setStyleSheet("background-color:rgb(230,230,230)");

    output_btn = new QPushButton(tr("输出端"),widget);
    output_btn->setGeometry(75,639,70,50);
    output_btn->setFocusPolicy(Qt::NoFocus);
    output_btn->setStyleSheet("background-color:rgb(200,200,200)");

    connect(btn_aio[2],SIGNAL(clicked(bool)),this,SLOT(find_num()));
    connect(btn_aio[3],SIGNAL(clicked(bool)),this,SLOT(slot_name()));
    connect(itable,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(slot_item_clicked(QTableWidgetItem *)));
    connect(otable,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(slot_item_clicked(QTableWidgetItem *)));
    connect(input_btn,SIGNAL(clicked(bool)),this,SLOT(slot_showinput()));
    connect(output_btn,SIGNAL(clicked(bool)),this,SLOT(slot_showoutput()));
}

void AIO::find_num()
{
    bool ok;
    QString string = QInputDialog::getText(this,tr("至:..."),tr("至:"),QLineEdit::Normal,tr("0"),&ok);
    QList<QTableWidgetItem *> item;
    if(!string.isEmpty() && (string.toInt() >= 1 && string.toInt() <= 32) )
    {
        if(AINPUT_EXIST)
        {
            item=itable->findItems(string,Qt::MatchExactly);
            itable->setCurrentItem(item.at(0));
        }
        else
        {
            item=otable->findItems(string,Qt::MatchExactly);
            otable->setCurrentItem(item.at(0));
        }
    }


}


void AIO::slot_name()
{

}

void AIO::slot_item_clicked(QTableWidgetItem *item)
{

}


void AIO::slot_showinput()
{
    otable->hide();
    itable->show();
    input_btn->setStyleSheet("background-color:rgb(230,230,230)");
    output_btn->setStyleSheet("background-color:rgb(200,200,200)");
    AINPUT_EXIST = 1;
}


void AIO::slot_showoutput()
{
    itable->hide();
    otable->show();
    output_btn->setStyleSheet("background-color:rgb(230,230,230)");
    input_btn->setStyleSheet("background-color:rgb(200,200,200)");
    AINPUT_EXIST = 0;
}

AIO::~AIO()
{

}
