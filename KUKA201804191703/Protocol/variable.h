/*******************************************************************************
 *                   	       GPCNC
 *           The General-Purposed CNC Software Develop Environment
 *           (c) Copyright 2004-2013, Automation Depatment of NJIT
 *                         All Rights Reserved
 *
 *-----------------------�ļ���Ϣ-----------------------------------------------
 * �ļ���: variable.h
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
#ifndef VARIABLE_H
#define VARIABLE_H

#include "rtype.h"

/*----------------------------------------------------------------------------*/
typedef struct  /*ϵͳ����*/
{
    i32     iData;          /* ֵ                   */
    u32     uState;         /* ״̬                 */
    u8      sNote[20];      /* ע��                 */
} rsys;

/*----------------------------------------------------------------------------*/
typedef struct  /*λ�ñ���*/
{
    vct     xData;          /* ֵ                   */
    u32     uState;         /* ״̬                 */
    u8      sNote[20];      /* ע��                 */
} rpos;

/* ����-----------------------------------------------------------------------*/
extern rsys aRin[32];
extern rsys aRout[32];
//extern rsys aRdat[128];
rsys aRdat[128];
//extern rpos aRpos[128];
rpos aRpos[128];

#endif
/*-----------------end of file----------------------*/
