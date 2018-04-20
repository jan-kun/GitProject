/*******************************************************************************
 *                   	       GPCNC
 *           The General-Purposed CNC Software Develop Environment
 *           (c) Copyright 2004-2013, Automation Depatment of NJIT
 *                         All Rights Reserved
 *
 *-----------------------文件信息-----------------------------------------------
 * 文件名: .h
 * 描  述:
 *
 * 创建人:
 * 版  本:
 * 日  期:
 *
 * 修改人:
 * 版  本:
 * 日　期:
 * 修改信息:
 *
 * 说明:
 *
 *...
*******************************************************************************/
#ifndef VM_H
#define VM_H

#include  "bus.h"

/*psedo 结构定义------------------------------------------------------------------*/
typedef struct
{
    u16  uOp;          /* 优先级1B+指令1B     2B */
    u16  uSub;         /* 子参数              2B */
    union {
        u32   uData;   /* 立即数              4B */
        i32   iData;   /* 数据指针            4B */
        void *pData;
    };
} psd;
/*操作码-----------------------------------------------------------------------*/
#define OP_NULL     0x0000
#define OP_MOVJ     0x0001
#define OP_MOVL     0x0002
#define OP_MOVC     0x0003
#define OP_MOVS     0x0004
#define OP_MOVAJ    0x0005
  #define OPMOV_VEL     0x0    //data速度
  #define OPMOV_ACC     0x1    //data加速度
  #define OPMOV_CR      0x2    //data允差
  #define OPMOV_END     0x3    //data终点
  #define OPMOV_MID     0x4    //data中点
  #define OPMOV_O       0x5    //data圆心
  #define OPMOV_ZONE    0x6    //拐弯半径
  #define OPMOV_TOOL    0x7    //工具中心点
  #define OPMOV_WOBJ    0X8    //工件坐标系
#define OP_AIO      0x0006     //sub地址    //data  模拟量值
#define OP_DIO      0x0007     //sub地址    //data  值
#define OP_PLS      0x0008     //sub地址    //data  值
#define OP_TIM      0x0009     //sub地址    //data  值
#define OP_CNT      0x000A     //sub地址    //data  值
#define OP_INTR     0x000B     //sub地址    //data  值
#define OP_SYS      0x000C
  #define OPSYS_MODE    0x0    //data
  #define OPSYS_CYCL    0x1    //data
  #define OPSYS_RCOOR   0x2    //data
  #define OPSYS_MCOOR   0x3    //data
  #define OPSYS_UCOOR   0x4    //data
  #define OPSYS_TCOOR   0x5    //data
  #define OPSYS_NOP     0x6
/*程序流程-------------------------------*/
#define OP_PRG      0x0010
  #define OPPRG_NAME    0x0
  #define OPPRG_END     0x1
#define OP_LINE     0x0011
  #define OPLINE_NUM    0x0
  #define OPLINE_END    0x1
#define OP_LBL      0x0012
  #define OPLBL_NAME    0x0
#define OP_CALL     0x0013
  #define OPCALL_NAME   0x0
  #define OPCALL_RET    0x1
#define OP_IF       0x0014
  #define OPIF_EXPR     0x0
  #define OPIF_THN      0x1
  #define OPIF_ELS      0x2
  #define OPIF_EIF      0x3
  #define OPIF_END      0x4
#define OP_SWCH     0x0015
  #define OPSWCH_EXPR   0x0
  #define OPSWCH_END    0x2
#define OP_FOR      0x0016
  #define OPFOR_ST      0x0
  #define OPFOR_STP     0x1
  #define OPFOR_TO      0x2
  #define OPFOR_NXT     0x3
#define OP_WHL      0x0017
  #define OPWHL_ST      0x0
  #define OPWHL_DO      0x1
  #define OPWHL_LOOP    0x2
#define OP_WAIT     0x0018
  #define OPWAIT_TIM    0x0
  #define OPWAIT_IN     0x1
  #define OPWAIT_EXPR   0x3
#define OP_JMP      0x0019
  #define OPJMP_LBL     0x0
  #define OPJMP_EXIT    0x1
  #define OPJMP_CONTI   0x2
