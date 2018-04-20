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
#ifndef ROBOT_H
#define ROBOT_H

#include <Protocol/bus.h>

typedef struct
{
    i32 aAxis[8];
}vct;
extern vct CurPoint;

typedef struct
{
    i32 aAxis[20];
}v20;


/*-----------------------------------------------------------------------------*//*----------------------------------------------------------------------------*/
typedef struct  /*用户坐标*/
{
    u8      sName[16];       /*用户坐标系名                 */
    vct     xCoor;           /*用户坐标系位姿——结果值     */
    u8      aSet[3];         /*设定时候用,                  */
    vct     aVct[3];         /*坐标系位置 依次org  xx  xy   */
} ucr;

/*----------------------------------------------------------------------------*/
typedef struct  /*工具坐标*/
{
    u8      sName[16];       /*工具坐标系名                 */
    vct     xCoor;           /*工具坐标系位姿——结果值     */
    u8      cSet[6];         /*设定时候用, 不需处理         */
    vct     aTC[6];          /*端部位置校验数据 TC1 -TC5    */
} tcr;

/*----------------------------------------------------------------------------*/
typedef struct  /*发送标志*/
{
    v8     vRef;          /* 标识位               */
    v8     vRbt;          /* 标识位               */
    v8     vMode;         /* 标识位               */
    v8     vTool;         /* 标识位               */
    v8     vUser;         /* 标识位               */
    v8     vCoor;         /* 标识位               */
    v8     vBrand;
    v8     vVer;

} flag;

/*----------------------------------------------------------------------------*/
typedef struct  /* 工具坐标 */
{
    char   ToolName[32];
    double Tool[6];
} tc;

typedef struct   /* 用户坐标 */
{
    char   UserName[32];
    double User[6];
} uc;

/*----------------------------------------------------------------------------*/
typedef struct   /* 用户坐标缓冲区 */
{
    char  ToolName[32];
    char  UserName[32];
    u32   uLine;
} tb;

typedef struct   /* 工具坐标缓冲区 */
{
    char  UserName[32];
    u32   uLine;
} ub;

/*----------------------------------------------------------------------------*/
typedef struct
{
    i8      iMode;           /* 操作模式             */
    i8      iCyc;            /* 循环模式             */
    i8      iRun;            /* 执行的状态           */
    i8      iTry;            /* 试运行状态           */
    i8      iCoor;           /* 操作坐标系类型        */

    i8      iFeed;           /* 手动速度             */
    u16     uVer;
    i8      iNckBuf;         /* NCK缓冲区数量       */
    vct     xVct;            /* 发送，8轴的缓冲区    */
    v20     xV20;            /* 接受，20轴的缓冲区   */

    i8      iUcr;            /* 当前用户坐标系,数组下标   */
    i8      iTcr;            /* 当前刀具坐标系,数组下标   */
    vct     xRef;            /* 当前参考点              */
    vct     xArm;            /* 机器人几何参数           */

    i8      iDecode;         /* 是否已经译码了           */
    i8      iSdNum;          /* 译码要发送的数据包的数量   */
    i8      iCdMod;          /* 0:单循环  1:多循环 */
    i8      iPressStop;      /* 表示停止按钮是否已经被按下 */

    i8      iStopTimer;      /* 停止译码的定时器 */
    i8      iUpdateTool;     /* 是否更新工具坐标 */
    i8      iUpdateUser;     /* 是否更新用户坐标 */
    i8      iStatus;         /* 运行状态       */
    i8      iRunCode;        /*  */

    i8      iChooseF;        /*  */
    i8      iUseInc;         /*  */

    i8      iJogErr;         /* 手动模式操作范围 */
} sys;
/*----------------------------------------------------------------------------*/
#define SYS_MODE_TECH           0x0
#define SYS_MODE_PLAY           0x1
#define SYS_MODE_REMOTE         0x2
#define SYS_MODE_TECH_LOW       0x0
#define SYS_MODE_TECH_FULL      0x3
#define SYS_MODE_AUTO           0x1
/*----------------------------------------------------------------------------*/
#define SYS_COOR_J          0x0     /* 关节 */
#define SYS_COOR_R          0x1     /* 基座标 */
#define SYS_COOR_T          0x2     /* 工具坐标 */
#define SYS_COOR_U          0x3     /* 工件坐标 */
#define SYS_COOR_E          0x4     /* 大地坐标 */
/*----------------------------------------------------------------------------*/
#define SYS_CYC_STEP        0x0     /* 单步     */
#define SYS_CYC_CYCL        0x1     /* 单循环   */
#define SYS_CYC_LOOP        0x2     /* 多循环   */
/*----------------------------------------------------------------------------*/
#define SYS_TRY_NONE        0x0     /*    */
#define SYS_TRY_FORW        0x1     /*    */
#define SYS_TRY_BACK        0x2     /*    */
#define SYS_TRY_ALL         0x3     /*    */
#define SYS_TRY_CRS         0x4     /*    */
/*----------------------------------------------------------------------------*/
#define SYS_RUN_STOP        0x0     /*    */
#define SYS_RUN_AUTO        0x1     /*    */
#define SYS_RUN_WAIT        0x2     /*    */
#define SYS_RUN_FLOW        0x3     /*    */
#define SYS_RUN_READY       0x4     /*    */
/*----------------------------------------------------------------------------*/
#define SYS_DECODE_YES      0x1
#define SYS_DECODE_NOT      0x0
/*----------------------------------------------------------------------------*/
#define SYS_STOP_YES        0x1
#define SYS_STOP_NOT        0x2
/*----------------------------------------------------------------------------*/
#define SYS_RECORD_CYCL     0x0
#define SYS_RECORD_LOOP     0x1
/*----------------------------------------------------------------------------*/
#define SYS_RUNCODE_NOT     0x0
#define SYS_RUNCODE_YES     0x1
/*----------------------------------------------------------------------------*/
#define SYS_TIMER_STOP      0x0
#define SYS_TIMER_START     0x1
/*----------------------------------------------------------------------------*/
#define SYS_UPDATET_YES     0x1
#define SYS_UPDATET_NOT     0x2
/*----------------------------------------------------------------------------*/
#define SYS_UPDATEU_YES     0x1
#define SYS_UPDATEU_NOT     0x2
/*----------------------------------------------------------------------------*/
#define SYS_CHOOSE_YES      0x1
#define SYS_CHOOSE_NOT      0x0
/*----------------------------------------------------------------------------*/
#define SYS_STATUS_RUN      0x1
#define SYS_STATUS_STOP     0x2
/*----------------------------------------------------------------------------*/
#define SYS_INC_NONE        0x0
#define SYS_INC_SMALL       0x1
#define SYS_INC_MID         0x2
#define SYS_INC_BIG         0x3
#define SYS_INC_USER        0x4
/*----------------------------------------------------------------------------*/
#define SYS_NCKBUF_FULL     0x0

