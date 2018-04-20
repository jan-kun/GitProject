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
#ifndef MACRO_H
#define MACRO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Protocol/linuxtypes.h"

/*特定字符定义**************************************************************************/
extern  char DECL[5];

extern  char INT_[4];
extern  char BASIS_SUGG_T[13];
extern  char FDAT_[5];
extern  char LDAT_[5];
extern  char PDAT_[5];
extern  char E6POS_[6];
extern  char TQM_TQDAT_T[12];
  extern char T11[4];
  extern char T12[4];
  extern char T13[4];
  extern char T14[4];
  extern char T15[4];
  extern char T16[4];
  extern char T21[4];
  extern char T22[4];
  extern char T23[4];
  extern char T24[4];
  extern char T25[4];
  extern char T26[4];
  extern char K1[3];
  extern char K2[3];
  extern char K3[3];
  extern char K4[3];
  extern char K5[3];
  extern char K6[3];
  extern char o1[3];
  extern char o2[3];
  extern char ID[3];
  extern char OVM[4];
  extern char TMF[4];

extern  char VEL[4];
extern  char ACC[4];
extern  char APO_MODE[9];
extern  char APO_DIST[9];
extern  char CDIS[6];
extern  char TOOL_NO[8];
extern  char BASE_NO[8];
extern  char IPO_FRAME[10];
  extern  char BASE[6];
  extern  char TCP[5];
extern  char POINT2[9];
extern  char TQ_STATE[9];
  extern  char TRUE_[5];
  extern  char FALSE_[6];
extern  char APO_FAC[8];
extern  char AXIS_VEL[9];
extern  char AXIS_ACC[9];
extern  char ORI_TYP[8];
  extern char VAR[5];
  extern char JOINT[7];
  extern char CONSTANT[10];
extern  char CIRC_TYP[9];
extern  char JERK_FAC[9];
extern  char GEAR_JERK[10];
extern  char EXAX_IGN[9];
/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/

/*代码源文件*****************************************************************************/
extern char  INI[4];

extern char  _FOLD[6];
extern char  _ENDFOLD[9];
extern char  _BWDSTART[10];
extern char  PDAT_ACT[9];
extern char  LDAT_ACT[9];
extern char  FDAT_ACT[9];
extern char  BAS[4];
extern char  _H_POS[7];
extern char  PTP_[4];
extern char  LIN_[4];
extern char  CIRC_[5];

extern char  LOOP[5];
extern char  ENDLOOP[8];
extern char  EXIT[5];
extern char  WHILE[6];
extern char  ENDWHILE[9];
extern char  IF[3];
extern char  ELSE[5];
extern char  ENDIF[6];
extern char  FOR[4];
extern char  ENDFOR[7];

extern char  DEF[4];
extern char  END[4];

extern char  _IN[4];
extern char  _OUT[5];

/*默认得参数*****************************************************************************/
extern char  PDEFAULT[9];
extern char  FHOME[6];
extern char  XHOME[6];

/*字符长度定义****************************************************************************/
extern  size_t  SizeVel;
extern  size_t  SizeAcc;
extern  size_t  SizeApoDist;
extern  size_t  SizeApoFac;
extern  size_t  SizeAxisVel;
extern  size_t  SizeAxisAcc;
extern  size_t  SizeOriTyp;
extern  size_t  SizeCircTyp;
extern  size_t  SizeJerkFac;
extern  size_t  SizeGearJerk;
extern  size_t  SizeExaxIgn;
extern  size_t  SizeApoMode;
extern  size_t  SizeToolNo;
extern  size_t  SizeBaseNo;
extern  size_t  SizeIpoFrame;
extern  size_t  SizePoint2;
extern  size_t  SizeTqState;
extern  size_t  SizeTxx;
extern  size_t  SizeKx;
extern  size_t  SizeOvm;

#endif
