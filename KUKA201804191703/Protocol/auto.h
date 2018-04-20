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
#ifndef AUTO_H
#define AUTO_H

#include <Protocol/robot.h>
#include <Protocol/vm.h>


#define AUTO_OK    0x0
#define AUTO_CIR   0x1
#define AUTO_FLOW  0x2
#define AUTO_END   0x3
#define AUTO_ERR   0xFF


extern u32   uRunState;
extern void  RunState(void);

extern void  InitAuto(sys *pSys);
extern u16   PsdToNckt(nckt *pNckt, sys *pSys, psd *pPsd);

#endif
