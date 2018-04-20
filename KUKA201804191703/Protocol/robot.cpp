/*******************************************************************************
 *                   	       GPCNC
 *           The General-Purposed CNC Software Develop Environment
 *           (c) Copyright 2004-2013, Automation Depatment of NJIT
 *                         All Rights Reserved
 *
 *-----------------------文件信息-----------------------------------------------
 * 文件名: robot.c
 * 描  述:
 *
 * 创建人: 周伯荣
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

#include "Protocol/linuxtypes.h"
#include <string.h>
#include <unistd.h>
#include "Protocol/bus.h"

#include "IO.h"
#include "vm.h"
#include "auto.h"
#include "robot.h"
#include "data.h"
#include "error.h"
#include "RobConvert.h"
vct CurPoint;

/***********************************************************************************/
/*--------------------------------------------------------------------------------*/
ucr     aUser[24];      /* 用户坐标系           */
tcr     aTool[24];      /* 刀具坐标系           */
tcr     aPhy[24];       /* 工具坐标系物理参数   */

sys     xSys;           /* 系统参数             */
//nckt    xImdT;          /* 立即发送包           */
//nckt    xBufT[3];       /* 缓冲发送包           */
nckr    xBufR;          /* 缓冲接受包           */

sImdT   xImdT;          /* 立即发送包           */
sBufT   xBufT;          /* 缓冲发送包           */
sNulT   xNulT;          /* 空数据发送包         */

nckt    xMidT;          /*  */

/*--------------------------------------------------------------------------------*/
flag    xFlag;          /* 标识                 */

ub      xUserBuf[8];     /*  */
tb      xToolBuf[8];     /*  */

double  TrgPoint[6];

/**********************************************************************************/

extern void InitNckT(nckt *pNckt)
{
    int i;
    for(i = 0; i < 8; i++)
    {
        pNckt->uCmd[i] = 0;
    }

    return;
}

extern void InitSys(sys *pSys)
{

    pSys->iFeed         = 50;             /* 手动速度         */
//    pSys->iCyc          = SYS_CYC_CYCL;
    pSys->iRun          = SYS_RUN_STOP;   /* 执行的状态       */
    pSys->iCoor         = SYS_COOR_J;
    pSys->iTry          = SYS_TRY_NONE;
    pSys->iUcr          = 0;              /* 坐标系          */
    pSys->iTcr          = 0;              /* 刀具            */
    pSys->iNckBuf       = 5;
    pSys->iDecode       = SYS_DECODE_NOT; /* 译码            */
    pSys->iChooseF      = SYS_CHOOSE_NOT;
    pSys->iSdNum        = 0;
    pSys->iCdMod        = SYS_RECORD_CYCL;/* 循环模式         */
    pSys->iPressStop    = SYS_STOP_NOT;
    pSys->iStatus       = SYS_STATUS_STOP;

    pSys->xArm.aAxis[0] = 290000;
    pSys->xArm.aAxis[1] = 0;
    pSys->xArm.aAxis[2] = 270000;
    pSys->xArm.aAxis[3] = 70000;
    pSys->xArm.aAxis[4] = 302000;
    pSys->xArm.aAxis[5] = 72000;
    pSys->xArm.aAxis[6] = 0;
    pSys->xArm.aAxis[7] = 0;
}

extern void InitFlag(flag *pFlag)
{
    pFlag->vRef   = 0;
    pFlag->vRbt   = 0;
    pFlag->vMode  = 0;
    pFlag->vTool  = 0;
    pFlag->vUser  = 0;
    pFlag->vCoor  = 0;
    pFlag->vBrand = 4;
    return;
}

