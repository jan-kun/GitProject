#ifndef FILEMANGE_H
#define FILEMANGE_H

#include <QWidget>
#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif
#include "mymodel.h"
#include"CodePad/codepad.h"
#include"CodePad/gramerr.h"
#include"MyWidget/menuwidget.h"
#include "listfiltermodel.h"

class FileMange : public QWidget
{
    Q_OBJECT

public:
    FileMange(QWidget *parent = 0);
    ~FileMange();
    QWidget *widget;
    QFrame *frame_filemange;
    CodePad *win_CodePad = NULL;
    GramErr *win_GramErr = NULL;
    QLabel *lab_filter;   //左侧tree上方显示过滤器的标签
    QLabel *lab_path;  //右侧list上方的小标签显示list的根目录
    MyModel *model;
    ListFilterModel *listFilterProxyModel;
    QTreeView *tree;
    QListView *list;
    QLabel *lab_count;  //下方按钮上的小标签，显示有几个对象
    QLabel *lab_b;
    QPushButton *btnCtlFunc[7];
    /*新建模块左侧需要的控件*/
    QFrame *frame_modular = NULL;
    QTableWidget *tab_module = NULL;

    MenuWidget *frame_edit = NULL;
    QFrame *frameBtnEdit[2];
    QPushButton *btnEdit0[16];
    QPushButton *btnEdit1[3];

    QString class_name;    //记录tree和list具体是哪个获得了焦点，每次变化需要更新
    QModelIndex CurrentModel;    //记录tree和list当前选中的项，每次改变都需要更新
    QPalette old_pa , new_pa;
    QString old_stysheet , new_stylesheet;
    int btnState = 0;   //0，常态 ; 1，创建文件、文件夹；2，有语法错误

    void init_mainframe();
    bool func_foldIsEmpty(QString filepath);
    void func_GenericOpen(QModelIndex current);
    void func_reapplyFilter(int state);
    void func_BuildEditFrame();
    void func_FrameModule();   //新建模块左侧界面
    void func_showErrList();
    void func_new();
    int func_creatModule(QString absoultpath, QString name);
    int func_deleteFile(QString path);
    int func_deleteDir(QString path);
    void func_BtnChange(int state);
    void func_resize();
    bool eventFilter(QObject *obj, QEvent *e);

public slots:
    void slot_treeClicked(QModelIndex current);
    void slot_listClicked(QModelIndex current);
    void slot_listDbClicked(QModelIndex current);

    void slot_BtnCtlClicked();
    void setEditColor();
    void slot_BtnEdit();
    void slot_handeleUserChanged(int n);
    void slot_restoreFrame();
    void slot_showDetailErr(int line);
};

#endif // FILEMANGE_H
