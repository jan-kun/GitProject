/*************************************************************************************
 *                   	       GPCNC
 *           The General-Purposed CNC Software Develop Environment
 *           (c) Copyright 2004-2007, Automation Depatment of NJIT
 *                         All Rights Reserved
 *
 *--------------------------文件信息 --------------------------------------------------
 * 文件名: control.h
 * 描  述: 根据字地址数据的类型，发送不同的控制命令，根据返回的信息类型，执行下一步操作
 *
 * 创建人: zbr
 * 版  本:
 * 日  期:
 *
 * 修改人: zzw
 * 版  本:
 * 日　期: 2009-03-01
 * 修改信息:
 *
 * 说明:
 *
 *...
**************************************************************************************/
#ifndef BUS_H
#define BUS_H

#include "linuxtypes.h"

/******************************************************************************/
/*key  结构定义---------------------------------------------------------------*/
#ifdef ABB_HMI
typedef struct {
    u8   cHeader;             /* 数据包的，0x55                 1B */
    u8   cAddr;               /* 板地址，  8位表示              1B */
    u8   cAppend;             /* 辅助信息，重复次数等           1B */
    u8   cCmd;                /* 命令，    8位表示              1B */

    u16  uUpDown;
    u16  uPosX;
    u16  uPosY;

    u16  uMovXA;
    u16  uMovYB;
    u16  uMovZC;

    u8   aDI[4];
    u8   aDO[4];

    u16  nCRC;                /* CRC                            2B */
    u16  nTemp;               /* 冗余字节, 0xAAAA               2B */
}key; /* 定长，BYTE_PKG = 24, BYTE_CRC = 26, BYTE_ALL = 28 */

#else
typedef struct {
    u8   cHeader;             /* 数据包的，0x55                 1B */
    u8   cAddr;               /* 板地址，  8位表示              1B */
    u8   cAppend;             /* 辅助信息，重复次数等           1B */
    u8   cCmd;                /* 命令，    8位表示              1B */

    u8   aDI [12];            /* 数字量输入，96b               12B */
    u8   aDO [8];             /* 数字量输出，64b                8B */

    u16  nCRC;                /* CRC                            2B */
    u16  nTemp;               /* 冗余字节, 0xAAAA               2B */
}key; /* 定长，BYTE_PKG = 24, BYTE_CRC = 26, BYTE_ALL = 28 */

#endif

/*nckt    结构定义------------------------------------------------------------*/
typedef struct
{
    u8   cHeader;            /* 数据包的，0x55                 1B */
    u8   cAddr;              /* 板地址，  8位表示              1B */
    u8   cAppend;            /* 辅助信息，重复次数等           1B */
    u8   cNck;               /* 命令，    8位表示              1B */

    u8   aDI[20];            /* 命令参数，如IO, 位置等        20B */
    u8   aDO[20];            /* 命令参数，如IO, 位置等        20B */

    u8   uCmd[8];            /* 命令                           1B */
    u8   uSub[8];            /* 子命令                         1B */
    union {
        u32  uData[8];       /* 命令参数，如IO, 位置等            */
        i32  iData[8];       /* 命令参数，如IO, 位置等         4B */
    };

    i32  aAxis[8];           /* 位置等                        32B */

    u16  nCRC;               /* CRC                            2B */
    u16  nTemp;              /* 冗余字节, 0xAAAA               2B */
}nckt; /* 定长，BYTE_PKG = 40 + 80 = 120, BYTE_CRC = 124, BYTE_ALL = 128 */

/*nckr  结构定义--------------------------------------------------------------*/
typedef struct
{
    u8   cHeader;            /* 数据包的，0x55                 1B */
    u8   cAddr;              /* 板地址，  8位表示              1B */
    u8   cAppend;            /* 辅助信息，重复次数等           1B */
    u8   cNck;               /* 命令，    8位表示              1B */

    u8   aDI[20];            /* 命令参数，如IO, 位置等        20B */
    u8   aDO[20];            /* 命令参数，如IO, 位置等        20B */

    u8   uCmd[4];            /* 命令                           1B */
    u8   uSub[4];            /* 子命令                         1B */
    union {
        u32  uData[4];       /* 命令参数，如IO, 位置等            */
        i32  iData[4];       /* 命令参数，如IO, 位置等         4B */
    };

    i32  aAxis[20];          /* 位置等                        80B */

    u16  nCRC;               /* CRC                            2B */
    u16  nTemp;              /* 冗余字节, 0xAAAA               2B */
}nckr; /* 定长，BYTE_PKG = 40 + 104 = 144, BYTE_CRC = 148, BYTE_ALL = 152 */
/*----------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#define NCK_NULL         0x0
#define NCK_IMD          0x1
#define NCK_BUF          0x2
/*--------------------------------------------------------------------------*/
#define NCK_MODE     0x01   /* 模式,SUB:                       			6B  */
  #define NCK_MODE_MANU   0x00  /* 模式,SUB, 示教           			    */
  #define NCK_MODE_AUTO   0x01  /* 模式,SUB:自动            		 	    */
  #define NCK_MODE_RMT    0x02  /* 模式,SUB:远程           			      */
