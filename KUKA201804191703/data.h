#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include<QPlainTextEdit>
#include<QFileInfoList>
#include"Protocol/robot.h"
#include "Protocol/RobConvert.h"

#define CNT_Type 26
#define CNT_Cmd 5
#define Proger 0x00
#define Expert 0x01
#define Recv 0x02
#define Maint 0x03
#define Admin 0x04
#define Operator 0x05
#define RadTDege 57.2957795131
#define Cor_Jog 0x00
#define Cor_Global 0x01
#define Cor_Base 0x02
#define Cor_Tool 0x03
#define Inc_Con 0x00
#define Inc_100 0x01
#define Inc_10 0x02
#define Inc_1 0x03
#define Inc_p1 0x04
#define GERRLENGTH 100
typedef enum{
    Manual1,  //T1
    Manual2,  //T2
    AUTO,  //自动
    FullMan,  //全速手动
    Exter   //外部
}RModel;  //机器人运行模式

typedef enum{
    M_Go,   //Go
    M_Act,  //动作
    M_Sig   //单个步骤
}CModel;   //程序运行模式

typedef enum{
    Variable,  //变量
    CONST,  //常量
}Stortype;  //存储类型

typedef struct{
    bool value;
    int setboolvalue(QStringList list)
    {
        if(list.count() != 1 )
            return -1;
        else
        {
            if(QString::compare(list.at(0) , "true" , Qt::CaseInsensitive) == 0)
                value = true;
            else
                value = false;
            return 0;
        }
    }
}BOOL;

typedef struct{
    int value;
    int setintval(QStringList list)
    {
        bool ok;
        if(list.count() != 1 )
            return -1;
        else
        {
            value = list.at(0).toInt(&ok,10);
            if(ok)
                return 0;
            else
                return -1;
        }
    }
}INT;

typedef struct{
    double value;
    int setrealval(QStringList list)
    {
        bool ok;
        if(list.count() != 1 )
            return -1;
        else
        {
            value = list.at(0).toDouble(&ok);
            if(ok)
                return 0;
            else
                return -1;
        }
    }
}REAL;

typedef struct{
    char value;
    int setcharval(QStringList list)
    {
        if(list.count() != 1 )
            return -1;
        if(list.at(0).count() != 1)
            return -1;
        else
        {
            value = list.at(0).at(0).toLatin1();
            return 0;
        }
    }
}CHAR;

typedef struct{
    double axis[6];  //a1a2a3a4a5a6
    int setaxisval(QStringList list)
    {
        bool ok;
        if(list.count() != 6 )
            return -1;
        for(int i = 0 ; i < 6 ; i++)
        {
            axis[i] = list.at(i).toDouble(&ok);
            if( !ok )
                return -1;
        }
        return  0;
    }
}AXIS;

typedef struct{
    double eaxis[12];  //a1-a6 , e1-e6
    int sete6axisval(QStringList list)
    {
        bool ok;
        if(list.count() != 12 )
            return -1;
        for(int i = 0 ; i < 12 ; i++)
        {
            eaxis[i] = list.at(i).toDouble(&ok);
            if( !ok )
                return -1;
        }
        return 0;
    }
}E6AXIS;

typedef struct{
    double frameval[6];  //xyzabc
    int setframeval(QStringList list)
    {
        bool ok;
        if(list.count() != 6 )
            return -1;
        for(int i = 0 ; i < 6 ; i++)
        {
            frameval[i] = list.at(i).toDouble(&ok);
            if( !ok )                                  return -1;
        }
        return 0;
    }
}FRAME;

typedef struct{
    double posval[6];  //xyzabc
    int S , T;
    int setposval(QStringList list)
    {
        bool ok;
        if(list.count() != 8 )
            return -1;
        for( int i = 0 ; i < 6 ; i++)
        {
            posval[i] = list.at(i).toDouble(&ok);
            if( !ok )                              return -1;
        }
        S = list.at(6).toInt(&ok);
        T = list.at(7).toInt(&ok);
        return 0;
    }
}POS;

