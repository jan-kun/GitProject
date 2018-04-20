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
/******************************************************************************/
#include <Protocol/linuxtypes.h>

#include <string.h>
#include <Protocol/bus.h>
#include <math.h>

#include "error.h"
#include "robot.h"
#include "IO.h"
#include "var.h"

#include "vm.h"
#include "auto.h"

#include "Protocol/RobConvert.h"

/* 轮廓插补标识 */
#define ITP_READY     0x0

#define ITP_ARC_MID   0x1
#define ITP_ARC_END   0x2

#define ITP_SPL_P1    0x4
#define ITP_SPL_P2    0x5
#define ITP_SPL_P3    0x6
#define ITP_SPL_P4    0x7
#define ITP_SPL_P5    0x8

#define Ang2Rad       0.0174532925
#define Ang2RadHT     1745.3292519

u32   uRunState = RUN_STATE_STOP;

/*----------------------------------------------------------------------------*/
static u32  bFlagTimer = 0;         /* 延时标识 */
static u8   uFlagItp   = ITP_READY;
static u16  uLineCmp   = 0;

static void GetTrgPoint(psd *pPsd, double *pTrgPoint);
static void DataConver(nckt *pNckt,sys *pSys);

/*----------------------------------------------------------------------------*/
/*操作码----------------------------------------------------------------------*/
/*机器人动作------------------------------*/
static u16 OpMovJ  (nckt *, sys *, psd *);  //OPR_MOVJ
static u16 OpMovL  (nckt *, sys *, psd *);  //OPR_MOVL
static u16 OpMovC  (nckt *, sys *, psd *);  //OPR_MOVC
static u16 OpMovS  (nckt *, sys *, psd *);  //OPR_MOVS
static u16 OpAIO   (nckt *, sys *, psd *);  //OPR_AIO
static u16 OpDIO   (nckt *, sys *, psd *);  //OPR_DIO
static u16 OpPls   (nckt *, sys *, psd *);  //OPR_PLS
static u16 OpTim   (nckt *, sys *, psd *);  //OPR_TIM
static u16 OpCnt   (nckt *, sys *, psd *);  //OPR_CNT
static u16 OpIntr  (nckt *, sys *, psd *);  //OPR_INTR
static u16 OpSys   (nckt *, sys *, psd *);  //OPR_SYS
/*程序流程--------------------------------*/
static u16 OpPrg   (nckt *, sys *, psd *);  //OPR_PRG
static u16 Opline  (nckt *, sys *, psd *);  //OPR_LINE
static u16 OpLbl   (nckt *, sys *, psd *);  //OPR_LBL
static u16 OpCall  (nckt *, sys *, psd *);  //OPR_CALL
static u16 OpIf    (nckt *, sys *, psd *);  //OPR_IF
static u16 OpSwch  (nckt *, sys *, psd *);  //OPR_SWCH
static u16 OpFor   (nckt *, sys *, psd *);  //OPR_FOR
static u16 OpWhl   (nckt *, sys *, psd *);  //OPR_WHL
static u16 OpWait  (nckt *, sys *, psd *);  //OPR_WAIT
static u16 OpJmp   (nckt *, sys *, psd *);  //OPR_JMP

/*-------------------------------------------------------------------------------*/
static u16  (*OpToNckt[0x20])(nckt *pNckt, sys *pSys, psd *pPsd);

/******************************************************************************/
void InitAuto(sys *pSys)
{
    int i;
    for(i = 0; i < 0x20; i++) OpToNckt[i] = NULL;

/*操作码----------------------------------------------------------------------*/
    OpToNckt[OP_MOVJ  & 0x0ff] = OpMovJ;  //OP_MOVJ
    OpToNckt[OP_MOVL  & 0x0ff] = OpMovL;  //OP_MOVL
    OpToNckt[OP_MOVC  & 0x0ff] = OpMovC;  //OP_MOVC
    OpToNckt[OP_MOVS  & 0x0ff] = OpMovS;  //OP_MOVS
    OpToNckt[OP_MOVAJ & 0x0ff] = OpMovS;  //OP_MOVS
    OpToNckt[OP_AIO   & 0x0ff] = OpAIO;   //OP_AIO
    OpToNckt[OP_DIO   & 0x0ff] = OpDIO;   //OP_DIO
    OpToNckt[OP_PLS   & 0x0ff] = OpPls;   //OP_PLS
    OpToNckt[OP_TIM   & 0x0ff] = OpTim;   //OP_TIM
    OpToNckt[OP_CNT   & 0x0ff] = OpCnt;   //OP_CNT
    OpToNckt[OP_INTR  & 0x0ff] = OpIntr;  //OP_INTR
    OpToNckt[OP_SYS   & 0x0ff] = OpSys;   //OP_SYS
/*程序流程--------------------------------*/
    OpToNckt[OP_PRG   & 0x0ff] = OpPrg;   //OP_PRG
    OpToNckt[OP_LINE  & 0x0ff] = Opline;  //OP_LINE
    OpToNckt[OP_LBL   & 0x0ff] = OpLbl;   //OP_LBL
    OpToNckt[OP_CALL  & 0x0ff] = OpCall;  //OP_CALL
    OpToNckt[OP_IF    & 0x0ff] = OpIf;    //OP_IF
    OpToNckt[OP_SWCH  & 0x0ff] = OpSwch;  //OP_SWCH
    OpToNckt[OP_FOR   & 0x0ff] = OpFor;   //OP_FOR
    OpToNckt[OP_WHL   & 0x0ff] = OpWhl;   //OP_WHL
    OpToNckt[OP_WAIT  & 0x0ff] = OpWait;  //OP_WAIT
    OpToNckt[OP_JMP   & 0x0ff] = OpJmp;   //OP_JMP

    bFlagTimer = 0;
    uFlagItp   = ITP_READY;

    return;
}

