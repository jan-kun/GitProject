#include "info.h"

QString get_tipcontext;
QString tipcontext;
extern QLabel *lab_infotext;
extern QLabel *lab_info_img;

INFO::INFO(QWidget *parent)
    : QWidget(parent)
{
    widget=new QWidget(this);
    widget->resize(494,694);
    model=new QStandardItemModel;
    view=new QListView(widget);
    view->setGeometry(0,0,494,694);
    view->setModel(model);
    view->show();
    connect(view,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_select_info(QModelIndex)));
    /*
    item=new QStandardItem;
    QDateTime time = QDateTime::currentDateTime();
    QString str=time.toString("hh:mm:ss yyyy/MM/dd");
    QPixmap pixmap1("error.png");
    item->setIcon(QIcon(pixmap1));
    item->setText(str+"\n"+"驱动器名称不一致"+"\n"+"______________________________________________________________________");
*/
    lab_noinfo = new QLabel(widget);
    lab_noinfo->setText(tr("无任何报警或提示信息，点击报警栏返回"));
    lab_noinfo->move(50,200);
    lab_noinfo->show();

    Num_OF_Info = 0;
    temp_num = 0;
    Num_OF_ERROR=0;
    Num_OF_Warning=0;
    Num_OF_Tip=0;
    Num_OF_Waiting=0;

}


/*产生信息提示*/
void INFO::slot_change_num_of_info(int warningID, QString str)
{
    if(Num_OF_Info != 0)
    {
        lab_noinfo->hide();
    }
    /*注意，信息提示列表只有100个*/
    if(Num_OF_Info<MAX_Tip_NUM)
    {
        if(Num_OF_Info > temp_num)
        {
            item[Num_OF_Info]=new MyStandardItem;
            item[Num_OF_Info]->setText(str);

            if(warningID == 0)
            {
                item[Num_OF_Info]->setIcon(QIcon(":/image/image/error.png"));
                item[Num_OF_Info]->ID = 0;
                Num_OF_ERROR++;
                emit num_of_error_changed();
            }
            if(warningID == 1)
            {
                item[Num_OF_Info]->setIcon(QIcon(":/image/image/warning.png"));
                item[Num_OF_Info]->ID = 1;
                Num_OF_Warning++;
                emit num_of_warning_changed();
            }
            if(warningID == 2)
            {
                item[Num_OF_Info]->setIcon(QIcon(":/image/image/tip.png"));
                item[Num_OF_Info]->ID = 2;
                Num_OF_Tip++;
                emit num_of_tip_changed();
            }

            model->insertRow(0,item[Num_OF_Info]);
        }

        if(Num_OF_Info < temp_num)
        {
            if(warningID == 0)
                emit num_of_error_changed();
            if(warningID == 1)
                emit num_of_warning_changed();
            if(warningID == 2)
                emit num_of_tip_changed();
        }

        temp_num = Num_OF_Info;
    }
}

void INFO::slot_select_info(QModelIndex index)
{
    get_tipcontext = index.data().toString();
    QStringList strList = get_tipcontext.split("\n");
    tipcontext = strList.at(1);
    lab_infotext->setText(tipcontext);

    current_item = (MyStandardItem *)model->itemFromIndex(index);
    if(current_item->ID == 0)
    {
        lab_info_img->setPixmap(QPixmap(":/image/image/error.png"));
    }
    if(current_item->ID == 1)
    {
        lab_info_img->setPixmap(QPixmap(":/image/image/warning.png"));
    }
    if(current_item->ID == 2)
    {
        lab_info_img->setPixmap(QPixmap(":/image/image/tip.png"));
    }
    if(current_item->ID == 3)
    {

    }
}

void INFO::slot_Has_No_info()
{
    if(Num_OF_Info == 0)
    {
        lab_noinfo->show();
    }
}


INFO::~INFO()
{

}