typedef struct{
    double e6posval[12]; //xyzabc , e1-e6
    int S , T;
    int sete6posval(QStringList list)
    {
        bool ok;
        if(list.count() != 14 )
            return -1;
        for( int i = 0 ; i < 12 ; i++)
        {
            e6posval[i] = list.at(i).toDouble(&ok);
            if( !ok )                                  return -1;
        }
        S = list.at(12).toInt(&ok);
        T = list.at(13).toInt(&ok);
        return 0;
    }
}E6POS;

typedef struct{
    int TOOL_NO;   //工具坐标号
    int BASE_NO;   //基座标号
    int IPO_FRAME;    //外部TCP为TRUE时为#TCP 0，为FALSE时为#BASE 1
    char* POINT2;   //辅助点?
    bool TQ_STATE;   //碰撞识别
    int setfdatval(QStringList list)
    {
        bool ok;
        if(list.count() != 5 )
            return -1;
        TOOL_NO = list.at(0).toInt(&ok,10);
        BASE_NO = list.at(1).toInt(&ok,10);
        IPO_FRAME = list.at(2).toInt(&ok,10);
        POINT2 = list.at(3).toLocal8Bit().data();
        if(QString::compare(list.at(4) , "true" , Qt::CaseInsensitive) == 0)
            TQ_STATE = true;
        else
            TQ_STATE = false;
        return 0;
    }
}FDAT;

typedef struct{
    double VEL;  //运动速度
    double ACC;  //轨迹加速
    double APO_DIST;  //圆滑过渡距离
    int APO_MODE;  //出现CONST时，才会出现这个变量 没有CONST 0 有CONST 1
    int setpdatval(QStringList list)
    {
        bool ok;
        if(list.count() != 4 )
            return -1;
        VEL = list.at(0).toDouble(&ok);
        ACC = list.at(1).toDouble(&ok);
        APO_DIST = list.at(2).toDouble(&ok);
        APO_MODE = list.at(3).toInt(&ok,10);
        return 0;
    }
}PDAT;

typedef struct{
    double VEL;   //运动速度
    double ACC;  //轨迹加速
    double APO_DIST;  //圆滑过渡距离
    double APO_FAC;
    double AXIS_VEL;  //轴速
    double AXIS_ACC;   //轴加速
    int ORI_TYP;    //方向导引  0：标准，1：手动PTP，2：恒定的方向
    int CIRC_TYP;   //圆周上的方向导引
    double JERK_FAC;
    double GEAR_JERK;   //传动装置
    double EXAX_IGN;
    int setldatval(QStringList list)
    {
        bool ok;
        if(list.count() != 11 )
            return -1;
        VEL = list.at(0).toDouble(&ok);
        ACC = list.at(1).toDouble(&ok);
        APO_DIST = list.at(2).toDouble(&ok);
        APO_FAC = list.at(3).toDouble(&ok);
        AXIS_VEL = list.at(4).toDouble(&ok);
        AXIS_ACC = list.at(5).toDouble(&ok);
        ORI_TYP = list.at(6).toInt(&ok,10);
        CIRC_TYP = list.at(7).toInt(&ok,10);
        JERK_FAC = list.at(8).toDouble(&ok);
        GEAR_JERK = list.at(9).toDouble(&ok);
        EXAX_IGN = list.at(10).toDouble(&ok);
        return 0;
    }
}LDAT;

typedef struct{
    double baseval[6];  //xyzabc
    int setbasedatval(QStringList list)
    {
        bool ok;
        if(list.count() != 6)
            return -1;
        for(int i = 0 ; i < 6 ; i++)
        {
            baseval[i] = list.at(i).toDouble(&ok);
            if( !ok )                                return -1;
        }
        return 0;
    }
}BASEDAT;

typedef struct{
    double loadval[10];  //质量、重心(xyzabc)、惯性矩(xyz)
    int setloaddatval(QStringList list)
    {
        bool ok;
        if(list.count() != 10)
            return -1;
        for(int i = 0 ; i < 10 ; i++)
        {
            loadval[i] = list.at(i).toDouble(&ok);
            if( !ok )                               return -1;
        }
        return  0;
    }
}LOADDAT;


