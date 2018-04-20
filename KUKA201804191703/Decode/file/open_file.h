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
#ifndef OPEN_FILE_H
#define OPEN_FILE_H

#include "Protocol/vm.h"
#include "Protocol/IO.h"
#include <stdbool.h>

extern bool ScanFile(vm *pVM,char *pDatPath,char *pSrcPath);
extern bool GetIOState(u8 *pIOArray, u16 Addr);

#endif
