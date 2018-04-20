#include "filemange.h"
#include "code2psd.h"

FileMange::FileMange(QWidget *parent)
    : QWidget(parent)
{
    widget=new QWidget(this);
    widget->setGeometry(0,0,494,694);
    widget->setObjectName("w");
    widget->setStyleSheet("#w{background-color:rgb(230,230,230)}");

    frame_filemange = new QFrame(widget);
    frame_filemange->setGeometry(0,0,494,694);
    frame_filemange->setObjectName("ff");
    frame_filemange->setStyleSheet("#w{background-color:rgb(230,230,230)}");
    init_mainframe();
}

/*
** 函数功能：初始化资源管理器界面
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void FileMange::init_mainframe()
{
    QLabel *lab_tip;
    lab_tip = new QLabel(frame_filemange);
    lab_tip->setGeometry(0,0,494,30);
    lab_tip->setStyleSheet("background-color:rgb(252,122,35);color:white;font:bold;font-size:15px");
    lab_tip->setText("   导航器");

    lab_filter = new QLabel(frame_filemange);
    lab_filter->setGeometry(0,30,300,20);
    lab_filter->setStyleSheet("background-color:rgb(34,187,255);color:black;font-size:15px");
    lab_filter->setText("  过滤器：用户");

    lab_path = new QLabel(frame_filemange);
    lab_path->setGeometry(300,30,194,20);
    lab_path->setStyleSheet("background-color:rgb(230,230,230);color:black;font-size:15px");
    lab_path->setText("  ...的内容：");

    model = new MyModel;  //文件系统模型
    model->setRootPath(KUKA_PATH);
    QStringList filter;
//    filter<<"([a-z]|[A-Z]){1,}([a-z]|[A-Z]|[0-9]){0,30}";
    model->setNameFilters(filter);

    listFilterProxyModel = new ListFilterModel(this);
    listFilterProxyModel->setSourceModel(model);
    func_reapplyFilter(0);

    /*左侧树状视图*/
    tree =new QTreeView(frame_filemange);
    tree->setModel(model);
    tree->setRootIndex(model->index(KUKA_PATH));
    tree->setGeometry(0,50,300,574);
    tree->setHeaderHidden(true);
    tree->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tree->setStyleSheet("background-color:rgb(230,230,230)");
    /*隐藏树结构里多余的信息，只显示文件名*/
    tree->hideColumn(1);
    tree->hideColumn(2);
    tree->hideColumn(3);
    tree->hideColumn(4);
    tree->setObjectName(tr("Left_Tree"));
    tree->installEventFilter(this);
    connect(tree,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_treeClicked(QModelIndex)));
    /*右侧list结构视图*/
    list=new QListView(frame_filemange);
    list->setModel(listFilterProxyModel);
    list->setRootIndex(listFilterProxyModel->mapFromSource(model->index(KUKA_MAIZI)));
    list->setGeometry(300,50,194,574);
    list->setStyleSheet("background-color:rgb(230,230,230)");
    list->setObjectName(tr("Right_List"));
    list->installEventFilter(this);
    connect(list,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_listClicked(QModelIndex)));
    connect(list,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_listDbClicked(QModelIndex)));

    lab_b = new QLabel(frame_filemange);
    lab_b->setGeometry(0, 624, 494, 70);

    /*显示左侧树形项有几个对象，显示右侧list项有无被标记*/
    lab_count = new QLabel(lab_b);
    lab_count->setGeometry(0,0,494,20);
    lab_count->setStyleSheet("background-color:rgb(230,230,230);color:black;font-size:15px");
    lab_count->setText(" 对象");

    for(int i=0;i<7;i++)
    {
        btnCtlFunc[i]=new QPushButton(lab_b);
        btnCtlFunc[i]->setGeometry(2+70*i,25,68,42);
        btnCtlFunc[i]->setFocusPolicy(Qt::NoFocus);
        connect(btnCtlFunc[i],SIGNAL(clicked(bool)),this,SLOT(slot_BtnCtlClicked()));
        btnCtlFunc[i]->setIconSize(QSize(68,42));
    }
    btnCtlFunc[0]->setIcon(QIcon(":/image/image/btn_new.png"));
    btnCtlFunc[1]->setIcon(QIcon(":/image/image/btn_select.png"));
    btnCtlFunc[2]->setIcon(QIcon(":/image/image/btn_backup.png"));
    btnCtlFunc[3]->setIcon(QIcon(":/image/image/btn_save.png"));
    btnCtlFunc[4]->setIcon(QIcon(":/image/image/btn_del.png"));
    btnCtlFunc[5]->setIcon(QIcon(":/image/image/btn_open.png"));
    btnCtlFunc[6]->setIcon(QIcon(":/image/image/btn_edit.png"));

    win_CodePad = new CodePad(widget);
    win_CodePad->move(0,0);
    win_CodePad->hide();

    old_pa=btnCtlFunc[0]->palette();
    old_stysheet="background-color:rgb(249,249,249)";
    new_pa.setColor(QPalette::WindowText,QColor(30,144,255,0));
    new_stylesheet="background-color:rgb(57,185,255)";
}

