/*******************************************************************************
 *                   	       GPCNC
 *           The General-Purposed CNC Software Develop Environment
 *           (c) Copyright 2004-2013, Automation Depatment of NJIT
 *                         All Rights Reserved
 *
 *-----------------------文件信息-----------------------------------------------
 * 文件名: variable.h
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
#ifndef VARIABLE_H
#define VARIABLE_H

#include "rtype.h"

/*----------------------------------------------------------------------------*/
typedef struct  /*系统变量*/
{
    i32     iData;          /* 值                   */
    u32     uState;         /* 状态                 */
    u8      sNote[20];      /* 注释                 */
} rsys;

/*----------------------------------------------------------------------------*/
typedef struct  /*位置变量*/
{
    vct     xData;          /* 值                   */
    u32     uState;         /* 状态                 */
    u8      sNote[20];      /* 注释                 */
} rpos;

/* 参数-----------------------------------------------------------------------*/
extern rsys aRin[32];
extern rsys aRout[32];
//extern rsys aRdat[128];
rsys aRdat[128];
//extern rpos aRpos[128];
rpos aRpos[128];

#endif
/*-----------------end of file----------------------*/
