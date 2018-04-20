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

#ifndef RUN_CMD_H
#define RUN_CMD_H

#include "Protocol/vm.h"

typedef struct code2hmi
{
    char    pMdlCrn[32];     /* 这里的module是指界面的程序模块 */
    char    pSubCrn[32];     /* 当前的子程序 */
    u32     LineCrn;         /* 当前程序的行号 */
}code2hmi;

extern  code2hmi  CodeRecord; /* 程序记录用的 */

#endif
