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
#ifndef CODE2PSD_H
#define CODE2PSD_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Protocol/vm.h"
#include "Protocol/IO.h"

#include "macro.h"
#include "var.h"
#include "code2hmi.h"
#include "get_data.h"
#include "open_file.h"
#include "initbuff.h"
#include "Protocol/linuxtypes.h"


extern  bool  GetCmd2Run(vm *pVM);
extern  bool  GetMdlEnter(vm *pVM, char *pDatPath, char *pSrcPath);

#endif
