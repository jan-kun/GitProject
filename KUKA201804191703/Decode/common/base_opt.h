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
#ifndef BASE_OPT_H
#define BASE_OPT_H

#include "Protocol/linuxtypes.h"
#include <stdbool.h>

extern bool CmpStr(char *pCdBuf, u32 *pCdAdd, char *Dst);
extern void CpyStr(char *pCdBuf, u32 *pCdAdd, char *Dst,u8 Length);

extern void GetVarName(char *pCdBuf, u32 *pCdAdd, char *VarName);

extern void EvaData(char *pCdBuf, u32 *pCdAdd);
extern void EndOfLine(char *pCdBuf, u32 *pCdAdd);

extern double GetE6posValue(char *pCdBuf, u32 *pCdAdd, u16 sPara);
  #define    VAR_E6POS     0x0100
    #define  X_DEF         0x0001
    #define  Y_DEF         0x0002
    #define  Z_DEF         0x0003
    #define  A_DEF         0x0004
    #define  B_DEF         0x0005
    #define  C_DEF         0x0006
    #define  S_DEF         0x0007
    #define  T_DEF         0x0008
    #define  E1_DEF        0x0009
    #define  E2_DEF        0x000A
    #define  E3_DEF        0x000B
    #define  E4_DEF        0x000C
    #define  E5_DEF        0x000D
    #define  E6_DEF        0x000E

extern double GetPdatValue(char *pCdBuf, u32 *pCdAdd, u16 sPara);
    #define    VAR_PDAT      0x0200
    #define  VEL_DEF       0x0001
    #define  ACC_DEF       0x0002
    #define  APO_DIST_DEF  0x0003
    #define  APO_MODE_DEF  0x0004

extern u32 GetFdatValue(char *pCdBuf, u32 *pCdAdd, u16 sPara);
  #define    VAR_FDAT      0x0300
    #define  TOOL_NO_DEF   0x0001
    #define  BASE_NO_DEF   0x0002
    #define  IPO_FRAME_DEF 0x0003
    #define  POINT2_DEF    0x0004
    #define  TQ_STATE_DEF  0x0005

extern double GetLdatValue(char *pCdBuf, u32 *pCdAdd, u16 sPara);
  #define    VAR_LDAT      0x0400
  //#define  VEL_DEF       0x0001
  //#define  ACC_DEF       0x0002
  //#define  APO_DIST_DEF  0x0003
    #define  APO_FAC_DEF   0x0004
    #define  AXIS_VEL_DEF  0x0005
    #define  AXIS_ACC_DEF  0x0006
    #define  ORI_TYP_DEF   0x0007
    #define  CIRC_TYP_DEF  0x0008
    #define  JERK_FAC_DEF  0x0009
    #define  GEAR_JERK_DEF 0x000A
    #define  EXAX_IGN_DEF  0x000B

extern double GetTqmdatValue(char *pCdBuf, u32 *pCdAdd, u16 sPara);
  #define    VAR_TQMDAT    0x0500
    #define  T11_DEF       0x0001
    #define  T12_DEF       0x0002
    #define  T13_DEF       0x0003
    #define  T14_DEF       0x0004
    #define  T15_DEF       0x0005
    #define  T16_DEF       0x0006
    #define  T21_DEF       0x0007
    #define  T22_DEF       0x0008
    #define  T23_DEF       0x0009
    #define  T24_DEF       0x000A
    #define  T25_DEF       0x000B
    #define  T26_DEF       0x000C
    #define  K1_DEF        0x000D
    #define  K2_DEF        0x000E
    #define  K3_DEF        0x000F
    #define  K4_DEF        0x0010
    #define  K5_DEF        0x0011
    #define  K6_DEF        0x0012
    #define  O1_DEF        0x0013
    #define  O2_DEF        0x0014
    #define  ID_DEF        0x0015
    #define  OVM_DEF       0x0016
    #define  TMF_DEF       0x0017

extern bool GetBoolValue(char *pCdBuf, u32 *pCdAdd, u16 sPara);

#endif
