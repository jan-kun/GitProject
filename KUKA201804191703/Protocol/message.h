/*************************************************************************************
 *                   	       GPCNC
 *           The General-Purposed CNC Software Develop Environment
 *           (c) Copyright 2004-2007, Automation Depatment of NJIT
 *                         All Rights Reserved
 *
 *--------------------------文件信息 --------------------------------------------------
 * 文件名: control.h
 * 描  述: 根据字地址数据的类型，发送不同的控制命令，根据返回的信息类型，执行下一步操作
 *
 * 创建人: zbr
 * 版  本:
 * 日  期:
 *
 * 修改人: zzw
 * 版  本:
 * 日　期: 2009-03-01
 * 修改信息:
 *
 * 说明: 		
 *
 *...
**************************************************************************************/
#ifndef MESSAGE_H
#define MESSAGE_H

#define    KEY_SSP    0
#define    KEY_SSP    0
#define    KEY_LSP    0
#define    KEY_XSP    0


#include "linuxtypes.h"
#include"data.h"


typedef struct
{
    u8 data[16];
}Rock_PKG;

extern i16 Axis_Info[6];   //保存最新的XYZABC值
extern i16 oldAxis_info[6];  //上一个XYZABC值


#endif //MESSAGE_H

/*===============================(H) end of file ==============================*/



