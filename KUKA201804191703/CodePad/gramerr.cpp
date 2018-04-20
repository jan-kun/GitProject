#include "gramerr.h"

GramErr::GramErr(QWidget *parent) :
    QWidget(parent)
{
    func_buildFrame();
}

void GramErr::func_buildFrame()
{
    frame_base = new QFrame(this);
    frame_base->setGeometry(0,0,494,322);

    lab_title = new QLabel(frame_base);
    lab_title->setGeometry(0,0,494,30);
    lab_title->show();
    lab_title->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
    lab_title->setText("   故障显示");

    tab_err=new QTableWidget(1,4,frame_base);
    tab_err->show();
    tab_err->setGeometry(0,30,414,242);
    QStringList header;
    header<<tr("行")<<tr("列")<<tr("错误编码")<<tr("说明");
    tab_err->setHorizontalHeaderLabels(header);
    tab_err->horizontalHeader()->setHighlightSections(false);
    tab_err->verticalHeader()->setVisible(false);
    tab_err->verticalHeader()->setDefaultSectionSize(30);
    tab_err->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width: 50px;}");
    tab_err->setSelectionBehavior(QAbstractItemView::SelectRows);
    tab_err->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tab_err->setSelectionMode(QAbstractItemView::SingleSelection);
    tab_err->setStyleSheet("font-size:15px;color:black;background-color:rgb(230,230,230);border-top:1px solid black;");
    tab_err->setColumnWidth(0,50);
    tab_err->setColumnWidth(1,50);
    tab_err->setColumnWidth(2,50);
    tab_err->setColumnWidth(3,264);
    connect(tab_err,SIGNAL(cellClicked(int,int)),this,SLOT(slot_tabClicked(int,int)));

    for(int i = 0 ; i < 6 ; i++)
    {
        btn_errFunc[i] = new QPushButton(frame_base);
        btn_errFunc[i]->show();
        btn_errFunc[i]->setGeometry(421,35+47*i,68,42);
        btn_errFunc[i]->setFocusPolicy(Qt::NoFocus);
        btn_errFunc[i]->setIconSize(QSize(68,42));
        btn_errFunc[i]->setEnabled(false);
        btn_errFunc[i]->setIcon(QIcon(":/image/image/btn_kb.png"));
        connect(btn_errFunc[i],SIGNAL(clicked()),this,SLOT(slot_btnClicked()));
    }
    btn_errFunc[0]->setIcon(QIcon(":/image/image/show.png"));
    btn_errFunc[0]->setEnabled(true);
    btn_errFunc[1]->setIcon(QIcon(":/image/image/refresh.png"));
    btn_errFunc[1]->setEnabled(true);

    lab_detail = new QLabel(frame_base);
    lab_detail->show();
    lab_detail->setGeometry(0,272,414,50);
}

void GramErr::func_updateGramErr()
{
    int count;
    tab_err->clear();
    QStringList header;
    header<<tr("行")<<tr("列")<<tr("错误编码")<<tr("说明");
    tab_err->setHorizontalHeaderLabels(header);
    count = pErr;
    tab_err->setRowCount(count);    //只显示一个任务与程序
    tab_err->setColumnCount(4);
    for(int i = 0 ; i < count ; i++)
    {
        tab_err->setItem(i,0,new QTableWidgetItem);
        tab_err->setItem(i,1,new QTableWidgetItem);
        tab_err->setItem(i,2,new QTableWidgetItem);
        tab_err->setItem(i,3,new QTableWidgetItem);
    }
    for(int i = 0 ; i < count ; i++)
    {
        tab_err->item(i,0)->setText(QString::number(gErrTab[i].line));
        tab_err->item(i,3)->setText(gErrTab[i].detail);
    }
}

void GramErr::func_setTitle(QString name)
{
    lab_title->setText(name);
}

void GramErr::hideEvent(QHideEvent *)
{
    emit aboutToHide();
}

void GramErr::slot_btnClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(!btn)
        return;
    if(btn == btn_errFunc[0])
    {
        int row = -1;
        int line;
        bool ok;
        row = tab_err->currentRow();
        if(row < 0 || row >= tab_err->rowCount())
            return;
        line = tab_err->item(row,0)->text().toInt(&ok);
        if(!ok)
            return;
        emit sig_showDetail(line);
    }
}

void GramErr::slot_tabClicked(int r, int c)
{
    QString detail;
    detail = tab_err->item(r,3)->text();
    lab_detail->setText(detail);
}
