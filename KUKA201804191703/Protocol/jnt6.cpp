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
 ***********************************************************************************/
/***********************************************************************************/
#include "Protocol/linuxtypes.h"

#include <stdlib.h>
#include <math.h>

#include "rtype.h"
#include "robot.h"
#include "transform.h"

/* ---------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------*/
rarm  xRarm;

/* ---------------------------------------------------------------------------*/
extern void InitArm(rarm *pRarm, vct *pArm)
{
    pRarm->l1  = pArm->aAxis[0] * 0.001;        /* 机器人底座高         */
    pRarm->l2  = pArm->aAxis[1] * 0.001;        /* s轴轴线与l轴轴线距离 */
    pRarm->l3  = pArm->aAxis[2] * 0.001;        /* 大臂长度             */
    pRarm->l4  = pArm->aAxis[3] * 0.001;        /* u轴轴线到r轴轴线距离 */
    pRarm->l5  = pArm->aAxis[4] * 0.001;        /* 小臂长度             */
    pRarm->l6  = pArm->aAxis[5] * 0.001;        /* 工具长度             */
    pRarm->l7  = sqrt( pRarm->l4 * pRarm->l4 + pRarm->l5 * pRarm->l5);  
                                                /* sqrt(l4*l4+l5*l5)    */
    pRarm->l8  = 0;                             /* 工作台高度           */
    pRarm->Al5 = atan(pRarm->l5 / pRarm->l4);   /* arctan(l5/l4)        */

    return;  
}

/*******************************************************************************
* 函数名：CalcEnd
* 说  明：根据机器人六个关节的角度值计算机器人末端的位置和方向向量
* 参  数：dA[6]机器人各关节的弧度，rarm机器人正运算结构体
* 返回值：无
* 输  出：直接修改缓冲区数值
* 日  期：2014.07.06
*
*******************************************************************************/
void CalcEnd(double *pA, rarm *pRarm)
{
    unsigned char i;

    double  SinAs,  CosAs,  SinAl,  CosAl;
    double  dLc[3], dLd[3], dLe[3], dLf[3];
    double  dPx[3], dPy[3], dPz[3];
    double  dS[3],  dC[3],  dAr[3], dMr[3][3];

    SinAs = sin(pA[0]),  CosAs = cos(pA[0]);
    SinAl = sin(pA[1]),  CosAl = cos(pA[1]);
    
    dLc[0] = pRarm->l2 * CosAs;
    dLc[1] = pRarm->l2 * SinAs;
    dLc[2] = pRarm->l1 + pRarm->l8;
    
    dLd[0] = dLc[0] + pRarm->l3 * SinAl * CosAs;
    dLd[1] = dLc[1] + pRarm->l3 * SinAl * SinAs;
    dLd[2] = dLc[2] + pRarm->l3 * CosAl;

    dLe[0] = dLd[0] + pRarm->l4 * sin(pA[1] - pA[2]) * CosAs;
    dLe[1] = dLd[1] + pRarm->l4 * sin(pA[1] - pA[2]) * SinAs;
    dLe[2] = dLd[2] + pRarm->l4 * cos(pA[1] - pA[2]);

    dLf[0] = dLd[0] + pRarm->l7 * sin(PI - pA[1] - pRarm->Al5 + pA[2]) * CosAs;
    dLf[1] = dLd[1] + pRarm->l7 * sin(PI - pA[1] - pRarm->Al5 + pA[2]) * SinAs;
    dLf[2] = dLd[2] - pRarm->l7 * cos(PI - pA[1] - pRarm->Al5 + pA[2]);
    
    for(i = 0; i <= 2; i++) dPz[i] = (dLf[i] - dLe[i]) / pRarm->l5;
    
    dPy[0] = -SinAs;
    dPy[1] = CosAs;
    dPy[2] = 0;
    
    dPx[0] = dPy[1] * dPz[2] - dPy[2] * dPz[1];
    dPx[1] = dPy[2] * dPz[0] - dPy[0] * dPz[2];
    dPx[2] = dPy[0] * dPz[1] - dPy[1] * dPz[0];
    
    for(i = 0; i <= 2; i++)
    {
        dAr[i] = -pA[3 + i];
        dS[i]  = sin(dAr[i]);
        dC[i]  = cos(dAr[i]);
    }
   
    dMr[0][0] =  dC[0] * dC[1] * dC[2] - dS[0] * dS[2];
    dMr[1][0] =  dS[0] * dC[1] * dC[2] + dC[0] * dS[2];
    dMr[2][0] = -dS[1] * dC[2];
  
    dMr[0][1] = -dC[0] * dC[1] * dS[2] - dS[0] * dC[2];
    dMr[1][1] = -dS[0] * dC[1] * dS[2] + dC[0] * dC[2];
    dMr[2][1] =  dS[1] * dS[2];
  
    dMr[0][2] =  dC[0] * dS[1];
    dMr[1][2] =  dS[0] * dS[1];
    dMr[2][2] =  dC[1];    
    
    for(i = 0; i <= 2; i++)
    {
        pRarm->dMr[0][i] = dPx[0] * dMr[0][i] + dPy[0] * dMr[1][i] + dPz[0] * dMr[2][i];
        pRarm->dMr[1][i] = dPx[1] * dMr[0][i] + dPy[1] * dMr[1][i] + dPz[1] * dMr[2][i];
        pRarm->dMr[2][i] = dPx[2] * dMr[0][i] + dPy[2] * dMr[1][i] + dPz[2] * dMr[2][i];
    }
    pRarm->dPos[0] = dLf[0] + pRarm->l6 * pRarm->dMr[0][2];
    pRarm->dPos[1] = dLf[1] + pRarm->l6 * pRarm->dMr[1][2];
    pRarm->dPos[2] = dLf[2] + pRarm->l6 * pRarm->dMr[2][2];
    
    return;
}

