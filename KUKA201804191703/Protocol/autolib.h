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
#ifndef AUTOLIB_H
#define AUTOLIB_H

#include "robot.h"

#define ERROR_LABEL        0xFFFF
#define ERROR_NEXT         0xFFFE
#define ERROR_PREV         0xFFFD

typedef struct
{
    u32 uStk[0x80];
    u32 uTop;  
}ip;


/* 跳转到Label */
extern u16 AutoGoto(op *pOP, u32 Label);

/* 跳转下一行 */
extern u16 AutoNext(op *pOP, u32 uIP);

/* 跳转上一行 */
extern u16 AutoPrev(op *pOP, u32 uIP);

/* 子程序跳转 */
extern u16 AutoCall(u8 sSub, ip *pIp);

/* 子程序返回 */
extern u16 AutoRet(ip *pIp);

#endif
