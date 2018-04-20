#ifndef BASE_IN_H
#define BASE_IN_H

#include <QWidget>
#include"Conditional.h"
#include"data.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

class Base_IN : public QWidget
{
    Q_OBJECT
public:
    explicit Base_IN(QWidget *parent = 0);
    QFrame* frame_base = NULL;
    QPushButton* btn_func[7];
    QDoubleValidator *validator;
    int currentPage;
    int BaseNum;
    QString BaseName;
    double basedata[6];
    double res[6];
    
    QFrame* frame_child[3] = {NULL,NULL,NULL};
    QComboBox *cb_base;
    QLineEdit *line_name;
    QLabel *lab_base1[6];
    
    QLabel *lab_base2[2];
    QLineEdit* line_data[6];
    
    QLabel *lab_base[2];
    QLabel *lab_base3[6];
    
    void frame_SelectBase();
    void frame_BaseData();
    void frame_Res();
    
    void func_continue();
    void func_back();
    void func_save();
    void func_CreatBaseDataDecl(CodeData data);

signals:
    void Sig_NewBase(CodeData *base);
public slots:
    void slot_BtnClicked();
    void slot_CbClicked(int index);

};

#endif // BASE_IN_H