/*
** 函数功能：判断文件夹是否为空
** 函数参数：filepath，需要判断的文件夹
** 函数返回值：为空返回true，否则返回false
** 函数说明：无
*/
bool FileMange::func_foldIsEmpty(QString filepath)
{
    QFileInfo info(filepath);
    if(info.isDir())
    {
        QDir dir(filepath);
        if(dir.count()==2)
            return true;
        else
            return false;
    }
}

/*
** 函数功能：构建编辑菜单
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void FileMange::func_BuildEditFrame()
{
    if(frame_edit == NULL)
    {
        frame_edit = new MenuWidget(widget);
        QSizePolicy sizepolicy = frame_edit->sizePolicy();
        sizepolicy.setHorizontalPolicy(QSizePolicy::Expanding);
        frame_edit->setSizePolicy(sizepolicy);
        frame_edit->move(326,0);
        frame_edit->setMinimumSize(168,652);
        frame_edit->setObjectName("fe");
        frame_edit->setStyleSheet("#fe{border:4px solid rgb(0,170,255)}");
        frame_edit->show();

        QLabel *lab_tip=new QLabel(frame_edit);
        lab_tip->setGeometry(4,3,160,17);
        lab_tip->setStyleSheet("background-color:rgb(160,165,170);color:white;font:bold;font-size:14px");
        lab_tip->setText(tr("  导航器"));
        lab_tip->show();

        for(int i=0;i<2;i++)
        {
            frameBtnEdit[i]=new QFrame(frame_edit);
            frameBtnEdit[i]->setGeometry(4+162*i,20,160,624);
            frameBtnEdit[i]->hide();
            frameBtnEdit[i]->setStyleSheet("background-color:rgb(230,230,230)");
        }
        frameBtnEdit[0]->show();
        QString str[16] = {"新","打开","全选","剪切","复制","添加","删除","备份","存档",\
                                       "打印", "改名","属性","过滤器","选择","取消选择程序","程序复位"};
        for(int i=0;i<16;i++)
        {
            btnEdit0[i]=new QPushButton(frameBtnEdit[0]);
            btnEdit0[i]->setGeometry(0,39*i,160,39);
            btnEdit0[i]->show();
            btnEdit0[i]->setText(str[i]);
            btnEdit0[i]->setFocusPolicy(Qt::NoFocus);
            btnEdit0[i]->setStyleSheet(old_stysheet);
            connect(btnEdit0[i],SIGNAL(clicked()),this,SLOT(slot_BtnEdit()));
            connect(btnEdit0[i],SIGNAL(clicked()),this,SLOT(setEditColor()));
        }

        QLabel *labpix[3];
        QPixmap pixmap;
        pixmap.load(":/arrow.PNG");
        for(int i=0;i<3;i++)
        {
            labpix[i]=new QLabel(this);
            labpix[i]->hide();
            labpix[i]->resize(20,15);
            labpix[i]->setStyleSheet("background: transparent");
            labpix[i]->setPixmap(pixmap);
        }
        labpix[0]->setParent(btnEdit0[1]);
        labpix[0]->move(130,15);
        labpix[0]->show();
        labpix[1]->setParent(btnEdit0[8]);
        labpix[1]->move(130,15);
        labpix[1]->show();
        labpix[2]->setParent(btnEdit0[13]);
        labpix[2]->move(130,15);
        labpix[2]->show();

        for(int i=0;i<3;i++)
        {
            btnEdit1[i]=new QPushButton(frameBtnEdit[1]);
            btnEdit1[i]->setGeometry(0,39*i,160,39);
            btnEdit1[i]->show();
            btnEdit1[i]->setFocusPolicy(Qt::NoFocus);
            btnEdit1[i]->setStyleSheet(old_stysheet);
            connect(btnEdit1[i],SIGNAL(clicked()),this,SLOT(slot_BtnEdit()));
        }
    }
    else
    {
        if(frame_edit->isHidden())
        {
            frame_edit->show();
            frame_edit->resize(168,652);
            frame_edit->move(326,0);
        }
        else
            frame_edit->hide();
    }
}

/*
** 函数功能：新建模块左侧界面
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void FileMange::func_FrameModule()
{
//    if(!tree->currentIndex().isValid())
//    {
//        QMessageBox::warning(this,tr("提示"),tr("创建模块文件失败，\r\r模块文件的父目录不能为空！"),\
//                             QMessageBox::Yes);
//        func_BtnChange(0);
//        return;
//    }
    if(frame_modular == NULL)
    {
        frame_modular = new QFrame(frame_filemange);
        frame_modular->setGeometry(0,30,300,574);
        frame_modular->setObjectName("fm");
        frame_modular->setStyleSheet("#fm{background-color:rgb(230,230,230)}");

        QLabel *lab_modular=new QLabel(frame_modular);
        lab_modular->show();
        lab_modular->setGeometry(0,0,240,30);
        lab_modular->setText(tr("  选择模板"));

        tab_module = new QTableWidget(6,2,frame_modular);
        tab_module->setGeometry(0,30,300,510);
        QStringList header;
        header<<tr("名称")<<tr("注释");
        tab_module->setHorizontalHeaderLabels(header);
        tab_module->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgb(230,230,230);text-align: left;}");
        tab_module->verticalHeader()->setVisible(false);
        tab_module->verticalHeader()->setDefaultSectionSize(30);
        tab_module->setColumnWidth(0,100);
        tab_module->setColumnWidth(1,195);
        tab_module->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tab_module->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tab_module->setShowGrid(false);
        tab_module->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tab_module->setSelectionMode(QAbstractItemView::SingleSelection);
        tab_module->horizontalHeader()->setHighlightSections(false);
        tab_module->setSelectionBehavior(QAbstractItemView::SelectRows);
        tab_module->setStyleSheet("color:black;selection-background-color:rgb(34,187,255)");
        tab_module->show();

        tab_module->setItem(0,0,new QTableWidgetItem("Cell"));
        tab_module->setItem(1,0,new QTableWidgetItem("Expert"));
        tab_module->setItem(2,0,new QTableWidgetItem("Expert Submit"));
        tab_module->setItem(3,0,new QTableWidgetItem("Function"));
        tab_module->setItem(4,0,new QTableWidgetItem("Modul"));
        tab_module->setItem(5,0,new QTableWidgetItem("Submit"));

        tab_module->setItem(0,1,new QTableWidgetItem("外部分配器自动运行"));
        tab_module->setItem(1,1,new QTableWidgetItem("专家模块"));
        tab_module->setItem(2,1,new QTableWidgetItem("专家"));
        tab_module->setItem(3,1,new QTableWidgetItem("功能"));
        tab_module->setItem(4,1,new QTableWidgetItem("模块"));
        tab_module->setItem(5,1,new QTableWidgetItem("提交用户"));
    }
    frame_modular->show();
    frame_modular->raise();
}

void FileMange::func_showErrList()
{
    if(win_GramErr == NULL)
    {
        win_GramErr = new GramErr(widget);
        win_GramErr->setGeometry(0,0,494,322);
        connect(win_GramErr,SIGNAL(aboutToHide()),this,SLOT(slot_restoreFrame()));
        connect(win_GramErr,SIGNAL(sig_showDetail(int)),this,SLOT(slot_showDetailErr(int)));
    }
    func_resize();
    win_GramErr->show();
    win_GramErr->func_updateGramErr();
    listwidget.prepend(win_GramErr);
}
/*
** 函数功能：新建文件或文件夹
** 函数参数：无
** 函数返回值：无
** 函数说明：通过读取class_namae来判断当前焦点在tree还是list中，在tree中创建文件夹，在list中创建文件
*/
void FileMange::func_new()
{
    if(class_name != "Left_Tree" && class_name != "Right_List")
    {
        QMessageBox::warning(this,tr("提示"),tr("光标点在左侧创建文件夹，右侧创建文件，请选择"),QMessageBox::Yes);
        return;
    }
    func_BtnChange(1);   //控制栏按钮要发生变化的
    if(frame_edit != NULL)
        frame_edit->hide();
    if(class_name == tr("Left_Tree"))
    {
        QInputDialog *inpdog = new QInputDialog(this,Qt::Dialog);
        inpdog->setFixedSize(QSize(200,200));
        inpdog->setInputMode(QInputDialog::TextInput);
        inpdog->setWindowIcon(QIcon(":/image/image/robot.png"));
        inpdog->setLabelText("输入新建文件夹名称");
        int res = inpdog->exec();
        if(res == QDialog::Accepted)
        {
            QString inputstr = inpdog->textValue();
            if(!VarIsStandable(inputstr))
            {
                QMessageBox::warning(this,tr("提示"),tr("创建文件夹失败，\r\r文件夹名称非法!"),\
                                     QMessageBox::Yes);
                func_BtnChange(0);
                return;
            }
            QModelIndex modelindx = model->mkdir(tree->currentIndex() , inputstr);
            if( !modelindx.isValid() )
            {
                QMessageBox::warning(this,tr("提示"),tr("创建文件夹失败，\r\r请检查是否有同名文件夹的存在，\n或者您输入的名称是否合法"),\
                                     QMessageBox::Yes);
                func_BtnChange(0);
                return;
            }
            func_BtnChange(0);
        }
        else if(res == QDialog::Rejected)
        {
            func_BtnChange(0);
            return;
        }
    }
    else if(class_name == tr("Right_List"))
        func_FrameModule();
}

