/*******************************************************************************
 *                   	       GPCNC
 *           The General-Purposed CNC Software Develop Environment
 *           (c) Copyright 2004-2007, Automation Depatment of NJIT
 *                         All Rights Reserved
 *
 *-----------------------文件信息-----------------------------------------------
 * 文件名: drivers.c
 * 描  述: 提供伺服板的驱动函数
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
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "crc16.h"
#include "bus.h"
#include "robot.h"

u8    aKeyRx[32];
u8    aNckRx[320];
nckt  aNcktTx[4];
nckt  Repetition;
nckr  aNcktRx;

key  *pKeyRx = NULL;
nckr *pNckRx = &aNcktRx;
nckt *pNckTx = NULL;

i8    xSendNum = 0;/* 在实际要发的数据包的数量中的第几个 */
u8    xAppend = 0;     /*  */

/*===============================(C) end of file =============================*/
