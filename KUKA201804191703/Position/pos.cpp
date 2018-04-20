#include "pos.h"
#include"Protocol/message.h"
double Fpos[7];   //存储xyzq1q2q3q4的数据

Pos::Pos(QWidget *parent)
    : QWidget(parent)
{
    QWidget *widget=new QWidget(this);
    widget->resize(494,694);
    widget->setObjectName("w3");
    widget->setStyleSheet("#w3{background-color:rgb(230,230,230)}");

    lab_tip = new QLabel(widget);
    lab_tip->setGeometry(0,0,494,30);
    lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
    lab_tip->setText("   机器人位置（笛卡尔式）");

    /*笛卡尔式*/
    dtable=new QTableWidget(16,3,widget);
    dtable->setGeometry(5,35,408,604);
    QStringList header;
    header<<tr("名称")<<tr("值")<<tr("单位");
    dtable->setHorizontalHeaderLabels(header);
    dtable->horizontalHeader()->setStyleSheet("border:none;background-color:rgb(200,200,200)");
    dtable->horizontalHeader()->setHighlightSections(false);
    dtable->verticalHeader()->setVisible(false);
    dtable->verticalHeader()->setDefaultSectionSize(30);
    dtable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dtable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dtable->setSelectionBehavior(QAbstractItemView::SelectRows);
    dtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    dtable->setSelectionMode(QAbstractItemView::SingleSelection);
    dtable->setStyleSheet("font-size:15px;color:black;background-color:rgb(230,230,230);border:1px solid black;");
    dtable->setColumnWidth(0,160);
    dtable->setColumnWidth(1,160);
    dtable->setColumnWidth(2,85);

    dtable->setItem(0,0,new QTableWidgetItem(tr("位置")));
    dtable->setItem(1,0,new QTableWidgetItem(tr("  X")));
    dtable->setItem(2,0,new QTableWidgetItem(tr("  Y")));
    dtable->setItem(3,0,new QTableWidgetItem(tr("  Z")));

    dtable->setItem(4,0,new QTableWidgetItem(tr("取向")));
    dtable->setItem(5,0,new QTableWidgetItem(tr("  A")));
    dtable->setItem(6,0,new QTableWidgetItem(tr("  B")));
    dtable->setItem(7,0,new QTableWidgetItem(tr("  C")));

    dtable->setItem(8,0,new QTableWidgetItem(tr("机器人位置")));
    dtable->setItem(9,0,new QTableWidgetItem(tr("  S")));
    dtable->setItem(10,0,new QTableWidgetItem(tr("  T")));

    dtable->setItem(1,1,new QTableWidgetItem( QString::number(0) ) );
    dtable->setItem(2,1,new QTableWidgetItem( QString::number(0) ) );
    dtable->setItem(3,1,new QTableWidgetItem( QString::number(0) ) );

    dtable->setItem(5,1,new QTableWidgetItem( QString::number(0) ) );
    dtable->setItem(6,1,new QTableWidgetItem( QString::number(0) ) );
    dtable->setItem(7,1,new QTableWidgetItem( QString::number(0) ) );
    dtable->setItem(9,1,new QTableWidgetItem( QString::number(0) ) );
    dtable->setItem(10,1,new QTableWidgetItem( QString::number(0) ) );

    dtable->setItem(1,2,new QTableWidgetItem( "mm" ) );
    dtable->setItem(2,2,new QTableWidgetItem( "mm" ) );
    dtable->setItem(3,2,new QTableWidgetItem( "mm" ) );

    dtable->setItem(5,2,new QTableWidgetItem( "deg" ) );
    dtable->setItem(6,2,new QTableWidgetItem( "deg" ) );
    dtable->setItem(7,2,new QTableWidgetItem( "deg" ) );
    dtable->setItem(9,2,new QTableWidgetItem( "二进制" ) );
    dtable->setItem(10,2,new QTableWidgetItem( "二进制" ) );

    DKR_EXIST = 1;  //DKR_EXIST为1表示当前为笛卡尔坐标系,为0表示当前为轴位置

    /*轴位置*/
    ztable=new QTableWidget(16,4,widget);
    ztable->setGeometry(5,35,408,604);
    QStringList header1;
    header1<<tr("  ")<<tr("轴")<<tr("位置[度,mm]")<<tr("电机[deg]");
    ztable->setHorizontalHeaderLabels(header1);
    ztable->horizontalHeader()->setStyleSheet("border:none;background-color:rgb(200,200,200)");
    ztable->horizontalHeader()->setHighlightSections(false);
    ztable->verticalHeader()->setVisible(false);
    ztable->verticalHeader()->setDefaultSectionSize(30);
    ztable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ztable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ztable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ztable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ztable->setSelectionMode(QAbstractItemView::SingleSelection);
    ztable->setStyleSheet("font-size:15px;color:black;background-color:rgb(230,230,230);border:1px solid black;");
    ztable->setColumnWidth(0,35);
    ztable->setColumnWidth(1,124);
    ztable->setColumnWidth(2,124);
    ztable->setColumnWidth(3,124);

    for(int i=0;i<6;i++)
    {
        QLabel *label=new QLabel();
        label->resize(30,30);
        label->setStyleSheet("background: transparent;border:none");
        label->setPixmap(QPixmap(":/image/image/anix.png"));

        label->setLineWidth(0);
        ztable->setCellWidget(i,0,label);
        ztable->resizeColumnToContents(0);
        ztable->setItem(i,1,new QTableWidgetItem(tr("A")+QString::number(i+1)));
        ztable->setItem(i,2,new QTableWidgetItem(QString::number(0.000)));
        ztable->setItem(i,3,new QTableWidgetItem(QString::number(0.000)));
    }
    ztable->hide();

    btn_mod = new QPushButton(widget);
    btn_mod->setGeometry(421,35,68,42);
    btn_mod->setFocusPolicy(Qt::NoFocus);
    btn_mod->setIconSize(QSize(68,42));
    btn_mod->setIcon(QIcon(":/image/image/btn_z.png"));
    connect(btn_mod,SIGNAL(clicked(bool)),this,SLOT(slot_mod_changed()));
    for(int i=0;i<5;i++)
    {
        btn_kong[i]=new QPushButton(widget);
        btn_kong[i]->setGeometry(421,90+i*55,68,42);
        btn_kong[i]->setEnabled(false);
        btn_kong[i]->setIconSize(QSize(68,42));
        btn_kong[i]->setIcon(QIcon(":/image/image/btn_kb.png"));
    }

}