/*
** 函数功能：创建模块文件
** 函数参数：parentindex，tree视图中的当前模型索引；name，模块名
** 函数返回值：成功，1；失败,0
** 函数说明：无
*/
int FileMange::func_creatModule(QString absoultpath , QString name)
{
    if( !VarIsStandable(name) )
    {
        QMessageBox::warning(this,tr("提示"),tr("创建模块文件失败，\r\r请检查您输入的名称是否合法"),\
                             QMessageBox::Yes);
        return 0;
    }
    if( absoultpath.isEmpty() )
    {
        QMessageBox::warning(this,tr("提示"),tr("创建模块文件失败，\r\r模块所属的父目录不能为空"),\
                             QMessageBox::Yes);
        return 0;
    }
    QFile file(absoultpath+"/"+name);
    if ( file.exists())
    {
        QMessageBox::warning(this,tr("提示"),tr("创建模块文件失败，\r\r请检查是否有同名文件的存在，或者您输入的名称是否合法"),\
                             QMessageBox::Yes);
        return 0;
    }
    else
    {
        file.open( QIODevice::ReadWrite | QIODevice::Text );
        file.close();
        return 1;
    }
}

int FileMange::func_deleteFile(QString path)
{
    QFile file(path);
    file.remove();
}

int FileMange::func_deleteDir(QString path)
{

}