typedef struct{
    double toolval[6];  //xyzabc
    LOADDAT Load;
    double ctoolval[7];
    double err[3];
    int settooldatval(QStringList list)
    {
        bool ok;
        if(list.count() != 16)
            return -1;
        for(int i = 0 ; i < 6 ; i++)
        {
            toolval[i] = list.at(i).toDouble(&ok);
            if( !ok )                               return -1;
        }
        QStringList loadlist;
        loadlist.clear();
        for(int i = 6 ; i < 16 ; i++)
            loadlist.append( list.at(i) );
        return Load.setloaddatval(loadlist);
    }
}TOOLDAT;

typedef enum{
    booltype,
    inttype,
    realtype,
    chartype,
    axistype,
    e6axistype,
    frametype,
    postype,
    e6postype,
    fdattype,
    pdattype,
    ldattype,
    basedattype,
    loaddattype,
    tooldatatype,
}CodeDataType;  //数据类型

typedef union
{
    BOOL boolVal;
    INT intVal;
    REAL realVal;
    CHAR charVal;
    AXIS axisVal;
    E6AXIS e6axisVal;
    FRAME frameVal;
    POS posVal;
    E6POS e6posVal;
    FDAT fdatVal;
    PDAT pdatVal;
    LDAT ldatVal;
    BASEDAT basedatVal;
    LOADDAT loaddatVal;
    TOOLDAT tooldatVal;
}Value;

typedef struct{
    QString name;
    Value val;
    QString Svalue;  //将值转化为QString用来显示
    CodeDataType dtype;
    Stortype stotype;
    QString module;   //数据所属的模块，绝对路径，但是不包含后缀名
    QString code;
    int dimension;
}CodeData;

typedef struct{
    QString name;  //程序名称
    QString srcpath;
    QString datpath;
}ModuleI;  //程序包含的信息

typedef struct{
    int line;
    QString detail;
}GrameErr;

typedef struct{
    RModel Model;
    CModel CodeMod;
    int EMG;  //为1表示释放急停，为0表示急停按下
    int PEMG;
    int Motor;   //使能状态
    int PMotor;   //上一个使能状态
    int alarm[4];  //4类报警信息
    int PVel;
    int MVel;
    int Spd_hand;
    int Spd_code;
    int Incre;   //增量  0:
    int Cor_Mouse;  //鼠标状态下选择的坐标系 0：关节运动 1：全局坐标系 2：基座标系 3：工具坐标系
    int Cor_Key;    //按键状态下选择的坐标系
    int User;  //当前用户
    CodeData CurTool;
    CodeData CurBase;
    int indexb;   //当前工具号
    int indext;  //当前基坐标号
    int ipo;  //IPO
    int RobMod;
    int PRobMod;
    QString CurMName;
}SysRob;

extern int fd;
extern SysRob KukaRob;
extern double Fpos[7];   //存储xyzq1q2q3q4的数据
extern QMap<QString,int> TypeToIndex;   //将数据类型映射成整数，从0到CNT_Type
extern QMap<QString,int> CMDToIndex;   //将指令类型映射成整数，从0到CNT_Cmd
extern QMap<int,int> EditToSrc;
extern QMap<QString,int> ParserRes;
extern QList<ModuleI> ModuleInfo;
extern QList<CodeData> codedata[CNT_Type];
extern GrameErr gErrTab[GERRLENGTH];
extern int pErr;

extern QFileInfoList dirInfoList;
extern QFileInfoList DataFileList;  //保存目录下所有的数据文件信息
extern QFileInfoList ParserFileList;   //保存目录下的所有待译码的文件
extern void func_initmap();
extern void func_ReadModuleFromPath(QString path);
extern void func_ReadParserFileFromPath(QString path);
extern int func_checkModName(QString name);
extern int func_ReadCodeDataFromFile(QString path);
extern int func_ReadRobSysDataFromFile(QString path);
//extern void func_initCodedata();
extern void func_initSysData();
extern QString func_BuildDataName(QString type);   //构建一个变量名
extern int func_JudgeDataExist(QString name);
extern int func_getDataIndex(QString name,QString type);
extern CodeData func_getDataByName(QString name,bool *ok);
extern int func_srcLineToEdit(int srcline);
extern int func_CursorGoStr(QPlainTextEdit *edit, QString str);
extern void func_SaveToFile(QPlainTextEdit*edit, QString path);
extern void func_getE6Pos(double *val);
extern void GetJntRec(double *pAix,SysRob  *pASys, sys *pSys,int iCoor);
#endif // DATA_H