/*----------------------------------------------------------------------------*/
#define SYS_JOGERR_NONE     0x0
#define SYS_JOGERR_A1       0x1
#define SYS_JOGERR_A2       0x2
#define SYS_JOGERR_A3       0x3
#define SYS_JOGERR_A4       0x4
#define SYS_JOGERR_A5       0x5
#define SYS_JOGERR_A6       0x6
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#define RUN_STATE_ALM       0x0
#define RUN_STATE_EMG       0x1
#define RUN_STATE_PAUSE     0x2
#define RUN_STATE_RUN       0x3
#define RUN_STATE_STOP      0x4

typedef struct
{
    int   flag;
    nckt  aImdT;          /* 周期发送包           */
} sImdT;

typedef struct
{
    int   flag;
    nckt  aBufT[3];       /* 周期发送包           */
} sBufT;

typedef struct
{
    int  flag;           /* no use               */
    nckt aNulT;          /* 周期发送包             */
} sNulT;

/******************************************************************************/
/* 参数-----------------------------------------------------------------------*/
extern ucr  aUser[24];     /* 用户坐标系           */
extern tcr  aTool[24];     /* 工具坐标系           */
extern tcr  aPhy[24];      /* 工具坐标系物理参数   */

extern flag xFlag;          /* 标识                 */
extern sys  xSys;           /* 系统参数             */

//extern nckt xImdT;          /* 周期发送包           */
//extern nckt xBufT[3];       /* 周期发送包           */
//extern nckt xNulT;          /*  */

extern sImdT xImdT;          /* 周期发送包           */
extern sBufT xBufT;          /* 周期发送包           */
extern sNulT xNulT;          /* 周期发送包           */

extern nckr xBufR;           /* 周期接受包           */

extern nckt xMidT;           /*  */

extern ub   xUserBuf[8];     /*  */
extern tb   xToolBuf[8];     /*  */

extern double TrgPoint[6];

/******************************************************************************/
/* 初始化---------------------------------------------------------------------*/
extern int  InitHMI(void);
extern int  InitCTL(void);

extern void InitNckT(nckt *pNckT);
extern void InitSys(sys *pSys);
extern void InitFlag(flag *pFlag);

/* ---------------------------------------------------------------------------*/
extern void BusNckR(nckr *pNckR);
extern void RunCode(void);

/* ---------------------------------------------------------------------------*/
extern u8   DataFlag(v8 *pFlag);
extern void NcktCmd(nckt *pNckt, u8   uCmd, u8 uSub, u32 uData);
extern void NcktVct(nckt *pNckt, vct *pVct);

/* ---------------------------------------------------------------------------*/
#endif