/*
** 函数功能：打开所选择的模型项
** 函数参数：所选择的模型项
** 函数返回值：无
** 函数说明：需判断是tree还是list中的模型项，需判断模型项是文件还是文件夹，需判断文件夹是不是空文件夹
*/
void FileMange::func_GenericOpen(QModelIndex current)
{
    if(!current.isValid())
        return;
    if(class_name == tr("Right_List"))
    {
        QString filepath = model->filePath(listFilterProxyModel->mapToSource(current));
        QFileInfo fileinfo(filepath);
        if(fileinfo.isDir())
        {
            list->setRootIndex(list->currentIndex());
        }
        if(fileinfo.isFile())
        {
            if(fileinfo.suffix() == "src")
                return;
            win_CodePad->show();
            win_CodePad->raise();
            win_CodePad->func_ReadFileToEdit(filepath);
            win_CodePad->lab_info[1]->setText(filepath);
            listwidget.prepend(win_CodePad);
            KukaRob.CurMName = filepath;
        }
    }
    else if(class_name == tr("Left_Tree"))
    {
        QString filepath = model->filePath(current);
        if( !tree->isExpanded(tree->currentIndex()) )
            tree->expand(tree->currentIndex());
        else
            tree->collapse(tree->currentIndex());
        if(tree->currentIndex().isValid())
        {
            QFileInfo fileinfo(filepath);
            if(fileinfo.isFile())
                return;
            QModelIndex index = listFilterProxyModel->mapFromSource(current);
            list->setRootIndex(index);
        }
        else
            return;
    }
}

