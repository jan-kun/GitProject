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
#include "var.h"
#include "Protocol/vm.h"

e6pos  data_e6pos;
pdat   data_pdat;
fdat   data_fdat;
ldat   data_ldat;
tqmdat data_tqmdat;

size_t SizeVarE6pos  = sizeof(e6pos)  + sizeof(var) - 4;
size_t SizeVarPdat   = sizeof(pdat)   + sizeof(var) - 4;
size_t SizeVarFdat   = sizeof(fdat)   + sizeof(var) - 4;
size_t SizeVarLdat   = sizeof(ldat)   + sizeof(var) - 4;
size_t SizeVarTqmdat = sizeof(tqmdat) + sizeof(var) - 4;

size_t SizeE6pos     = sizeof(e6pos);
size_t SizePdat      = sizeof(pdat);
size_t SizeFdat      = sizeof(fdat);
size_t SizeLdat      = sizeof(ldat);
size_t SizeTqmdat    = sizeof(tqmdat);

size_t SizeVar       = sizeof(var) - 4;

errflag   ErrFlag;
