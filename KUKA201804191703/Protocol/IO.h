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
#ifndef IO_H
#define IO_H
#include "linuxtypes.h"

/* IO VAL -------------------------------------------------------------*/
#define IO_ON    0x80000000
#define IO_OFF   (~IO_ON)

/* KEY IO--------------------------------------------------------------*/
#define KEY_DI_START    0x00       /* start按钮状态位      */
#define KEY_DI_HOLD     0x01       /* hold按钮状态位       */
#define KEY_DI_MODE0    0x02     /* mode0旋钮状态位      */
#define KEY_DI_MODE1    0x03     /* mode1旋钮状态位      */
#define KEY_DI_SERVO    0x10       /* 使能输入中间变量     */

#define KEY_DO_START    0x0
#define KEY_DO_HOLD     0x1
#define KEY_DO_SERVO    0x2
#define KEY_DO_POWER    0x3

/* NCK_DIO(0x00-0x1f, 4B,32b)---------------------------------------------------*/
#define NCK_DI_XSP        0x00
#define NCK_DI_XSM       0x01
#define NCK_DI_YLP        0x02
#define NCK_DI_YLM       0x03
#define NCK_DI_ZUP       0x04
#define NCK_DI_ZUM      0x05
#define NCK_DI_XRP       0x06
#define NCK_DI_XRM      0x07
#define NCK_DI_YBP       0x08
#define NCK_DI_YBM      0x09
#define NCK_DI_ZTP       0x0A
#define NCK_DI_ZTM      0x0B
#define NCK_DI_7P         0x0C
#define NCK_DI_7M        0x0D
#define NCK_DI_8P         0x0E
#define NCK_DI_8M        0x0F

#define NCK_DI_HOLD       0x10    /* 状态保持，减速停     */
#define NCK_DI_START       0x11    /* 状态运行             */
#define NCK_DI_SERVO      0x12    /* 伺服使能             */
#define NCK_DI_MODE0      0x13    /* mode0旋钮状态位      */
#define NCK_DI_MODE1      0x14    /* mode1旋钮状态位      */


#define NCK_DI_STOP       0x18    /* 1:STOP   0:RUN     */


#define NCK_DI_DEAD         0x20    /*deadman                  */
#define NCK_DI_EMG          0x21    /* EMG                         */

//#define NCK_DI_AA0      0x18    /* 轴0报警     */
//#define NCK_DI_AA1      0x19    /* 轴1报警     */
//#define NCK_DI_AA2      0x1A    /* 轴2报警     */
//#define NCK_DI_AA3      0x1B    /* 轴3报警     */
//#define NCK_DI_AA4      0x1C    /* 轴4报警     */
//#define NCK_DI_AA5      0x1D    /* 轴5报警     */
//#define NCK_DI_AA6      0x1E    /* 轴6报警     */
//#define NCK_DI_AA7      0x1F    /* 轴7报警     */


#define NCK_DI_AA0       0x30    /* 轴0报警     */
#define NCK_DI_AA1       0x31    /* 轴1报警     */
#define NCK_DI_AA2       0x32    /* 轴2报警     */
#define NCK_DI_AA3       0x33    /* 轴3报警     */
#define NCK_DI_AA4       0x34    /* 轴4报警     */
#define NCK_DI_AA5       0x35    /* 轴5报警     */


#define NCK_DO_HOLD      0x00    /* 状态保持，减速停     */
#define NCK_DO_START     0x01    /* 状态运行             */
#define NCK_DO_SERVO     0x02    /* 伺服使能             */

/*  APP_DIO(0x20-   ,   )------------------------------------------------------*/
#define APP_IO_ADDR        0x20    /* 系统输入 */

//#define APP_DO_TOOL     0x0C    /*工具*/
//#define APP_DO_WELD     0x0C    /*弧焊*/
//#define APP_DO_WAVE     0x0C    /*摆焊*/


#define APP_DO_TOOL     0x0    /*工具*/
#define APP_DO_WELD     0x0    /*弧焊*/
#define APP_DO_WAVE     0x0    /*摆焊*/

/* FANUC */
#define APP_IN_SYS         0x20    /* 系统输入 */
#define APP_IN_USER       0x30    /* 用户输入 */
#define APP_OUT_SYS       0x20    /* 系统输出 */
#define APP_OUT_USER    0x2C    /* 用户输出 */

/*---------------------------------------------------------------------*/
typedef struct
{
    int   flag;        /*  */
    u8    ioDIc[20];   /* 256b           */
} sDIc;

typedef struct
{
    int   flag;
    u8    ioDOc[20];   /* 256b           */
} sDOc;
/*---------------------------------------------------------------------*/

/*Key状态---------------------------------------------------------------*/
extern u8    ioKIc[16];   /* 128b           */
extern u8    ioKOc[16];   /* 128b           */
extern u8    ioKIp[16];   /* 128b           */
extern u8    ioKOp[16];   /* 128b           */

/*Pad状态---------------------------------------------------------------*/
extern u8    ioPIc[16];   /* 128b           */
extern u8    ioPOc[16];   /* 128b           */
extern u8    ioPIp[16];   /* 128b           */
extern u8    ioPOp[16];   /* 128b           */

/*DIO状态---------------------------------------------------------------*/
extern u8    ioDIc[20];   /* 256b           */
extern u8    ioDOc[20];   /* 256b           */

/*DIO状态---------------------------------------------------------------*/
extern sDIc  ioDIState;
extern sDOc  ioDOState;

extern u8    ioDIp[20];   /* 256b           */
extern u8    ioDOp[20];   /* 256b           */

/*AIO状态---------------------------------------------------------------*/
extern u16   ioAI[8];     /* 8CH            */
extern u16   ioAO[4];     /* 4CH            */

/*MIO------------------------------------------------------------------*/
extern u8    ioMIc[64];   /* 512b           */
extern u8    ioMOc[64];   /* 512b           */
extern u8    ioMIp[64];   /* 512b           */
extern u8    ioMOp[64];   /* 512b           */

/*其它元件--------------------------------------------------------------*/
extern u16   tcDi[40];    /* TC初始值       */
extern u16   tcDc[40];    /* TC当前值       */
extern u8    tcEN[40];    /* TC使能         */
extern u8    tcM [40];    /* TC模式         */

/*define Function--------------------------------------------------------*/
extern u8    IO_Get(u8 *pIO, u16 uAddr);
extern void  IO_Set(u8 *pIO, u16 uAddr);
extern void  IO_Reset(u8 *pIO, u16 uAddr);
#endif
