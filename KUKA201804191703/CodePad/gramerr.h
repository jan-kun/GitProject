#ifndef GRAMERR_H
#define GRAMERR_H

#include <QWidget>
#include<QtGui>
#include<data.h>

class GramErr : public QWidget
{
    Q_OBJECT
public:
    explicit GramErr(QWidget *parent = 0);

    QFrame *frame_base = NULL;
    QLabel *lab_title;
    QPushButton *btn_errFunc[6];
    QTableWidget *tab_err;
    QLabel *lab_detail;
    void func_buildFrame();
    void func_updateGramErr();
    void func_setTitle(QString name);
    void hideEvent(QHideEvent *);
signals:
    void aboutToHide();
    void sig_showDetail(int line);
public slots:
    void slot_btnClicked();
    void slot_tabClicked(int r,int c);

};

#endif // GRAMERR_H