void FileMange::func_reapplyFilter(int state)
{
    QRegExp regExps("^((?!\\.).)*$");
    QRegExp regExpd("^((?!src).)*$");
    if(state == 0)
        listFilterProxyModel->setFilterRegExp(regExps);
    else if(state == 1)
        listFilterProxyModel->setFilterRegExp(regExpd);
}

/*
** 函数功能：更新下方控制栏的按钮变化
** 函数参数：state；1，创建文件或文件夹时的状态；0，常态
** 函数返回值：无
** 函数说明：无
*/
void FileMange::func_BtnChange(int state)
{
    if(state == 1)
    {
        btnState = 1;
        for(int i = 0 ; i < 7 ; i++)
        {
            btnCtlFunc[i]->setIcon(QIcon(":/image/image/btn_kb.png"));
            btnCtlFunc[i]->setEnabled(false);
        }
        btnCtlFunc[5]->setEnabled(true);
        btnCtlFunc[5]->setIcon(QIcon(":/image/image/btn_ok.png"));

        btnCtlFunc[6]->setEnabled(true);
        btnCtlFunc[6]->setIcon(QIcon(":/image/image/btn_quit.png"));
    }
    else if(state == 0)
    {
        btnState = 0;
        for(int i = 0 ; i < 7 ; i++)
            btnCtlFunc[i]->setEnabled(true);
        btnCtlFunc[0]->setIcon(QIcon(":/image/image/btn_new.png"));
        btnCtlFunc[1]->setIcon(QIcon(":/image/image/btn_select.png"));
        btnCtlFunc[2]->setIcon(QIcon(":/image/image/btn_backup.png"));
        btnCtlFunc[3]->setIcon(QIcon(":/image/image/btn_save.png"));
        btnCtlFunc[4]->setIcon(QIcon(":/image/image/btn_del.png"));
        btnCtlFunc[5]->setIcon(QIcon(":/image/image/btn_open.png"));
        btnCtlFunc[6]->setIcon(QIcon(":/image/image/btn_edit.png"));
    }
    else if(state == 2)
    {
        btnState = 2;
        for(int i = 0 ; i < 7 ; i++)
            btnCtlFunc[i]->setEnabled(true);
        btnCtlFunc[5]->setEnabled(false);
        btnCtlFunc[0]->setIcon(QIcon(":/image/image/btn_new.png"));
        btnCtlFunc[1]->setIcon(QIcon(":/image/image/errlist.png"));
        btnCtlFunc[2]->setIcon(QIcon(":/image/image/btn_open.png"));
        btnCtlFunc[3]->setIcon(QIcon(":/image/image/datalist.png"));
        btnCtlFunc[4]->setIcon(QIcon(":/image/image/btn_del.png"));
        btnCtlFunc[5]->setIcon(QIcon(":/image/image/btn_kb.png"));
        btnCtlFunc[6]->setIcon(QIcon(":/image/image/btn_edit.png"));
    }
}

void FileMange::func_resize()
{
    frame_filemange->move(0,322);
    frame_filemange->resize(494,372);
    lab_b->move(0,302);
    tree->resize(300,252);
    list->resize(194,252);

    win_CodePad->frame_base->move(0,322);
    win_CodePad->frame_base->resize(494,372);
    win_CodePad->codeeditor->resize(494,262);
    win_CodePad->lab_ctlframe->move(0,292);
}

/*
** 函数功能：事件过滤器，获取tree视图和list视图的获取焦点事件，更新class_name
** 函数参数：obj，监控的对象；e，事件类型
** 函数返回值：bool
** 函数说明：tree和list的视图只能响应在其项上的单击，而不能响应在空白处的单击事件，故而需要此函数
*/
bool FileMange::eventFilter(QObject *obj, QEvent *e)
{
    if( (obj == tree) && e->type() == QEvent::FocusIn )
    {
        class_name = tr("Left_Tree");
        lab_filter->setStyleSheet("background-color:rgb(34,187,255);color:black;font-size:15px");
        lab_path->setStyleSheet("background-color:rgb(230,230,230);color:black;font-size:15px");
    }
    if( (obj == list) && e->type() == QEvent::FocusIn )
    {
        class_name = tr("Right_List");
        lab_filter->setStyleSheet("background-color:rgb(230,230,230);color:black;font-size:15px");
        lab_path->setStyleSheet("background-color:rgb(34,187,255);color:black;font-size:15px");
    }
    return QWidget::eventFilter(obj,e);
}

