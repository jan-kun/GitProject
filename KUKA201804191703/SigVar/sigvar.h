#ifndef SIGVAR_H
#define SIGVAR_H

#include <QWidget>
#include"Conditional.h"
#include"data.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif
class SigVar : public QWidget
{
    Q_OBJECT
public:
    explicit SigVar(QWidget *parent = 0);
    QFrame* frame_base = NULL;
    QComboBox *cb_var[3];
    QLabel *lab_show;
    QPushButton *btn_var[6];

    void init_frame();
signals:

public slots:
    void slot_btnClicked();

};

#endif // SIGVAR_H
