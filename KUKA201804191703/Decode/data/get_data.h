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
#ifndef GET_DATA_H
#define GET_DATA_H

#include <stdbool.h>

#include "Protocol/vm.h"
#include "Protocol/linuxtypes.h"
#include "macro.h"
#include "base_opt.h"

typedef struct
{
    bool  flag;    /* 标志这个类型的变量是否已经有了数据 */
    u32   CrnAdd;  /*  */
    u32   NxtAdd;  /* NU */
} VarAddRecord;


typedef struct
{
    VarAddRecord   xE6posAdd;
    VarAddRecord   xPdatAdd;
    VarAddRecord   xFdatAdd;
    VarAddRecord   xLdatAdd;
    VarAddRecord   xTqmdatAdd;

} varaddress;

extern void ReadDatFile(vm *pVM);
extern u32  ProgramGetDat(mdl *pDatMdl, char *pDatBuf, char *VarName, u16 DatTyp);


extern varaddress xVarAdd;

#endif
