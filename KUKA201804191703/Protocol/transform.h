/*******************************************************************************
 *                   	       GPCNC
 *           The General-Purposed CNC Software Develop Environment
 *           (c) Copyright 2004-2013, Automation Depatment of NJIT
 *                         All Rights Reserved
 *
 *-----------------------�ļ���Ϣ-----------------------------------------------
 * �ļ���: .h
 * ��  ��: 
 *
 * ������: 
 * ��  ��: 
 * ��  ��: 
 *
 * �޸���:
 * ��  ��:
 * �ա���:
 * �޸���Ϣ:
 *
 * ˵��: 		
 *			
 *...
*******************************************************************************/
#ifndef TRANSFORM_H
#define TRANSFORM_H

/*-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------*/
typedef struct {
    double l1;              /* �����˵�����         */
    double l2;              /* s��������l�����߾��� */
    double l3;              /* ��۳���             */
    double l4;              /* u�����ߵ�r�����߾��� */
    double l5;              /* С�۳���             */
    double l6;              /* ���߳���             */
    double l7;              /* sqrt(l4*l4+l5*l5)    */
    double l8;              /* ����̨�߶�           */
    double Al5;             /* arctan(l5/l4)        */

    double dMr[3][3];       /* ĩ����̬����         */
    double dPos[6];         /* ĩ��λ��             */

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
