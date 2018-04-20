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
#include "macro.h"

char DECL[5]          = "DECL";
char INT_[4]          = "INT";
char BASIS_SUGG_T[13] = "BASIS_SUGG_T";
char E6POS_[6]        = "E6POS";
char PDAT_[5]         = "PDAT";
char FDAT_[5]         = "FDAT";
char LDAT_[5]         = "LDAT";
char TQM_TQDAT_T[12]  = "TQM_TQDAT_T";
  char T11[4]         = "T11";
  char T12[4]         = "T12";
  char T13[4]         = "T13";
  char T14[4]         = "T14";
  char T15[4]         = "T15";
  char T16[4]         = "T16";
  char T21[4]         = "T21";
  char T22[4]         = "T22";
  char T23[4]         = "T23";
  char T24[4]         = "T24";
  char T25[4]         = "T25";
  char T26[4]         = "T26";
  char K1[3]          = "K1";
  char K2[3]          = "K2";
  char K3[3]          = "K3";
  char K4[3]          = "K4";
  char K5[3]          = "K5";
  char K6[3]          = "K6";
  char o1[3]          = "o1";
  char o2[3]          = "o2";
  char ID[3]          = "ID";
  char OVM[4]         = "OVM";
  char TMF[4]         = "TMF";

char VEL[4]           = "VEL";
char ACC[4]           = "ACC";
char APO_MODE[9]      = "APO_MODE";
char APO_DIST[9]      = "APO_DIST";
char CDIS[6]          = "#CDIS";
char TOOL_NO[8]       = "TOOL_NO";
char BASE_NO[8]       = "BASE_NO";
char IPO_FRAME[10]    = "IPO_FRAME";
  char BASE[6]        = "#BASE";
  char TCP[5]         = "#TCP";
char POINT2[9]        = "POINT2[]";
char TQ_STATE[9]      = "TQ_STATE";
  char TRUE_[5]       = "TRUE";
  char FALSE_[6]      = "FALSE";

char APO_FAC[8]       = "APO_FAC";
char AXIS_VEL[9]      = "AXIS_VEL";
char AXIS_ACC[9]      = "AXIS_ACC";
char ORI_TYP[8]       = "ORI_TYP";
  char VAR[5]         = "#VAR";
  char JOINT[7]       = "#JOINT";
  char CONSTANT[10]   = "#CONSTANT";
char CIRC_TYP[9]      = "CIRC_TYP";
char JERK_FAC[9]      = "JERK_FAC";
char GEAR_JERK[10]    = "GEAR_JERK";
char EXAX_IGN[9]      = "EXAX_IGN";
/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------*/

/*代码源文件*****************************************************************************/
char  INI[4]          = "INI";

char  _FOLD[6]        = ";FOLD";
char  _ENDFOLD[9]     = ";ENDFOLD";
char  _BWDSTART[10]   = "$BWDSTART";
char  PDAT_ACT[9]     = "PDAT_ACT";
char  LDAT_ACT[9]     = "LDAT_ACT";
char  FDAT_ACT[9]     = "FDAT_ACT";
char  BAS[4]          = "BAS";
char  _H_POS[7]       = "$H_POS";
char  PTP_[4]         = "PTP";
char  LIN_[4]         = "LIN";
char  CIRC_[5]        = "CIRC";

char  LOOP[5]         = "LOOP";
char  ENDLOOP[8]      = "ENDLOOP";
char  EXIT[5]         = "Exit";
char  WHILE[6]        = "WHILE";
char  ENDWHILE[9]     = "ENDWHILE";
char  IF[3]           = "IF";
char  ELSE[5]         = "ELSE";
char  ENDIF[6]        = "ENDIF";
char  FOR[4]          = "FOR";
char  ENDFOR[7]       = "ENDFOR";

char  DEF[4]          = "DEF";
char  END[4]          = "END";

char  _IN[4]          = "$IN";
char  _OUT[5]         = "$OUT";

/*默认得参数*****************************************************************************/
char  PDEFAULT[9]     = "PDEFAULT";  /* PDAT_ACT */
char  FHOME[6]        = "FHOME";     /* FDAT_ACT */
char  XHOME[6]        = "XHOME";     /* PTP */

/* 关键字的字符识别 */
size_t  SizeVel       = strlen(VEL);
size_t  SizeAcc       = strlen(ACC);
size_t  SizeApoDist   = strlen(APO_DIST);
size_t  SizeApoFac    = strlen(APO_FAC);
size_t  SizeAxisVel   = strlen(AXIS_VEL);
size_t  SizeAxisAcc   = strlen(AXIS_ACC);
size_t  SizeOriTyp    = strlen(ORI_TYP);
size_t  SizeCircTyp   = strlen(CIRC_TYP);
size_t  SizeJerkFac   = strlen(JERK_FAC);
size_t  SizeGearJerk  = strlen(GEAR_JERK);
size_t  SizeExaxIgn   = strlen(EXAX_IGN);
size_t  SizeApoMode   = strlen(APO_MODE);
size_t  SizeToolNo    = strlen(TOOL_NO);
size_t  SizeBaseNo    = strlen(BASE_NO);
size_t  SizeIpoFrame  = strlen(IPO_FRAME);
size_t  SizePoint2    = strlen(POINT2);
size_t  SizeTqState   = strlen(TQ_STATE);
size_t  SizeTxx       = strlen(T11);
size_t  SizeKx        = strlen(K1);
size_t  SizeOvm       = strlen(OVM);

