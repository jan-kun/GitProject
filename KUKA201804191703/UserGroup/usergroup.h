#ifndef USERGROUP_H
#define USERGROUP_H

#include <QWidget>
#include"Conditional.h"
#include"data.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

class UserGroup : public QWidget
{
    Q_OBJECT
public:
    explicit UserGroup(QWidget *parent = 0);
    QFrame *frame_log = NULL;
    QLabel *lab_user;  //显示当前登录用户
    QLabel *lab_time;  //显示登录时间
    QPushButton *btn_loginctl[7];

    QFrame *frame_user = NULL;
    QTableWidget *tab_user;
    QLabel *lab_pass;
    QLineEdit *line_pass;
    QPushButton *btn_userctl[7];
    void init_frame();
    void frame_login();
    int func_userlogin(QString user , QString passcode);

signals:
    void sig_mimaerr();
    void sig_userlogin(int n);
public slots:
    void slot_btnClicked();
    void slot_tabClicked();

};

#endif // USERGROUP_H
