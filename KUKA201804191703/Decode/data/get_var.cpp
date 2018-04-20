/*******************************************************************************
 *
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
#include <stdlib.h>
#include <stdio.h>

#include "get_var.h"
#include "base_opt.h"
#include "macro.h"

/* 函数说明:获取E6POS的结构体数据
 * 参   数:@pE6pos:结构体E6POS的
 *        @pCdBuf:vm的代码头地址
 *        @pCdAdd:vm里面的代码地址
 * 返 回 值:无
*/
void GetE6posVar(e6pos *pE6pos, char *pCdBuf, u32 *pCdAdd)
{

    GetVarName(pCdBuf,pCdAdd,pE6pos->cName);

    EvaData(pCdBuf,pCdAdd); /* ={ */

    pE6pos->dx  = GetE6posValue(pCdBuf,pCdAdd,X_DEF);
    pE6pos->dy  = GetE6posValue(pCdBuf,pCdAdd,Y_DEF);
    pE6pos->dz  = GetE6posValue(pCdBuf,pCdAdd,Z_DEF);

    pE6pos->da  = GetE6posValue(pCdBuf,pCdAdd,A_DEF);
    pE6pos->db  = GetE6posValue(pCdBuf,pCdAdd,B_DEF);
    pE6pos->dc  = GetE6posValue(pCdBuf,pCdAdd,C_DEF);

    pE6pos->ds  = GetE6posValue(pCdBuf,pCdAdd,S_DEF);
    pE6pos->dt  = GetE6posValue(pCdBuf,pCdAdd,T_DEF);

    pE6pos->de1 = GetE6posValue(pCdBuf,pCdAdd,E1_DEF);
    pE6pos->de2 = GetE6posValue(pCdBuf,pCdAdd,E2_DEF);
    pE6pos->de3 = GetE6posValue(pCdBuf,pCdAdd,E3_DEF);
    pE6pos->de4 = GetE6posValue(pCdBuf,pCdAdd,E4_DEF);
    pE6pos->de5 = GetE6posValue(pCdBuf,pCdAdd,E5_DEF);
    pE6pos->de6 = GetE6posValue(pCdBuf,pCdAdd,E6_DEF);

}

/* 函数说明:获取PDAT的结构体数据
 * 参   数:@
 * 返 回 值:
*/
void GetPdatVar(pdat *pPdat, char *pCdBuf, u32 *pCdAdd)
{

    GetVarName(pCdBuf,pCdAdd,pPdat->cName);

    EvaData(pCdBuf,pCdAdd); /* ={ */

    pPdat->cVel      = GetPdatValue(pCdBuf,pCdAdd,VEL_DEF);
    pPdat->cAcc      = GetPdatValue(pCdBuf,pCdAdd,ACC_DEF);
    pPdat->sApo_dist = GetPdatValue(pCdBuf,pCdAdd,APO_DIST_DEF);
    pPdat->bApo_Mode = (bool)GetPdatValue(pCdBuf,pCdAdd,APO_MODE_DEF);

}

/* 函数说明:获取LDAT的结构体数据
 * 参   数:@
 * 返 回 值:
*/
void GetLdatVar(ldat *pLdat, char *pCdBuf, u32 *pCdAdd)
{

    GetVarName(pCdBuf,pCdAdd,pLdat->cName);

    EvaData(pCdBuf,pCdAdd); /* ={ */

    pLdat->cVel       = GetLdatValue(pCdBuf,pCdAdd,VEL_DEF);
    pLdat->cAcc       = GetLdatValue(pCdBuf,pCdAdd,ACC_DEF);
    pLdat->sApo_dist  = GetLdatValue(pCdBuf,pCdAdd,APO_DIST_DEF);
    pLdat->sApo_fac   = GetLdatValue(pCdBuf,pCdAdd,APO_FAC_DEF);
    pLdat->cAxis_vel  = GetLdatValue(pCdBuf,pCdAdd,AXIS_VEL_DEF);
    pLdat->cAxis_acc  = GetLdatValue(pCdBuf,pCdAdd,AXIS_ACC_DEF);
    pLdat->cOri_type  = GetLdatValue(pCdBuf,pCdAdd,ORI_TYP_DEF);;
    pLdat->cCirc_type = GetLdatValue(pCdBuf,pCdAdd,CIRC_TYP_DEF);;
    pLdat->cJerk_fac  = GetLdatValue(pCdBuf,pCdAdd,JERK_FAC_DEF);;
    pLdat->cGear_jerk = GetLdatValue(pCdBuf,pCdAdd,GEAR_JERK_DEF);;
    pLdat->cExax_ign  = GetLdatValue(pCdBuf,pCdAdd,EXAX_IGN_DEF);;
}

