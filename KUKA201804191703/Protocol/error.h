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
#ifndef ERROR_H
#define ERROR_H

#include "Protocol/linuxtypes.h"

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
#define  NCK_ERROR_LIMIT   0X01
#define  NCK_ERROR_SIGLE   0X02
  //idata=line   idata=-1
/*-----------------------------------------------------------------------------------*/

#define ERR_PKG_NONE      0x0000
#define ERR_PKG_NCK       0x1001
#define ERR_PKG_HMI       0x1002
#define ERR_PKG_KEY       0x1003

#define ERR_AUTO_CMD      0x1100
#define ERR_AUTO_DATA     0x1101
#define ERR_AUTO_ITP      0x1101


#define ERR_COUNT         0x60



#define ERR_JOINT_A1      0x01
#define ERR_JOINT_A2      0x02
#define ERR_JOINT_A3      0x03
#define ERR_JOINT_A4      0x04
#define ERR_JOINT_A5      0x05
#define ERR_JOINT_A6      0x06
#define ERR_JOINT_A7      0x07
#define ERR_JOINT_A8      0x08



/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/* err-------------------------------------------------------------------------------*/
typedef struct {
    u32     uNum;           /*                  */
    u32     nDate;          /*                  */
    u32     nTime;          /*                  */
    u32     uParam1;        /*                  */
    u32     uParam2;        /*                  */
} err;

/*******************************************************************************/
/* 系统参数--------------------------------------------------------------------*/
extern err  aErr[20];

/******************************************************************************/
extern void InitErr(void);
extern void (*ErrHandle[ERR_COUNT])(u32 uErrNum);

/* ---------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------*/
#endif
