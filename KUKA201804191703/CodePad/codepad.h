#ifndef CODEPAD_H
#define CODEPAD_H

#include <QWidget>
#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif
#include "codeeditor.h"
#include"cmd_line.h"
#include "data.h"
#include"MyWidget/menuwidget.h"
#include"codedatafunc.h"
#include"Parser/parser.h"

class CodePad : public QWidget
{
    Q_OBJECT

public:
    CodePad(QWidget *parent = 0);
    ~CodePad();
    CodeEditor *codeeditor;
    Cmd_Line *cmdline = NULL;
    CodeDataFunc *codefunc = NULL;
    Parser *funcParser = NULL;
    QFrame *frame_base = NULL;
    QLabel *lab_ctlframe;
    QWidget *widget;
    QLabel *lab_tip;
    QLabel *lab_info[4];
    QPushButton *btnCtlFunc[7];
    QString TAB = "        ";
    /*指令菜单控件*/
    MenuWidget *frame_cmd = NULL;
    QFrame *frameBtn[3];
    QPushButton *btnFrame0[6];
    QPushButton *btnFrame1[7];
    QPushButton *btnFrame2[4];
    QLabel *lab0[6];
    QLabel *lab1;

    MenuWidget *frame_edit = NULL;
    QFrame *frameBtnEdit[2];
    QLabel *labedit[3];
    QPushButton *btn_edit0[14];
    QPushButton *btn_edit1[6];

    QString filepath;
    QPalette old_pa , new_pa;
    QString old_stysheet , new_stylesheet;
    QString CurModule;  //当前打开的模块文件

    int flag_nc;  //新建指令时为0，更改指令时为1

    void init_mainframe();
    void func_ChangeCMD(QTextCursor temptc);
    void func_BuildCmdFrame();
    void func_showCmdFrame();
    void func_hideCmdFrame();
    void func_BuildEditFrame();
    void func_showEditFrame();
    void func_hideEditFrame();
    void func_SelectRow();  //语句行选择，箭头光标移至所选择的行
    void init_arrow();
    void init_editarrow();
    void setArrow(QLabel *lab , QPushButton *btn);
    void Btn_Change(int state);  //-2：未插入指令状态，0-4：PTP、LIN、CIRC、SLIN 、SCIRC状态

    int func_changeData(CodeData olddata,CodeData newdata);
    void func_BuildCMDData(int type);
    void func_PTP();
    void func_LIN();
    void func_CIRC();
    void func_SLIN();
    void func_SCIRC();
    QStringList func_GetPointInfo(QString *svalue);
    QStringList func_GetFDATInfo(QString *svalue);
    QStringList func_GetPDATInfo(QString *svalue);
    QStringList func_GetLDATInfo(QString *svalue);
    void func_CMDChange(QTextCursor temptc);   //更改指令
    int func_JudgeData(QString name);   //判断name是否为一个数据变量的名称，是的话返回数据类型，不是返回-1
    int func_JudgeLab(QString name);
    void init_ModuleFile(QString path);
    int func_ReadFileToEdit(QString path);

    QString func_strInsertDataDecl(CodeData data);
    QString func_calcDataDecl(CodeData data);
    void func_NewDataDecl(CodeData data);
    void func_ConfirmCmdData();
    QString func_CalcCmdDecl(int cmd, QString auxpoint, QString point, QString pldat, QString fdat);
    int func_extractCmdData(QString cmdline,QString* cmdtype,QString* auxpoint,QString* point , QString* pldata,QString* fdat);
    void func_editFileToSrcFile();
    QString func_editStrToSrcStr(QString strline);

    void func_ChangeCmdData();
    void func_ConfirmPara();
    void func_DelRow(QTextCursor temptc);
    void func_restoreFrame();

protected:
    void mousePressEvent(QMouseEvent *e);

signals:
    void sig_hasNewData(CodeData data);
    void sig_deleteData(CodeData data);
    void sig_changeData(CodeData olddata,CodeData newdata);
public slots:
    void slot_BtnCtlFunc(bool state);
    void slot_BtnFrame0();
    void slot_BtnFrame1();
    void slot_BtnFrame2();
    void slot_EditBtn();
    void setColor0();
    void setColor1();
    void setEditColor();
    void func_codedelselect();
    void slot_resizeFrame();
    void slot_changeBtn(int i);
    void slot_aboutToHide();
    void slot_aboutToShow(MenuWidget *wid);
    void slot_funcNewData(CodeData data);
    void slot_funcDelData(CodeData data);
};

#endif // CODEPAD_H
