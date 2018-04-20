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
#ifndef AUTOLIB_H
#define AUTOLIB_H

#include "robot.h"

#define ERROR_LABEL        0xFFFF
#define ERROR_NEXT         0xFFFE
#define ERROR_PREV         0xFFFD

typedef struct
{
    u32 uStk[0x80];
    u32 uTop;  
}ip;


/* ��ת��Label */
extern u16 AutoGoto(op *pOP, u32 Label);

/* ��ת��һ�� */
extern u16 AutoNext(op *pOP, u32 uIP);

/* ��ת��һ�� */
extern u16 AutoPrev(op *pOP, u32 uIP);

/* �ӳ�����ת */
extern u16 AutoCall(u8 sSub, ip *pIp);

/* �ӳ��򷵻� */
extern u16 AutoRet(ip *pIp);

#endif