void Pos::func_updateJogPos()
{
    for(int i=0;i<6;i++)
    {
        ztable->item(i,2)->setText(QString::number( double( CurPoint.aAxis[i])/100000*RadTDege , 10, 2));
    }
}

void Pos::func_updateDescartesPos()
{
    GetJntRec(Fpos,&KukaRob, &xSys,0);
    dtable->item(1,1)->setText(QString::number(Fpos[0] , 10, 2));
    dtable->item(2,1)->setText(QString::number(Fpos[1] , 10, 2));
    dtable->item(3,1)->setText(QString::number(Fpos[2] , 10, 2));
    dtable->item(5,1)->setText(QString::number(Fpos[3] , 10, 2));
    dtable->item(6,1)->setText(QString::number(Fpos[4] , 10, 2));
    dtable->item(7,1)->setText(QString::number(Fpos[5] , 10, 2));
}

void Pos::slot_mod_changed()
{
        if(DKR_EXIST == 1)
        {
            dtable->hide();
            ztable->show();
            btn_mod->setIcon(QIcon(":/image/image/btn_d.png"));
            lab_tip->setText("   机器人位置（与轴相关的）");
            DKR_EXIST = 0;
        }
        else
        {
            ztable->hide();
            dtable->show();
            btn_mod->setIcon(QIcon(":/image/image/btn_z.png"));
            lab_tip->setText("   机器人位置（笛卡尔式）");
            DKR_EXIST = 1;
        }
}


void Pos::Current_Pos_Changed()
{
    ztable->item(0,2)->setText(QString::number( CurPoint.aAxis[0] ));
    ztable->item(1,2)->setText(QString::number( CurPoint.aAxis[1] ));
    ztable->item(2,2)->setText(QString::number( CurPoint.aAxis[2] ));
    ztable->item(3,2)->setText(QString::number( CurPoint.aAxis[3] ));
    ztable->item(4,2)->setText(QString::number( CurPoint.aAxis[4] ));
    ztable->item(5,2)->setText(QString::number( CurPoint.aAxis[5] ));
}

Pos::~Pos()
{

}

void GetJntRec(double *pAix,SysRob  *pASys, sys *pSys,int iCoor)
{
    double t[7];
    if(iCoor==0 || iCoor==1)
    {
        Eula2Quat(pSys->xV20.aAxis+8,pAix,100000);
    }
    else
    {
        Eula2Quat(pSys->xV20.aAxis+8,t,100000);   //wold
//        World2User(pAix,t,pASys->CurBase.val..wobjval);
    }
}