/*
** 函数功能：左侧treeview点击槽函数
** 函数参数：current，当前点击的index
** 函数返回值：无
** 函数说明：在treeview上点击时会触发此函数，右侧list要以current为根项显示其下所有文件
*/
void FileMange::slot_treeClicked(QModelIndex current)
{
    class_name = tr("Left_Tree");
    if( tree->isExpanded(current) )
        tree->collapse(current);
    else
        tree->expand(current);
    CurrentModel = current;
    QModelIndex index = listFilterProxyModel->mapFromSource(current);
    list->setRootIndex(index);
    lab_filter->setStyleSheet("background-color:rgb(34,187,255);color:black;font-size:15px");
    lab_path->setStyleSheet("background-color:rgb(230,230,230);color:black;font-size:15px");
    lab_path->setText("...的内容："+model->fileName(current) );
    /*更新lab_count显示当前目录下对象个数*/
    QFileInfo info( model->filePath(current) );
    if(info.isDir())
    {
        QDir dir( model->filePath(current) );
        int count = dir.count()-2;
        lab_count->setText("  "+QString::number(count)+" 对象");
    }
}

/*
** 函数功能：右侧list点击槽函数
** 函数参数：current，当前点击的index
** 函数返回值：无
** 函数说明：此函数需要获取到当前点击index的index，路径，显示的字符
*/
void FileMange::slot_listClicked(QModelIndex current)
{
    int size,erres;
    QString filepath;
    class_name = tr("Right_List");
    CurrentModel=current;
    filepath=model->filePath(listFilterProxyModel->mapToSource(current));

    lab_path->setStyleSheet("background-color:rgb(34,187,255);color:black;font-size:15px");
    lab_filter->setStyleSheet("background-color:rgb(230,230,230);color:black;font-size:15px");

    QFileInfo info( filepath );
    if(info.isFile())
        size = info.size();
    else
        size = 0;
    lab_count->setText("  1 对象已被标记                      "+QString::number(size)+" 字节");
    if(ParserRes.contains(filepath))
    {
        erres = ParserRes.value(filepath);
        if(erres == -1)
            func_BtnChange(2);
        else
            func_BtnChange(0);
    }
    else
        func_BtnChange(0);
}

/*
** 函数功能：右侧list双击槽函数
** 函数参数：current，当前点击的index
** 函数返回值：无
** 函数说明：此函数需要获取到当前点击index的index，路径，显示的字符，并且打开双击的文件
*/
void FileMange::slot_listDbClicked(QModelIndex current)
{
    class_name = tr("Right_List");
    CurrentModel=current;
    func_GenericOpen(current);
    lab_path->setText("...的内容："+model->fileName(current) );
}

