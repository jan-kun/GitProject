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
#ifndef TRANSFORM_H
#define TRANSFORM_H

/*-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
typedef struct {
    double l1;              /* 机器人底座高         */
    double l2;              /* s轴轴线与l轴轴线距离 */
    double l3;              /* 大臂长度             */
    double l4;              /* u轴轴线到r轴轴线距离 */
    double l5;              /* 小臂长度             */
    double l6;              /* 工具长度             */
    double l7;              /* sqrt(l4*l4+l5*l5)    */
    double l8;              /* 工作台高度           */
    double Al5;             /* arctan(l5/l4)        */

    double dMr[3][3];       /* 末端姿态矩阵         */
    double dPos[6];         /* 末端位置             */

}rarm;

/* ---------------------------------------------------------------------------*/
extern rarm  xRarm;

extern void InitArm(rarm *pRarm, vct *pVct);
extern void CalcEnd(double *pA, rarm *pRarm);
extern void Matrix2Euler(double *pAr, double **qMr);
extern void Points2Matrix(double *pPo, double *pPx, double *pPy, double **qMr);

/* ---------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------*/

#endif
