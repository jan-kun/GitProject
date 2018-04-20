/*******************************************************************************
 *                   	       GPCNC
 *           The General-Purposed CNC Software Develop Environment
 *           (c) Copyright 2004-2013, Automation Depatment of NJIT
 *                         All Rights Reserved
 *
 *-----------------------�ļ���Ϣ-----------------------------------------------
 * �ļ���: .c
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
 ***********************************************************************************/
/***********************************************************************************/
#include "Protocol/linuxtypes.h"

#include "error.h"
#include "rtype.h"
#include "robot.h"

#include "pop.h"
#include "autolib.h"

/*����ָ���ջ*/
u16 AutoGoto(op *pOP, u32 uLabel)
{  
    for(int i = 0; i < 2000; i++)
    {
        if(pOP->uCmd == POP_PROC_LABEL)
        {
          if(pOP->data.uData == uLabel) return i++;
        }
        pOP++; 
    }
    
    return ERROR_LABEL;
}

u16 AutoNext(op *pOP, u32 uIP)
{  
    for(; uIP < 2000; uIP++)
    {
        pOP++; 
        if(pOP->uCmd  == POP_CTRL_LINE) 
          return uIP;
    }
    
    return ERROR_NEXT;
}

u16 AutoPrev(op *pOP, u32 uIP)
{  
    int uCrn = 1;
    
    for(; uIP > 0; uIP--)
    {
        if(pOP->uCmd == POP_CTRL_LINE)
        {
          if(uCrn) uCrn = 0;
          else     return uIP;
        }
        pOP--; 
    }
    
    return ERROR_PREV;
}

/***********************************************************************************/
/***********************************************************************************/