/********************************************************************************/
extern void RunCode(void)
{
    u16    uRet = AUTO_OK;
    i32    iCoor[6];
    double iqt[7];

    switch(xSys.iRun)
    {
    case SYS_RUN_AUTO:
    case SYS_RUN_FLOW:
        if(xSys.iNckBuf)     Decode();/* 译码操作 */
        switch(uRet)
        {
        case  AUTO_END:
            xSys.iRun = SYS_RUN_WAIT;
            break;
        case  AUTO_FLOW:
            xSys.iRun = SYS_RUN_FLOW;
            break;
//        case  AUTO_CIR:
//            xSys.iRun = SYS_RUN_AUTO;
//            break;
        case  AUTO_ERR:
            xSys.iRun = SYS_RUN_STOP;
            break;
        default:
         switch(xSys.iMode)
            {
            case SYS_MODE_PLAY:
                if(xSys.iCyc == SYS_CYC_STEP)   xSys.iRun = SYS_RUN_WAIT;
                else                            xSys.iRun = SYS_RUN_AUTO;
                break;
            case SYS_MODE_TECH:
                if(xSys.iCyc == SYS_CYC_STEP)   xSys.iRun = SYS_RUN_WAIT;
                else                            xSys.iRun = SYS_RUN_AUTO;
                break;
            }
        }
        break;
    case SYS_RUN_WAIT:
        if(xSys.iMode == SYS_MODE_TECH)
        {
            if(xSys.iNckBuf == 5)
            {
                xSys.iRun = SYS_RUN_STOP;
            }
        }
        break;
    }

    switch(xSys.iRun)
    {
        case SYS_RUN_STOP:
        case SYS_RUN_READY:
            if(xFlag.vRef == 1)
            {
                xImdT.aImdT.cNck = NCK_IMD;
                NcktCmd(&(xImdT.aImdT), NCK_REF, NCK_REF_SET, 0);
                memcpy(&(xImdT.aImdT.aAxis), &(xSys.xRef), sizeof(vct));
                xImdT.flag = 1;
            }
            else if(xFlag.vRbt  == 1)
            {
                xImdT.aImdT.cNck = NCK_IMD;
                NcktCmd(&(xImdT.aImdT), NCK_SET, NCK_SET_RBT, 0);
                memcpy(&(xImdT.aImdT.aAxis), &(xSys.xArm), sizeof(vct));
                xImdT.flag = 1;
            }
            else if(xFlag.vVer  == 1)
            {
                xImdT.aImdT.cNck = NCK_IMD;
                NcktCmd(&(xImdT.aImdT), NCK_GET, NCK_GET_VER, 2);
                xImdT.flag = 1;
            }
            else if(xFlag.vTool == 1)
            {
               iqt[0] = 0;
               iqt[1] = 0;
               iqt[2] = 0;
               iqt[3] = 0;
               iqt[4] = 0;
               iqt[5] = 0;
               iqt[6] = 0;
               Quat2Eula((int *)iCoor,iqt,1000);

               xImdT.aImdT.cNck = NCK_IMD;
               NcktCmd(&(xImdT.aImdT), NCK_SET, NCK_SET_TOOL, 0);
               memcpy(&(xImdT.aImdT.aAxis), iCoor, sizeof(vct));
               xImdT.flag = 1;
            }
            else if(xFlag.vUser == 1)
            {
                iqt[0] = 0;
                iqt[1] = 0;
                iqt[2] = 0;
                iqt[3] = 1;
                iqt[4] = 0;
                iqt[5] = 0;
                iqt[6] = 0;
                Quat2Eula((int *)iCoor,iqt,1000);

                xImdT.aImdT.cNck = NCK_IMD;
                NcktCmd(&(xImdT.aImdT), NCK_SET, NCK_SET_USER, 0);
                memcpy(&(xImdT.aImdT.aAxis),iCoor, sizeof(vct));
                xImdT.flag = 1;
             }
             else if(xFlag.vCoor == 1)
             {
                 xImdT.aImdT.cNck = NCK_IMD;
                 NcktCmd(&(xImdT.aImdT), NCK_COOR, (u8)(xSys.iCoor), 0);
                 xImdT.flag = 1;
             }
             else if(xFlag.vMode == 1)
             {
                 xImdT.aImdT.cNck = NCK_IMD;
                 NcktCmd(&(xImdT.aImdT), NCK_MODE, (u8)(xSys.iMode), 0);
                 xImdT.flag = 1;
             }
             else if(xFlag.vBrand == 1)
             {
                xImdT.aImdT.cNck = NCK_IMD;
                NcktCmd(&(xImdT.aImdT), NCK_SET, NCK_SET_BRAND, 3);
                xImdT.flag = 1;
             }
             break;
    }
}

/********************************************************************************/
void BusNckT(void)
{
//    if(pNckTx) api_NckTx();
}