/*******************************************************************************
 * 函数名：Points2Matrixr
 * 说  明：机器人姿态矩阵相对于直角坐标系下旋转量Z-Y-X
 * 参  数：dAr[3]旋转角度X、Y、Z,dTr[3][3]空间姿态矩阵
 * 返回值：无
 * 输  出：直接修改缓冲区数值
 * 日  期：2014.07.06
 *
 *******************************************************************************/
void Points2Matrix(double *pPo, double *pPx, double *pPy, double **qMr)
{
    double dDx[3], dDy[3], dDz[3], dTemp;

    dDx[0] = pPx[0] - pPo[0];
    dDx[1] = pPx[1] - pPo[1];
    dDx[2] = pPx[2] - pPo[2];

    dDy[0] = pPy[0] - pPo[0];
    dDy[1] = pPy[1] - pPo[1];
    dDy[2] = pPy[2] - pPo[2];

    dDz[0] = dDx[1] * dDy[2] - dDx[2] * dDy[1];
    dDz[1] = dDx[2] * dDy[0] - dDx[0] * dDy[2];
    dDz[2] = dDx[0] * dDy[1] - dDx[1] * dDy[0];

    dTemp = 1.0 / sqrt(dDx[0] * dDx[0] + dDx[1] * dDx[1] + dDx[2] * dDx[2]);    
    qMr[0][0] = dDx[0] * dTemp;
    qMr[1][0] = dDx[1] * dTemp;
    qMr[2][0] = dDx[2] * dTemp;

    dTemp = 1.0 / sqrt(dDz[0] * dDz[0] + dDz[1] * dDz[1] + dDz[2] * dDz[2]);    
    qMr[0][2] = dDz[0] * dTemp;
    qMr[1][2] = dDz[1] * dTemp;
    qMr[2][2] = dDz[2] * dTemp;

    qMr[0][1] = qMr[2][1] * qMr[0][2] - qMr[2][2] * qMr[0][1];
    qMr[1][1] = qMr[2][2] * qMr[0][0] - qMr[2][0] * qMr[0][2];
    qMr[2][1] = qMr[2][0] * qMr[0][1] - qMr[2][1] * qMr[0][0];
    
    return;
}
/*******************************************************************************
 * 函数名：Matrix2Euler
 * 说  明：机器人姿态矩阵相对于直角坐标系下旋转量Z-Y-X
 * 参  数：dAr[3]旋转角度X、Y、Z,dTr[3][3]空间姿态矩阵
 * 返回值：无
 * 输  出：直接修改缓冲区数值
 * 日  期：2014.07.06
 *
 *******************************************************************************/
void Matrix2Euler(double *pAr, double **qMr)
{
    double dTemp;
    
    if(fabs(qMr[2][0]) > 0.999999)
    {
        pAr[0] = 0.0;
        pAr[1] = PI / 2.0;
        pAr[2] = atan2( qMr[0][1], qMr[1][1]);
    }
    else
    {
        pAr[0] = atan2( qMr[1][0], qMr[0][0]);
        dTemp  = sqrt(qMr[0][0] * qMr[0][0] + qMr[1][0] * qMr[1][0]);
        pAr[1] = atan2(-qMr[2][0], dTemp);
        pAr[2] = atan2( qMr[2][1], qMr[2][2]);
    }
    
    return;
}

/* -----------------------------------------------------------------------------*/
/********************************************************************************/
