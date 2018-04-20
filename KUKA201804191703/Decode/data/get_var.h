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
#ifndef GET_VAR_H
#define GET_VAR_H

#include <stdbool.h>

#include "Protocol/linuxtypes.h"
#include "var.h"

extern u32  SearchSupVar(u32 DatSize, char *pName, char *pCdBuf, u32 *pCdAdd);

extern void GetE6posVar(e6pos *pE6pos, char *pCdBuf, u32 *pCdAdd);
extern void GetPdatVar(pdat *pPdat, char *pCdBuf, u32 *pCdAdd);
extern void GetFdatVar(fdat *pFdat, char *pCdBuf, u32 *pCdAdd);
extern void GetLdatVar(ldat *pLdat, char *pCdBuf, u32 *pCdAdd);
extern void GetTqmdatVar(tqmdat *pTqmdat, char *pCdBuf, u32 *pCdAdd);

#endif