/*
** 函数功能：按钮点击槽函数
** 函数参数：无
** 函数返回值：无
** 函数说明：无
*/
void FileMange::slot_BtnCtlClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if(!btn)
        return;
    if(btn == btnCtlFunc[0])    //新
    {
        func_new();
    }
    else if(btn == btnCtlFunc[1])/* xuanding */
    {
        if(btnState == 2)   //错误列表
        {
            QString path = model->filePath(listFilterProxyModel->mapToSource(CurrentModel));
            win_CodePad->funcParser->startParser(path);
            func_showErrList();
        }
        else if(btnState == 0)   //选定
        {
            func_GenericOpen(CurrentModel);
            win_CodePad->func_editFileToSrcFile();

            QString SrcPath = KukaRob.CurMName;
            QString DatPath = KukaRob.CurMName;
            SrcPath.append(".src");
            DatPath.append(".dat");

            char Src[50];
            char Dat[50];

            QByteArray BitPathArry1 = SrcPath.toLatin1();
            strcpy(Src, BitPathArry1.data());

            QByteArray BitPathArry2 = DatPath.toLatin1();
            strcpy(Dat, BitPathArry2.data());

            /* decode */
            GetMdlEnter(&xVM, Dat, Src);
            memcpy(&xMidTask, &xVM, sizeof(vm));
        }
    }
    else if(btn == btnCtlFunc[2])
    {
        if(btnState == 2)  //打开
            func_GenericOpen(CurrentModel);
        else if(btnState == 0)  //备份
        {

        }
    }
    else if(btn == btnCtlFunc[3])
    {

    }
    else if(btn == btnCtlFunc[4])   //删除
    {
        if(btnState == 1)
            return;
        if(class_name != "Right_List")
            return;
        /*删除程序文件，需要同时删除三个文件提供编辑器显示的文件、src文件、dat文件。*/
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString str = time.toString("hh:mm:ss yyyy/MM/dd"); //设置显示格式
        QString str1 = model->filePath(listFilterProxyModel->mapToSource(list->currentIndex()));
        QFileInfo tempfile(str1);
        QString basename = tempfile.baseName();
        QString path = tempfile.absolutePath();
        QString codepath = path+"/"+basename;
        QString srcpath = codepath+".src";
        QString datpath = codepath+".dat";
        int res = QMessageBox::question(this,str+"导航器","您确定要将所选文件删除吗？","是","否");
        if(res == 0 )   //同时删除源文件和数据文件
        {
            func_deleteFile(codepath);
            func_deleteFile(srcpath);
            func_deleteFile(datpath);
        }
    }
    else if(btn == btnCtlFunc[5] && btnCtlFunc[0]->isEnabled())   //打开编辑
    {
        func_GenericOpen(CurrentModel);
    }
    else if( btn == btnCtlFunc[5] && !btnCtlFunc[0]->isEnabled() )    //OK
    {
        /*新建程序文件，程序文件有三部分组成，src文件，记录程序文件的所有内容，
         * dat文件记录指令产生的数据声明，无后缀名文件仅仅用来显示编辑器中的内容
        */
        if(tab_module == NULL)
            return;
        if(tab_module->currentRow() != 4)
            return;
        frame_modular->hide();

        QInputDialog *inpdog = new QInputDialog(this,Qt::Dialog);
        inpdog->setFixedSize(QSize(200,200));
        inpdog->setInputMode(QInputDialog::TextInput);
        inpdog->setWindowIcon(QIcon(":/image/image/robot.png"));
        inpdog->setLabelText("输入新建程序名称");
        int res = inpdog->exec();
        if(res == QDialog::Accepted)
        {
            QString listcurpath = model->filePath(listFilterProxyModel->mapToSource(list->currentIndex()));
            QFileInfo tempfile(listcurpath);
            QString parentpath;
            if(tempfile.isFile())
                parentpath = tempfile.absolutePath();
            else
                parentpath = tempfile.absoluteFilePath();
            QString inputstr = inpdog->textValue();
            if( func_creatModule(parentpath,inputstr) )
            {
                win_CodePad->init_ModuleFile(parentpath+"/"+inputstr);
                QString datpath = parentpath+"/"+inputstr+".dat";
                QFile file(datpath);
                file.open( QIODevice::ReadWrite | QIODevice::Text );
                file.close();

                QString srcpath = parentpath+"/"+inputstr+".src";
                QFile filesrc(srcpath);
                filesrc.open( QIODevice::ReadWrite | QIODevice::Text );
                filesrc.close();
            }
        }
        func_BtnChange(0);
    }
    else if(btn == btnCtlFunc[6] && btnCtlFunc[0]->isEnabled())  //编辑
    {
        func_BuildEditFrame();
    }
    else if(btn == btnCtlFunc[6] && !btnCtlFunc[0]->isEnabled())  //退出
    {
        if(frame_modular != NULL)
            frame_modular->hide();
        func_BtnChange(0);
    }
}

void FileMange::setEditColor()
{
    for(int i=0;i<16;i++)
    {
        btnEdit0[i]->setPalette(old_pa);
        btnEdit0[i]->setStyleSheet(old_stysheet);
    }
    for(int i=0;i<3;i++)
    {
        btnEdit1[i]->setPalette(old_pa);
        btnEdit1[i]->setStyleSheet(old_stysheet);
    }

    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    for(int i=0;i<16;i++)
    {
        if(btnEdit0[i] == sender())
        {
            btnEdit0[i]->setStyleSheet(new_stylesheet);
            btnEdit0[i]->setPalette(new_pa);
            break;
        }
    }
}


