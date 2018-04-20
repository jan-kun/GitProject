#ifndef MY_INPUTDIALOG_H
#define MY_INPUTDIALOG_H

#include <QWidget>
#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

class My_InputDialog : public QWidget
{
    Q_OBJECT
public:
    explicit My_InputDialog(QWidget *parent = 0);

    QLabel *lab_name;
    QLabel *lab_tip;
    QLineEdit *line_input;
    QPushButton *btn_cancel,*btn_ok;
    QGridLayout *layout_dialog;
    QString temp_name,final_name;

    void Init_Interface();
    void set_text(QString title, QString label, QString linetext);
    QString show_Dialog();

public slots:
    void slot_btn_ok();
    void slot_btn_cancel();
};

#endif // MY_INPUTDIALOG_H
