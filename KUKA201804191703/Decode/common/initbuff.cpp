/*******************************************************************************
 *                   	       GPCNC
 *           The General-Purposed CNC Software Develop Environment
 *           (c) Copyright 2004-2013, Automation Depatment of NJIT
 *                         All Rights Reserved
 *
 *-----------------------文件信息-----------------------------------------------
 * 文件名: .c
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
 ******************************************************************************/
#include "initbuff.h"

extern void InitBuff(vm *pVM)
{
    memset(pVM,0,sizeof(vm));

    for(int i = 0; i < 8; i++){
        pVM->aSubStk[i].uLogicCrn = -1;
    }
    pVM->nSubCrn = -1;
}