void FileMange::slot_BtnEdit()
{
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    if(btn == btnEdit0[0])
    {

    }
    else if(btn == btnEdit0[1])   //打开
    {
        frame_edit->resize(330,652);
        frame_edit->move(164,0);
        frameBtnEdit[1]->show();
        for(int i=0;i<3;i++)
            btnEdit1[i]->show();
        btnEdit1[0]->setText(tr("文件/目录"));
        btnEdit1[1]->setText(tr("数据列表"));
        btnEdit1[2]->setText(tr("故障列表"));
    }
    else if(btn == btnEdit0[2])
    {

    }
    else if(btn == btnEdit0[3])
    {

    }
    else if(btn == btnEdit0[4])
    {

    }
    else if(btn == btnEdit0[5])
    {

    }
    else if(btn == btnEdit0[6])   //删除
    {
        if(class_name != "Right_List")
            return;
        /*删除程序文件，需要同时删除三个文件提供编辑器显示的文件、src文件、dat文件。*/
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString str = time.toString("hh:mm:ss yyyy/MM/dd"); //设置显示格式
        QString str1 = model->filePath(listFilterProxyModel->mapToSource(list->currentIndex()));
        QFileInfo tempfile(str1);
        QString basename = tempfile.baseName();
        QString path = tempfile.absolutePath();
        QString codepath = path+"/"+basename;
        QString srcpath = codepath+".src";
        QString datpath = codepath+".dat";
        int res = QMessageBox::question(this,str+"导航器","您确定要将所选文件删除吗？","是","否");
        if(res == 0 )   //同时删除源文件和数据文件
        {
            func_deleteFile(codepath);
            func_deleteFile(srcpath);
            func_deleteFile(datpath);
        }
    }
    else if(btn == btnEdit0[7])
    {

    }
    else if(btn == btnEdit0[8])   //存档
    {
        frame_edit->resize(330,652);
        frame_edit->move(164,0);
        frameBtnEdit[1]->show();
        for(int i=0;i<3;i++)
            btnEdit1[i]->show();
        btnEdit1[0]->setText(tr("USB（KCP）"));
        btnEdit1[1]->setText(tr("USB（控制柜）"));
        btnEdit1[2]->setText(tr("网络"));
    }
    else if(btn == btnEdit0[9])
    {

    }
    else if(btn == btnEdit0[10])
    {

    }
    else if(btn == btnEdit0[11])
    {

    }
    else if(btn == btnEdit0[12])
    {

    }
    else if(btn == btnEdit0[13])   //选择
    {
        frame_edit->resize(330,652);
        frame_edit->move(164,0);
        frameBtnEdit[1]->show();
        for(int i=0;i<2;i++)
            btnEdit1[i]->show();
        btnEdit1[2]->hide();
        btnEdit1[0]->setText(tr("无参数"));
        btnEdit1[1]->setText(tr("带参数"));
    }
    else if(btn == btnEdit0[14])
    {

    }
    else if(btn == btnEdit0[15])
    {

    }
    else if(btn == btnEdit1[0])
    {

    }
    else if(btn == btnEdit1[1])
    {

    }
    else if(btn == btnEdit1[2])
    {

    }
}

void FileMange::slot_handeleUserChanged(int n)
{
    switch (n) {
    case Proger:
        func_reapplyFilter(1);
        break;
    case Expert:
        func_reapplyFilter(1);
        break;
    case Recv:
        func_reapplyFilter(1);
        break;
    case Maint:
        func_reapplyFilter(1);
        break;
    case Admin:
        func_reapplyFilter(1);
        break;
    case Operator:
        func_reapplyFilter(0);
        break;
    default:
        break;
    }
}

void FileMange::slot_restoreFrame()
{
    frame_filemange->move(0,0);
    frame_filemange->resize(494,694);
    lab_b->move(0,624);
    tree->resize(300,574);
    list->resize(194,574);

    win_CodePad->frame_base->move(0,0);
    win_CodePad->frame_base->resize(494,694);
    win_CodePad->codeeditor->resize(494,584);
    win_CodePad->lab_ctlframe->move(0,614);
}

void FileMange::slot_showDetailErr(int line)
{
    func_GenericOpen(CurrentModel);
    win_CodePad->codeeditor->func_ArrowGoLine(line-1);
    win_GramErr->raise();
}

FileMange::~FileMange()
{

}