/*有优先级，函数、表达式------------------*/
#define OP_LB       0x0100 /* ( */
#define OP_RB       0x0101 /* ) */

#define OP_SIN      0x0201 /*  */
#define OP_COS      0x0202 /*  */
#define OP_ATAN     0x0203 /*  */
#define OP_SQRT     0x0204 /*  */
#define OP_ABS      0x0205 /*  */

#define OP_NOT      0x0301 /* ~ */
#define OP_BNOT     0x0302 /*  */
#define OP_SET      0x0303 /*  */
#define OP_RST      0x0304 /*  */

#define OP_MUL      0x0401 /* * */
#define OP_DIV      0x0402 /* / */
#define OP_MOD      0x0403 /*  */

#define OP_ADD      0x0501 /* + */
#define OP_SUB      0x0502 /* - */

#define OP_GR       0x0600 /* > */
#define OP_GE       0x0601 /* >= */
#define OP_LS       0x0602 /* < */
#define OP_LE       0x0603 /* <= */

#define OP_EQ       0x0700 /* == */
#define OP_NE       0x0701 /* != */

#define OP_AND      0x0800 /* || */
#define OP_BAND     0x0801 /* | */

#define OP_XOR      0x0900
#define OP_BXOR     0x0901

#define OP_OR       0x0A00 /* && */
#define OP_BOR      0x0A01 /* & */

#define OP_ASSIGN   0xFFFF /*  */

/*var 结构定义----------------------------------------------------------------*/
typedef struct
{
    char *pName;            /* 变量名指针code,  32字节    */
    u32  uCrn;              /* 当前变量位置              */
    u32  uPrv;              /* 上一个变量位置             */
    u32  uNxt;              /* 下一个变量位置             */
    u32  uSize;             /* 变量:代表结构体的大小
                             函数名:函数名在代码缓冲区的位置 */
    u8   uType;             /* 变量类型:常量、可变量、变量
                             函数名:该函数名所在的第几个模块 */
    u8   uVar;              /* 表示这个var是存储什么结构体 */
    u16  uDim1;             /* 第一维数组长度,字节数      */
    u16  uDim2;             /* 第二维数组长度,字节数      */
    u16  uDim3;             /* 第三维数组长度,字节数      */
    union {
      i32   nData;          /* 数据data，数组的首个元素   */
      float fData;          /* 数据data，数组的首个元素   */
      char  cData;          /* 数据data，数组的首个元素   */
    };
} var;
/*变量类型--------------------------------------------------------------------*/
#define VT_CONST  0X01  /* 常量 */
#define VT_PERS   0X02  /* 可变量 */
#define VT_VAR    0X03  /* 变量 */

#define VT_CHR    1  /* 字符       */
#define VT_LNG    4  /* 整型       */
#define VT_SNG    4  /* 浮点型     */
#define VT_LBL    1  /* 标号       */
#define VT_MDL    1  /* 模块名     */
#define VT_TRG    56 /* 目标位置，12轴 欧拉法*/
#define VT_JNT    48 /* 关节位置，12轴 48字节 */

/*tci 结构定义---------------------------------------------------------------*/
typedef struct
{
    u32   nSet;             /* 设置值                   */
    union {
      u32   nAct;           /* 实际值                   */
      void (*Intr)(void);   /* 中断函数                 */
    };
} tci;

/*flow 结构定义---------------------------------------------------------------*/
typedef struct
{
    u8   cFlw;                /* 类型，while？loop...  */
    u8   cExt;                /* 比较                  */
    i16  nPos;                /* 下一行位置            */
    var  xVar;                /* for, while, if        */
} flw;
#define FLW_NULL     0x0
#define FLW_IF       0x1
#define FLW_SW       0x2
#define FLW_FOR      0x3
#define FLW_DO       0x4
#define FLW_WHL      0x5

/* cbuf结构定义---------------------------------------------------------------*/
typedef struct
{
    char  aCode[0xFFFFF];   /* 代码缓冲区，1M              */
    u32   nPage;            /* 代码缓冲区页数              */
    u32   nCrn;             /* 代码缓冲区当前位置          */
} cbuf;/*代码缓冲区*/

