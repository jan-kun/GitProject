#ifndef CONDITIONAL_H
#define CONDITIONAL_H

#define LINUX_ARM 1
#define ARM_PC 1  //0,PC；1，ARM板

#if ARM_PC
#define KUKA_PATH "/KUKA"
#define KUKA_MAIZI "/KUKA/MAIZI-PC"
#define KUKA_ARCHIVE "/KUKA/(ARCHIVE:\\)"
#define KUKA_R1 "/KUKA/MAIZI-PC/R1"
#define KUKA_Pro "/KUKA/MAIZI-PC/R1/Program"
#define KUKA_SYS "/KUKA_SYS"
#define KUKA_USB "/KUKA_SYS/usb"
#define KUKA_PARA "/KUKA_SYS/Para"
#define xSysFile "/KUKA_SYS/Para/xsys.txt"
#define RobSysDataFile "/KUKA_SYS/Para/robsys.dat"
#else
#define KUKA_PATH "/home/jan/temp/KUKA"
#define KUKA_MAIZI "/home/jan/temp/KUKA/MAIZI-PC"
#define KUKA_ARCHIVE "/home/jan/temp/KUKA/(ARCHIVE:\\)"
#define KUKA_R1 "/home/jan/temp/KUKA/MAIZI-PC/R1"
#define KUKA_Pro "/home/jan/temp/KUKA/MAIZI-PC/R1/Program"
#define KUKA_SYS "/home/jan/temp/KUKA_SYS"
#define KUKA_USB "/home/jan/temp/KUKA_SYS/usb"
#define KUKA_PARA "/home/jan/temp/KUKA_SYS/Para"
#define xSysFile "/home/jan/temp/KUKA_SYS/Para/xsys.txt"
#define RobSysDataFile "/home/jan/temp/KUKA_SYS/Para/robsys.dat"
#endif

#include<QString>
#include<QList>
#include<QWidget>
#include"Protocol/bus.h"
#include"Protocol/robot.h"
#include"QBitArray"
#include<QRegExp>

class QComboBox;
static bool ENABLE;    //使能状态
static enum enum_key_model //按键工作模式
{
    ANIX_K,
    GLOBAL_K,
    BASE_K,
    TOOL_K,
}Current_Key_Model;

static enum enum_mouse_model //6D鼠标工作模式
{
    ANIX_M,
    GLOBAL_M,
    BASE_M,
    TOOL_M,
}Current_Mouse_Model;

typedef enum comd   //指令列表
{
    PTP,
    LIN,
    CIRC,
    SLIN,
    SCIRC
}Command_Order;

typedef enum the_Cont   //转角过渡方式
{
    NO_Cont,
    Has_Cont
}CONT;

typedef struct pos_info  //坐标点的信息
{
    float X_POS_Coor;   //坐标值
    float Y_POS_Coor;
    float Z_POS_Coor;
    float A_Pos_Info;   //姿态
    float B_Pos_Info;
    float C_Pos_Info;
}point_pos;

typedef struct pos_program  //编程时点的名称
{
    char* str;
    int i;
}program_pos;

typedef struct data_program //编程时点的数据
{
    char* name;
    point_pos data;
}program_data;

typedef struct info_point
{
    char* point_name;
    point_pos point_coor;
}point_info;

extern QBitArray DInput;
extern QBitArray DOutput;

/*一条指令的结构体形式*/
typedef struct CMD_Code
{
    Command_Order cmd;
    point_info auxi_point;
    point_info purpose_point;
    CONT ct;
    int spd;
    program_data data;
    int lineNum;
    struct CMD_Code* next;
    struct CMD_Code* prior;
}CMD_Code;


/*指令链表*/
typedef struct
{
    CMD_Code *head;
    CMD_Code *tail;
    int length;
}CMD_LinkList;

void Set_Enable();  //设置使能
void Set_Mouse_Model();  //设置按键和鼠标的工作模式
void Set_Key_Model();

extern QList<QWidget*> listwidget;

extern void set_Cb_Tool(QComboBox* cb);    //设置选择工具号
extern void set_Cb_Base(QComboBox* cb);    //设置选择基坐标系
extern bool VarIsStandable(QString name);   //检查变量名是否合乎标准

extern QRegExp rg_var;
extern QObject *editableItem;
extern int flag_codeeditor;

char* QStringToChar(QString str);
QString CharToQString(char* str);

/*创建一个空的指令链表，只包含头尾节点，长度为0*/
void Init_Cmd_LinkList(CMD_LinkList* linklist);
void Append_CMD_LinkList(CMD_LinkList* linklist , CMD_Code* new_CMD);
void remove_CMD_LinkList(CMD_LinkList* linklist);
void Insert_CMD_LinkList(CMD_LinkList* linklist , CMD_Code *ins, int index);
void Destroy_CMD_LinkList(CMD_LinkList* linklist);
bool IsEmpty(CMD_LinkList* linklist);

void Read_CMDCode(QString str,int line);
void CMD_PTP(QList<QString> cmd_list);
void CMD_LIN(QList<QString> cmd_list);
void CMD_CIRC(QList<QString> cmd_list);
void CMD_SLIN(QList<QString> cmd_list);
void CMD_SCIRC(QList<QString> cmd_list);

void Fill_CMD(QString str, int line);   //
//void Handle_CMD(int line);

#endif // CONDITIONAL_H