void InitLine(sys *pSys)
{
    bFlagTimer = 0;
    uFlagItp   = ITP_READY;

    pSys->iRun   = SYS_RUN_AUTO;
    pSys->iSdNum = 0;
}

/******************************************************************************/
/*执行一行程序，一行程序有多个指令*/
u16 PsdToNckt(nckt *pNckt, sys *pSys, psd *pPsd)
{
    u16     uRet = AUTO_OK;
    u16     uOp = pPsd->uOp;
    u16     uPrior = uOp >> 8;
    int     i;

    InitLine(pSys);

    while(uOp != OP_NULL)
    {
        if (uPrior == 0)
        {
            uRet = OpToNckt[uOp & 0xFF](pNckt, pSys, pPsd);
        }//  else{}/*其它后面再说*/

        pPsd++;
        uOp = pPsd->uOp;
        uPrior = uOp >> 8;
    }

    DataConver(pNckt, pSys);

    return uRet;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/*机器人动作------------------------------*/
static u16 OpMovJ(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_MOVJ
{
    u32    vel;

    if(uFlagItp != ITP_READY) return ERR_AUTO_ITP;

    switch(pPsd->uSub)
    {
    case OPMOV_VEL:
        vel = pPsd->uData * 80;
        NcktCmd(pNckt, NCK_ITP, NCK_ITP_VJNT, vel);
        break;
    case OPMOV_END:
        GetTrgPoint(pPsd, TrgPoint);
        NcktCmd(pNckt, NCK_MOV, NCK_MOVJ,NCK_DATA_RECT);
        pSys->iSdNum = 1;
        break;
    case OPMOV_TOOL:
        pSys->iTcr = pPsd->uData;
        break;
    case OPMOV_WOBJ:
        pSys->iUcr = pPsd->uData;
        break;
    }

   return AUTO_OK;
}

static u16 OpMovL(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_MOVL
{
    u32    vel;
    u32    zone;
    int    i,j;
    double MidTool[7];

    if(uFlagItp != ITP_READY) return ERR_AUTO_ITP;
    switch(pPsd->uSub)
    {
    case OPMOV_VEL:
        break;
    case OPMOV_END:
        break;
    case OPMOV_ZONE:
        break;
    case OPMOV_TOOL:
        break;
    case OPMOV_WOBJ:
        break;
    }

    return AUTO_OK;
}

static u16 OpMovC(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_MOVC
{
    u32    vel;
    u32    zone;
    int    i,j;
    double MidTool[7];

    if(uFlagItp != ITP_READY) return ERR_AUTO_ITP;

    switch(pPsd->uSub)
    {
    case OPMOV_VEL:
        break;
    case OPMOV_END:/* 第二个参数 */
        break;
    case OPMOV_MID:/* 第一个参数 */
        break;
    case OPMOV_ZONE:
        break;
    case OPMOV_TOOL:
        break;
    case OPMOV_WOBJ:
        break;
    }

    return AUTO_OK;
}

static u16 OpMovS(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_MOVS/OPR_MOVAJ
{
    u32    vel;
    u32    zone;
    int    i,j;
    double MidTool[7];

    if(uFlagItp != ITP_READY) return ERR_AUTO_ITP;

    switch(pPsd->uSub)
    {
    case OPMOV_VEL:
        break;
    case OPMOV_END:
        break;
    case OPMOV_ZONE:
        break;
    case OPMOV_TOOL:
        break;
    case OPMOV_WOBJ:
        break;
    }

    return AUTO_OK;
}

static u16 OpAIO(nckt *pNckt, sys *pSys, psd *pPsd)  // POP_IO_AOUT
{
//?    NcktCmd(pNckt, NCK_IO, NCK_AI, pPsd->uData + APP_IO_ADDR);

//?    NcktCmd(pNckt, NCK_IO, NCK_AO, pPsd->uData + APP_IO_ADDR);

    return AUTO_OK;
}

static u16 OpDIO(nckt *pNckt, sys *pSys, psd *pPsd)  // OprDIO
{

//    NcktCmd(pNckt, NCK_IO, NCK_DI, pPsd->uData + APP_IO_ADDR);

    NcktCmd(pNckt, NCK_IO, NCK_DO, pPsd->uData);
    pSys->iSdNum = 1;

    return AUTO_OK;
}

static u16 OpPls(nckt *pNckt, sys *pSys, psd *pPsd)  // POP_IO_PLUSE
{

  return AUTO_OK;
}

static u16 OpTim(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_TIM
{
    NcktCmd(pNckt, NCK_TIMER, NULL, pPsd->uData);
    pSys->iSdNum = 1;

    return AUTO_OK;
}

static u16 OpCnt(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_CNT
{

  return AUTO_OK;
}

static u16 OpIntr(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_INTR
{

  return AUTO_OK;
}

static u16 OpSys(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_SYS
{

  return AUTO_OK;
}

/*程序流程--------------------------------*/
static u16 OpPrg(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_PRG
{

    if(pPsd->uSub == OPPRG_END)
    {
       NcktCmd(pNckt, NCK_END, NULL, NULL);
    }

    return AUTO_OK;
}


static u16 Opline(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_LINE
{
    uLineCmp = (u16)(pPsd->uData);

    NcktCmd(pNckt, NCK_LINE, 0, pPsd->uData);

    return AUTO_OK;
}

static u16 OpLbl(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_LBL
{

  return AUTO_OK;
}

static u16 OpCall(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_CALL
{

  return AUTO_OK;
}

static u16 OpIf(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_IF
{

  return AUTO_OK;
}

static u16 OpSwch(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_SWCH
{

  return AUTO_OK;
}

static u16 OpFor(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_FOR
{

  return AUTO_OK;
}

static u16 OpWhl(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_WHL
{

  return AUTO_OK;
}

static u16 OpWait(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_WAIT
{
    static u32 uWait;
    static u16 uIOaddr;
    static u16 uIOval;

    if(pSys->iNckBuf != 5) return AUTO_FLOW;

    if(bFlagTimer == 0)
    {
        uIOaddr = (pPsd->iData & 0xFFFF) + APP_IO_ADDR;    /* 0: 端口 地址 */
        uIOval  = (pPsd->iData >> 16) & 0xFFFF;            /* 0: 端口 状态 */

        pPsd++;              /* 1: 等待时间  */

//        uWait = pPsd->iData * 0.04;
        bFlagTimer = 1;
    } else uWait--;

    if((uIOval == IO_Get(ioDIc, uIOaddr)) || (uWait == 0))
    {
        bFlagTimer = 0;

        return AUTO_OK;
    }

    return AUTO_FLOW;
}

static u16 OpJmp(nckt *pNckt, sys *pSys, psd *pPsd)  // OPR_JMP
{

  return AUTO_OK;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void  RunState(void)
{
    if(xSys.iRun == SYS_RUN_STOP)     uRunState = RUN_STATE_STOP;
    else                              uRunState = RUN_STATE_RUN;
    if(IO_Get(ioDOc, NCK_DO_HOLD))    uRunState = RUN_STATE_PAUSE;
    if(IO_Get(ioDIc, NCK_DI_AA0)
    && IO_Get(ioDIc, NCK_DI_AA1)
    && IO_Get(ioDIc, NCK_DI_AA2)
    && IO_Get(ioDIc, NCK_DI_AA3)
    && IO_Get(ioDIc, NCK_DI_AA4)
    &&IO_Get(ioDIc, NCK_DI_AA5)==0)   uRunState = RUN_STATE_ALM;
    if(IO_Get(ioDIc, NCK_DI_EMG)==0)  uRunState = RUN_STATE_EMG;
}
/******************************************************************************/

/******************************************************************************/

static void GetTrgPoint(psd *pPsd, double *pTrgPoint)
{
    pTrgPoint[0] = ((e6pos *)(pPsd->pData))->dx;
    pTrgPoint[1] = ((e6pos *)(pPsd->pData))->dy;
    pTrgPoint[2] = ((e6pos *)(pPsd->pData))->dz;

    pTrgPoint[3] = ((e6pos *)(pPsd->pData))->da;
    pTrgPoint[4] = ((e6pos *)(pPsd->pData))->db;
    pTrgPoint[5] = ((e6pos *)(pPsd->pData))->dc;
}

static void DataConver(nckt *pNckt,sys *pSys)
{
    for(int i = 0; i < 6; i++){
        pNckt->aAxis[i] = TrgPoint[i] * 100000;
    }
    return;
}