/* 函数说明:获取FDAT的结构体数据
 * 参   数:@
 * 返 回 值:
*/
void GetFdatVar(fdat *pFdat, char *pCdBuf, u32 *pCdAdd)
{

    GetVarName(pCdBuf,pCdAdd,pFdat->cName);

    EvaData(pCdBuf,pCdAdd); /* ={ */

    pFdat->cTool     = (u8)GetFdatValue(pCdBuf,pCdAdd,TOOL_NO_DEF);
    pFdat->cBase     = (u8)GetFdatValue(pCdBuf,pCdAdd,BASE_NO_DEF);
    pFdat->bExt_tcp  = (bool)GetFdatValue(pCdBuf,pCdAdd,IPO_FRAME_DEF);
    pFdat->pSupPoint = GetFdatValue(pCdBuf,pCdAdd,POINT2_DEF);
    pFdat->bTq_state = (bool)GetFdatValue(pCdBuf,pCdAdd,TQ_STATE_DEF);
}

/* 函数说明:获取TQM_TQDAT_T得数据结构体
 * 参   数:@
 * 返 回 值:
 * 特别说明:TQM_TQDAT_T为外部TCP
*/
extern void GetTqmdatVar(tqmdat *pTqmdat, char *pCdBuf, u32 *pCdAdd)
{

    GetVarName(pCdBuf,pCdAdd,pTqmdat->cName);

    EvaData(pCdBuf,pCdAdd); /* ={ */

    pTqmdat->lTxx[0]  = GetTqmdatValue(pCdBuf,pCdAdd,T11_DEF);
    pTqmdat->lTxx[1]  = GetTqmdatValue(pCdBuf,pCdAdd,T12_DEF);
    pTqmdat->lTxx[2]  = GetTqmdatValue(pCdBuf,pCdAdd,T13_DEF);
    pTqmdat->lTxx[3]  = GetTqmdatValue(pCdBuf,pCdAdd,T14_DEF);
    pTqmdat->lTxx[4]  = GetTqmdatValue(pCdBuf,pCdAdd,T15_DEF);
    pTqmdat->lTxx[5]  = GetTqmdatValue(pCdBuf,pCdAdd,T16_DEF);
    pTqmdat->lTxx[6]  = GetTqmdatValue(pCdBuf,pCdAdd,T21_DEF);
    pTqmdat->lTxx[7]  = GetTqmdatValue(pCdBuf,pCdAdd,T22_DEF);
    pTqmdat->lTxx[8]  = GetTqmdatValue(pCdBuf,pCdAdd,T23_DEF);
    pTqmdat->lTxx[9]  = GetTqmdatValue(pCdBuf,pCdAdd,T24_DEF);
    pTqmdat->lTxx[10] = GetTqmdatValue(pCdBuf,pCdAdd,T25_DEF);
    pTqmdat->lTxx[11] = GetTqmdatValue(pCdBuf,pCdAdd,T26_DEF);

    pTqmdat->lKx[0]   = GetTqmdatValue(pCdBuf,pCdAdd,K1_DEF);
    pTqmdat->lKx[1]   = GetTqmdatValue(pCdBuf,pCdAdd,K2_DEF);
    pTqmdat->lKx[2]   = GetTqmdatValue(pCdBuf,pCdAdd,K3_DEF);
    pTqmdat->lKx[3]   = GetTqmdatValue(pCdBuf,pCdAdd,K4_DEF);
    pTqmdat->lKx[4]   = GetTqmdatValue(pCdBuf,pCdAdd,K5_DEF);
    pTqmdat->lKx[5]   = GetTqmdatValue(pCdBuf,pCdAdd,K6_DEF);

    pTqmdat->lOx[0]   = GetTqmdatValue(pCdBuf,pCdAdd,O1_DEF);
    pTqmdat->lOx[1]   = GetTqmdatValue(pCdBuf,pCdAdd,O2_DEF);

    pTqmdat->lId      = GetTqmdatValue(pCdBuf,pCdAdd,ID_DEF);

    pTqmdat->lOvm     = GetTqmdatValue(pCdBuf,pCdAdd,OVM_DEF);
    pTqmdat->lTmf     = GetTqmdatValue(pCdBuf,pCdAdd,TMF_DEF);
}

/* 函数说明:搜索辅助点
 * 参   数:
 * 返 回 值:返回在数据缓冲区得地址
 * 特别说明:从数据文件得头至尾搜索,然后存储在vm得数据缓冲区中
*/
u32  SearchSupVar(u32 DatSize,char *pName,char *pCdBuf,u32 *pCdAdd)
{
    u32  pDat = 0;
    while(pDat < DatSize){
        if(CmpStr(pCdBuf,pCdAdd,DECL)){
            if(CmpStr(pCdBuf,pCdAdd,pName)){
                GetE6posVar(&data_e6pos,pCdBuf,pCdAdd);
                /* 存储数据到数据缓冲区里面 */
                return NULL;
            }
        }
        pDat++;
    }
    return NULL;
}