#define NCK_PULSE    0x02   /* 脉冲输出  iData:脉冲周期              6B */
#define NCK_TIMER    0x03   /* 定时器    定时值ms                    6B */
#define NCK_IO       0x04   /* DI操作,     uData = L16+H16           6B */
  #define NCK_DI          0x00  /* SUB, DI; L16-ADDR, H16-VAL         */
  #define NCK_DO          0x01  /* SUB, DO; L16-ADDR, H16-VAL         */
  #define NCK_AI          0x02  /* SUB, AI; L16-ADDR, H16-VAL         */
  #define NCK_AO          0x03  /* SUB, AO; L16-ADDR, H16-VAL         */
#define NCK_JOG      0x05   /* 点动速度                              6B */
#define NCK_ITP      0x06   /* 插补参数                               B */
  #define NCK_ITP_VJNT    0x00  /* VX, 关节速度(%)                    */
  #define NCK_ITP_VMOV    0x01  /* VX, 端部线速度(mm/s)               */
  #define NCK_ITP_VROT    0x02  /* VX, 姿态转速(??)                   */
  #define NCK_ITP_VEXT    0x03  /* VX, 外部轴速度(mm/s)               */
  #define NCK_ITP_PL      0x04  /* 位置等级                           */
  #define NCK_ITP_CR      0x05  /* 转角半径                           */
  #define NCK_ITP_ACC     0x07  /* 插补加速度                         */
  #define NCK_ITP_DEC     0x08  /* 插补减速度                         */

#define NCK_SRV      0x10   /* 伺服使能  uData:使能值                6B */
#define NCK_LINE     0x11   /* 行号      uData:行号                  6B */
#define NCK_NEW      0x12   /* 新文件                                6B */
#define NCK_END      0x13   /* 结束                                  6B */
//#define NCK_RBT      0x14   /* 机器人类型，使用轴参数max32B         6B */

/*--------------------------------------------------------------------------*/
#define NCK_MOV      0x20   /* 插补类型  使用轴参数max32B           6B */
  #define NCK_MOVJ        0x00   /* 插补点                         */
  #define NCK_MOVL        0x01   /* 插补点                         */
  #define NCK_MOVCM       0x02   /* 插补点                         */
  #define NCK_MOVCE       0x03   /* 插补点                         */
  #define NCK_MOVSM       0x04   /* 插补点                         */
  #define NCK_MOVSE       0x05   /* 插补点                         */
  #define NCK_IMOVJ       0x06   /* 插补点                         */
  #define NCK_IMOVL       0x07   /* 插补点                         */
    #define NCK_DATA_JNT        0x00  /* 坐标类型：关节                */
    #define NCK_DATA_RECT       0x01  /* 坐标类型：直角                */
#define NCK_COOR     0x21   /*  Jog坐标系                           6B */
  #define NCK_COOR_JNT    0x00   /* Jog坐标系:关节                   */
  #define NCK_COOR_RECT   0x01   /* Jog坐标系:直角                   */
  #define NCK_COOR_TOOL   0x02   /* Jog坐标系:工具                   */
  #define NCK_COOR_USER   0x03   /* Jog坐标系:用户                   */
#define NCK_REF      0x22   /*参考点  使用轴参数max32B              6B */
  #define NCK_REF_SET     0x01   /* 设置原点                         B */
  #define NCK_REF_GET     0x02   /* 获取原点                         B */
#define NCK_SHIFT    0x23   /*平移                                  6B */
#define NCK_GOTO     0x24    /*MOVE to  point                       6B */

#define NCK_SET      0x30   /* 坐标系   使用轴参数max32B             6B */
  #define NCK_SET_TOOL    0x00    /* 工具坐标系                        */
  #define NCK_SET_USER    0x01    /* 用户坐标系                        */
  #define NCK_SET_BRAND   0x02    /* 机器人品牌                        */
  #define NCK_SET_RBT     0X03    /*  */
#define NCK_GET           0x31
  #define NCK_GET_VER     0X00    /* VERSION OF SYSYTEM */

/*--------------------------------------------------------------------------*/
#define NCK_ERROR    0xFF   /* SUB, 错误码,     uData:错误马            */

/*-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
extern u8   aKeyRx[32];

extern nckt aNcktTx[4];
extern nckr aNcktRx;
extern u8   aNckRx[320];

extern key  *pKeyRx;
extern nckt *pNckTx;
extern nckt  Repetition;
extern nckr *pNckRx;

extern i8   xSendNum;
extern u8   xAppend;
/*----------------------------------------------------------------------------*/

#endif //CNC_BUS_H

/*===============================(H) end of file ==============================*/


