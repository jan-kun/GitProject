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
#ifndef VAR_DEF_H
#define VAR_DEF_H

#include <stdbool.h>
#include <stdlib.h>

#include "Protocol/linuxtypes.h"

#define NAME_SIZE  32

/*---------------------------------------------------------------------------------
 * 一条语句PTP生成的数据
 * DECL E6POS XP1={X 3425.0,Y 0.0,Z 634.0,A 0.0,B 90.0,C 0.0,S 2,T 0,E1 0.0,E2 0.0,E3 0.0,E4 0.0,E5 0.0,E6 0.0}
 * DECL FDAT FP1={TOOL_NO 1,BASE_NO 0,IPO_FRAME #BASE,POINT2[] " ",TQ_STATE FALSE}
 * DECL PDAT PPDAT1={VEL 100.0,ACC 99.0,APO_DIST 100.0}
 *---------------------------------------------------------------------------------*/

/*E6POS的数据结构体*******************************************************************/
typedef struct
{
    char   cName[NAME_SIZE];
    double dx,dy,dz;/* 工具坐标系的原点相对于法兰坐标系的位置 */
    /* 关于欧拉角,kuka这么定义的 角度A:Z轴的旋转,角度B:Y轴的旋转,角度C:X轴的旋转 */
    double da,db,dc;/* 工具坐标系相对于法兰坐标系的姿态 */
    double ds,dt;
    double de1,de2,de3,de4,de5,de6;/* 外轴的数据 */
} e6pos;

/*FDAT的数据结构体********************************************************************/
typedef struct fdat
{
    char   cName[NAME_SIZE];
    u8     cTool;     /* 工具号  TOOL_NO */
    u8     cBase;     /* 基坐标  BASE_NO*/
    bool   bExt_tcp;  /* 外部tcp true:#tcp false:#base IPO_FRAME*/
    u32    pSupPoint; /* 辅助点 */
    bool   bTq_state; /* 碰撞识别 TQ_STATE*/
} fdat;

/*PDAT的数据结构体********************************************************************/
typedef struct
{
    char  cName[NAME_SIZE];
    u8    cVel;      /* 速度 */
    u8    cAcc;      /* 加速度 */
    u16   sApo_dist; /* 圆滑过渡的距离 */
    bool  bApo_Mode; /* 是否有圆弧过度 #CDIS:有圆滑过度(true)  无:没有圆弧过度(false) */
} pdat;

/*LDAT的数据结构体********************************************************************/
typedef struct
{
    char  cName[NAME_SIZE];
    u8    cVel;      /* 速度 */
    u8    cAcc;      /* 加速度 */
    u16   sApo_dist; /* 圆滑过度的距离 */
    u16   sApo_fac;
    u8    cAxis_vel;
    u8    cAxis_acc;
    u8    cOri_type; /* 方向导引:#VAR #JOINT #CONSTANT */
                     /*         0x01 0x02   0x03*/
    u8    cCirc_type;/* #BASE */
                     /* 0x01  */
    u8    cJerk_fac;
    u8    cGear_jerk;
    u8    cExax_ign;
} ldat;

/*TQM_TQDAT_T的数据结构体*************************************************************/
typedef struct
{
    char  cName[NAME_SIZE];
    u32   lTxx[12];
    u32   lKx[6];
    u32   lOx[2];
    u32   lId;
    u32   lOvm;
    u32   lTmf;
} tqmdat;

/*PTP指令用到的数据使用的结构体*********************************************************/
typedef struct
{
    e6pos dat_e6pos;
    fdat  dat_fdat;
    pdat  dat_pdat;
} ptp;/* point to point */

/*---------------------------------------------------------------------------------*/
typedef struct
{
    u32   LineNo; /* 错误的行号 */
    bool  flag;   /* 有错误:true 没有错误:false */
} errflag;

extern e6pos  data_e6pos;
extern pdat   data_pdat;
extern fdat   data_fdat;
extern ldat   data_ldat;
extern tqmdat data_tqmdat;

extern size_t SizeE6pos;
extern size_t SizePdat;
extern size_t SizeFdat;
extern size_t SizeLdat;
extern size_t SizeTqmdat;

extern size_t SizeVarE6pos;
extern size_t SizeVarPdat;
extern size_t SizeVarFdat;
extern size_t SizeVarLdat;
extern size_t SizeVarTqmdat;

extern size_t SizeVar;

extern errflag   ErrFlag;

#endif