/********************************************************************************/
/*处理接受缓冲区*/
u32 NckrCmd(u8 uCmd, u8 uSub, u32 uData)
{
    int  i;
    static u32  nMod, nMid;
    u32  nLine;

    switch(uCmd)
    {
    case NCK_LINE:
        break;
    case NCK_MODE:
        xFlag.vMode = 0;
        break;
    case NCK_SET:
        switch(uSub)
        {
        case NCK_SET_TOOL:
            xFlag.vTool = 0;
            break;
        case NCK_SET_USER:
            xFlag.vUser = 0;
            break;
        case NCK_SET_RBT:
            xFlag.vRbt = 0;
            break;
        case NCK_SET_BRAND:
            xFlag.vBrand = 0;
            break;
        }
        break;
    case NCK_COOR:
        xFlag.vCoor = 0;
        break;
    case NCK_REF:
        switch(uSub)
        {
        case NCK_REF_SET:
            xFlag.vRef = 0;
            break;
        case NCK_REF_GET:
            xFlag.vRef = 0;
            memcpy(&(xSys.xRef), &(pNckRx->aAxis), sizeof(vct));
            return 0;
        }
        break;
    case NCK_GET:
        if(uSub==NCK_GET_VER)
        {
            xFlag.vVer =0;
            xSys.uVer=(u16)uData;
        }
       break;
    case NCK_END:

        break;
    case NCK_ERROR:
//        ErrHandle[uData](uData);
        break;
    }

    return 1;
}

/*------------------------------------------------------------------------------*/
extern void BusNckR(nckr *pNckRx)
{
    u32 i, j = 1;
    u8  uMode0,uMode1;

    memcpy(ioDIp, ioDIc, 20);
    memcpy(ioDIc, pNckRx->aDI, 20);
    memcpy(ioDOp, ioDOc, 20);
    memcpy(ioDOc, pNckRx->aDO, 20);

    uMode0 = IO_Get(ioDIc, NCK_DI_MODE0);
    uMode1 = IO_Get(ioDIc, NCK_DI_MODE1);

    if(uMode0 == 0 && uMode1 == 0){
        xSys.iMode = SYS_MODE_TECH_LOW;
    }
    else if(uMode0 != 0){
        xSys.iMode = SYS_MODE_TECH_FULL;
    }
    else if(uMode1 != 0){
        xSys.iMode = SYS_MODE_AUTO;

    }

    memcpy(CurPoint.aAxis , pNckRx->aAxis , sizeof(i32)*6);

    xSys.iNckBuf = pNckRx->cNck;
    for(i = 0; i < 4; i++)
    {
        if(pNckRx->uCmd[i] == 0) break;
        j = NckrCmd(pNckRx->uCmd[i], pNckRx->uSub[i], pNckRx->uData[i]);
    }
    if(j)
        memcpy(&(xSys.xV20), pNckRx->aAxis, sizeof(v20));

    return;
}

/********************************************************************************/
/*------------------------------------------------------------------------------*/
extern void NcktCmd(nckt *pNckt, u8 uCmd, u8 uSub, u32 uData)
{
    int i;
    for(i = 0; i < 8; i++)
    {
        if(pNckt->uCmd[i] == 0)
        {
            pNckt->uCmd[i]  = uCmd;
            pNckt->uSub[i]  = uSub;
            pNckt->uData[i] = uData;
            return;
        }
    }

    return;
}

/*------------------------------------------------------------------------------*/
extern void NcktVct(nckt *pNckt, vct *pVct)
{
   vct *pVctT= (vct *)(pNckt->aAxis);
   memcpy(pVctT, pVct, sizeof(vct));
   return;
}

/*------------------------------------------------------------------------------*/
extern u8 DataFlag(v8 *pFlag)
{

    *pFlag = 1;

    while(true){
        if(*pFlag == 0)
            return 0;
    }

    return 1;
}

extern int InitCTL(void)
{
    /* <- 数据初始化, 开始发送数据->  */
    while(DataFlag(&xFlag.vBrand));
    while(DataFlag(&xFlag.vVer));
    while(DataFlag(&xFlag.vRef));
    while(DataFlag(&xFlag.vRbt));
    while(DataFlag(&xFlag.vUser));
    while(DataFlag(&xFlag.vTool));
    return 0;
}

extern int InitHMI(void)
{
    memset(&xImdT,0,sizeof(sImdT));
    memset(&xBufT,0,sizeof(sBufT));
    memset(&xNulT,0,sizeof(sNulT));
    ioDIState.flag = 0;

    InitSys(&xSys);
    InitFlag(&xFlag);
    InitAuto(&xSys);
    InitErr();
}

/******************************************************************************/
