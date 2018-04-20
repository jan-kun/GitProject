#ifndef TCP_IN_H
#define TCP_IN_H

#include <QWidget>
#include"Conditional.h"
#include"data.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

class TCP_IN : public QWidget
{
    Q_OBJECT
public:
    explicit TCP_IN(QWidget *parent = 0);

    QFrame *frame_base = NULL;
    QPushButton *btn_func[7];
    QDoubleValidator *validator;
    int currentPage;
    int TcpNum;
    QString TcpName;
    double tool_data[6];
    double load_data[10];
    /*第一页选择工具号所需控件*/
    QFrame *frame_child[4]={NULL,NULL,NULL,NULL};
    QLineEdit *line_name;
    QComboBox *cb_tool;
    QLabel *lab_data1[6];
    /*第二页输入工具坐标信息所需控件*/
    QLabel *lab_data2[2];
    QLineEdit* line_tool[6];
    /*第三页输入工具坐标负载信息所需控件*/
    QLabel *lab_data3[2];
    QLineEdit* line_load[10];
    /*第四页展示输入的工具坐标信息所需控件*/
    QLabel *lab_tool[2];
    QLabel *lab_data4[6];

    void frame_SelectTool();
    void frame_ToolData();
    void frame_Load();
    void frame_Res();
    void func_back();
    void func_continue();
    void func_save();
    void func_CreatToolDataDecl(CodeData data);

signals:
    void Sig_NewTool(CodeData* data );
public slots:
    void slot_BtnClicked();
    void slot_CbClicked(int index);

};

#endif // TCP_IN_H