/* dbuf 结构定义---------------------------------------------------------------*/
typedef struct
{
    char  aData[0x7FFFF];    /* 数据缓冲区，512K           */
    u32   nPage;             /* 数据缓冲区页数             */
    u32   nCrn;              /* 数据缓冲区当前位置         */
} dbuf;/*数据缓冲区*/

typedef struct
{
    i8     iWifl;

    i32    iValueInit;        /* 判断得值                 */
    i32    iValueCrn;
    i32    iValueFinal;

    u32    uAddStart;         /* 在代码缓冲区得地址         */
    u32    uAddCrn;
    u32    uAddEnd;

    char   pMdlCrn[32];
    char   pSubCrn[32];
    u32    uLineStart;        /* 初始得行号值               */

}wifl;/* while if for loop */

#define  WIFL_NULL   0x00
#define  WIFL_IF     0x01
#define  WIFL_FOR    0x02
#define  WIFL_LOOP   0x03
#define  WIFL_WHILE  0x04
#define  WIFL_SWITCH 0x05

/*mdl 结构定义---------------------------------------------------------------*/
typedef struct
{
    char  aMdl[32];         /* 模块名                       */
    u32   nStart;           /* 代码缓冲区位置                */
    u32   nSize;            /* 模块大小                     */

    u32   StartPdat;        /* Pdat数据的开头                */
    u32   StartLdat;
    u32   StartFdat;
    u32   StartE6pos;
    u32   StartTqmdat;
    u32   StartOther;

//    u32   pVtsk;            /* 任务变量                      */
//    u32   pVmdl;            /* 模块变量                      */
//    u32   pVloc;            /* 本地变量                      */
//    u32   pSub;             /* 子程序名                      */
} mdl; /* 依次保存在数据缓冲区，读一个模块，处理一个模块*/

/*sub 结构定义---------------------------------------------------------------*/
typedef struct
{
    char *pModule;          /* 模块名                         */
    char *sSub;             /* 子程序名                       */

    char  cMdlCrn[32];      /* 当前模块名                    */
    char  cSubCrn[32];      /* 当前的子函数名                 */

    u32   nRetC;            /* 代码缓冲区返回                 */
    u32   nRetD;            /* 数据缓冲区返回                 */
    u32   nBlck;            /* 当前译码位置                   */
    psd   aPsd[48];         /* 当前行译码结果                 */
    psd   aStk[16];         /* 当前行表达式执行堆栈           */

    u32   nLineCrn;         /* 行号                         */

//    u32   pVloc;            /* 本地变量                     */
//    u32   pVpgr;            /* 程序级变量                   */
//    u32   pPvar;            /* 参数                           */

    var  *pLbl;             /* 标号                           */
    flw  *pFlw;             /* if.switch.for.while...         */

    wifl  aLogicStk[16];    /* if.switch.for.while.的嵌套      */
    i16   uLogicCrn;        /* 当前的栈顶(初始值为-1)            */

} sub;

/*vm 结构定义----------------------------------------------------------------*/
typedef struct
{
    char   sName[32];     /* 文件名                          */
    cbuf   xCode;         /* 程序缓冲区                       */
    dbuf   xData;         /* 数据缓冲区，包含：               */
                          /*    变量、参数、标号、if.         */
                          /*    switch.for.while...          */
    mdl    aModule[8];    /* 模块，最多8个                   */
    sub    aSubStk[16];   /* 子程序堆栈，最多嵌套16层         */

    tci    aTimer[32];    /* 定时器，计数器                   */
    tci    aCounter[32];  /* 定时器，计数器                   */
    tci    aIntr[16];     /* 中断                             */

    i16    nModule;       /* 当前模块                       */
    i16    numMod;        /* 实际使用的模块数量              */
    i16    nSubCrn;       /* 程序堆栈顶                     */
} vm;

/*变量声明-------------------------------------------------------------------*/
extern vm xVM;
extern vm xMidTask;

/*函数声明-------------------------------------------------------------------*/
extern int  Init(void);
extern int  InitVM(vm *pVm, char *pFile);    /* 初始化虚拟机*/
extern u16  Code2Psd(vm *pVm);               /* 翻译一行代码*/
extern void PsdGo(nckt *pNck, vm *pVm);      /* 解释这段代码*/

extern void Decode(void);

#endif
