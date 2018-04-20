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
#include "get_data.h"
#include "var.h"
#include "get_var.h"


static void GetVarDat(mdl *pDatMdl, char *pData, u32 *pCrn, char *pLine, u32 *pLBuf);
static void SaveVar2DatBuf(mdl *pDatMdl, char *DatBuf, u32 *Crn, void *DatTyp, size_t DatSize, u16 VarTyp);

varaddress xVarAdd;

extern void ReadDatFile(vm *pVM)
{
    char   line[512];/* 读取到的行存放在这 */
    u32    pLine;
    mdl   *pDatMdl  = &(pVM->aModule[0]);
    char  *pCode    =   pVM->xCode.aCode;
    char  *pData    =   pVM->xData.aData;
    u32    FileSize =   pVM->aModule[0].nSize;
    u32    pBuff    =   pVM->aModule[0].nStart;/* 第一个文件存放数据,属于从0开始,不需要做偏移 */

    while(pBuff < FileSize){
        /* 读取一行到line的缓冲区里面 */
        for(pLine = 0;*(pCode + pBuff) != '\n';pLine++,pBuff++){
            line[pLine] = *(pCode + pBuff);
        }
        pBuff++;/* 这个跳过换行符 */

        for(pLine = 0;;){
            /* 跳过每一行的前面的空格 */
            for(;*(line + pLine) == ' ';pLine++){}

            /* 获取数据 */
            if(CmpStr(line,&pLine,DECL)){
                GetVarDat(pDatMdl,pData,&(pVM->xData.nCrn),line,&pLine);/* 数据取值 */
                break;
            }
            else{
                break;
            }
        }
    }
}

/* 函数说明:获取数据文件里面的变量
 * 参   数:@pData:数据缓冲区的头地址
 *         @pCrn:数据缓冲区的当前地址
 *         @pLine:行的头指针
 *         @pLBuf:行的偏移指针
 * 返 回 值:无
*/
static void GetVarDat(mdl *pDatMdl, char *pData, u32 *pCrn, char *pLine, u32 *pLBuf)
{
    char cKeyWord[32] = "";
    for(int i = 0;*(pLine + *pLBuf) != ' ';i++,(*pLBuf)++)
        cKeyWord[i] = *(pLine + *pLBuf);

    if(!strcmp(cKeyWord,PDAT_)){
        GetPdatVar(&data_pdat, pLine, pLBuf);
        SaveVar2DatBuf(pDatMdl, pData, pCrn, &data_pdat, SizePdat, VAR_PDAT);
    }
    else if(!strcmp(cKeyWord, FDAT_)){
        GetFdatVar(&data_fdat, pLine, pLBuf);
        SaveVar2DatBuf(pDatMdl, pData, pCrn, &data_fdat, SizeFdat, VAR_FDAT);
    }
    else if(!strcmp(cKeyWord, LDAT_)){
        GetLdatVar(&data_ldat, pLine, pLBuf);
        SaveVar2DatBuf(pDatMdl, pData, pCrn, &data_ldat, SizeLdat, VAR_LDAT);
    }
    else if(!strcmp(cKeyWord, E6POS_)){
        GetE6posVar(&data_e6pos, pLine, pLBuf);
        SaveVar2DatBuf(pDatMdl, pData, pCrn, &data_e6pos, SizeE6pos, VAR_E6POS);
    }
    else if(!strcmp(cKeyWord, TQM_TQDAT_T)){
        GetTqmdatVar(&data_tqmdat, pLine, pLBuf);
        SaveVar2DatBuf(pDatMdl, pData, pCrn, &data_tqmdat, SizeTqmdat, VAR_TQMDAT);
    }
    else{/* 找不到的数据类型,跳过本行 */
        return;
    }
}

/* 函数说明:将获取到的数据存放到VM中的数据缓冲区
 * 参   数:@pDatMdl:数据模块的地址
 *        @DatBuf:数据缓冲区的头指针
 *        @Crn:数据缓冲区的当前地址
 *        @DatTyp:要存储的数据类型
 *        @DatSize：数据大小
 * 返 回 值:无
 * 特别说明:存储方式为var类型
*/
static void SaveVar2DatBuf(mdl *pDatMdl, char *DatBuf, u32 *Crn, void *DatTyp, size_t DatSize, u16 VarTyp)
{

    var  *pVarCrn,*pVarPrv;
    void *pVoidDat;

    pVarCrn  = (var *)(DatBuf + *Crn);
    pVoidDat = &(pVarCrn->fData);

    memcpy(pVoidDat, DatTyp, DatSize);

    switch (VarTyp) {
    case VAR_PDAT:
        if(xVarAdd.xPdatAdd.flag == true){/* 已经有数据了 */
            pVarPrv = (var *)(DatBuf + xVarAdd.xPdatAdd.CrnAdd);   /* 上一个数据 */
            pVarPrv->uNxt            = *Crn;                       /*  */
            xVarAdd.xPdatAdd.CrnAdd  = *Crn;                       /* 更新当前的最新地址 */
        }
        else{
            xVarAdd.xPdatAdd.CrnAdd  = *Crn;
            pDatMdl->StartPdat       = *Crn;
            xVarAdd.xPdatAdd.flag    = true;
        }
        pVarCrn->pName = ((pdat *)pVoidDat)->cName;
        break;
    case VAR_LDAT:
        if(xVarAdd.xLdatAdd.flag == true){
            pVarPrv = (var *)(DatBuf + xVarAdd.xLdatAdd.CrnAdd);
            pVarPrv->uNxt            = *Crn;
            xVarAdd.xLdatAdd.CrnAdd  = *Crn;
        }
        else{
            xVarAdd.xLdatAdd.CrnAdd  = *Crn;
            pDatMdl->StartLdat       = *Crn;
            xVarAdd.xLdatAdd.flag    = true;
        }
        pVarCrn->pName = ((ldat *)pVoidDat)->cName;
        break;
    case VAR_FDAT:
        if(xVarAdd.xFdatAdd.flag == true){
            pVarPrv = (var *)(DatBuf + xVarAdd.xFdatAdd.CrnAdd);
            pVarPrv->uNxt            = *Crn;
            xVarAdd.xFdatAdd.CrnAdd  = *Crn;
        }
        else{
            xVarAdd.xFdatAdd.CrnAdd  = *Crn;
            pDatMdl->StartFdat       = *Crn;
            xVarAdd.xFdatAdd.flag    = true;
        }
        pVarCrn->pName = ((fdat *)pVoidDat)->cName;
        break;
    case VAR_E6POS:
        if(xVarAdd.xE6posAdd.flag == true){
            pVarPrv = (var *)(DatBuf + xVarAdd.xE6posAdd.CrnAdd);
            pVarPrv->uNxt             = *Crn;
            xVarAdd.xE6posAdd.CrnAdd  = *Crn;
        }
        else{
            xVarAdd.xE6posAdd.CrnAdd  = *Crn;
            pDatMdl->StartE6pos       = *Crn;
            xVarAdd.xE6posAdd.flag    = true;
        }
        pVarCrn->pName = ((e6pos *)pVoidDat)->cName;
        break;
    case VAR_TQMDAT:
        if(xVarAdd.xTqmdatAdd.flag == true){
            pVarPrv = (var *)(DatBuf + xVarAdd.xTqmdatAdd.CrnAdd);
            pVarPrv->uNxt             = *Crn;
            xVarAdd.xTqmdatAdd.CrnAdd = *Crn;
        }
        else{
            xVarAdd.xTqmdatAdd.CrnAdd = *Crn;
            pDatMdl->StartTqmdat      = *Crn;
            xVarAdd.xTqmdatAdd.flag   = true;
        }
        pVarCrn->pName = ((tqmdat *)pVoidDat)->cName;
        break;
    default:
        break;
    }

    *Crn += (DatSize + SizeVar);
}

/* 函数说明:
 * 参   数:@pDatMdl:数据模块
 *        @pDatBuf:数据缓冲区
 *        @VarName:数据名字
 *        @DatTyp:数据类型
 * 返 回 值:搜索到变量在数据缓冲区的地址
*/
extern u32  ProgramGetDat(mdl *pDatMdl,char *pDatBuf, char *VarName, u16 DatTyp)
{
    u32    VarAddress;
    var   *pSearchVar;

    switch (DatTyp) {
    case VAR_PDAT:
        VarAddress = pDatMdl->StartPdat;
        break;
    case VAR_LDAT:
        VarAddress = pDatMdl->StartLdat;
        break;
    case VAR_FDAT:
        VarAddress = pDatMdl->StartFdat;
        break;
    case VAR_E6POS:
        VarAddress = pDatMdl->StartE6pos;
        break;
    case VAR_TQMDAT:
        VarAddress = pDatMdl->StartTqmdat;
        break;
    default:
        VarAddress = pDatMdl->StartOther;
        break;
    }

    for(;VarAddress < 0xFFFFFFFF;VarAddress = pSearchVar->uNxt){
        pSearchVar = (var *)(pDatBuf + VarAddress);

        if(pSearchVar->pName == NULL)            return 0xFFFFFFFF;
        if(!strcmp(pSearchVar->pName,VarName))   return VarAddress;
        if(pSearchVar->uNxt == NULL)             return 0xFFFFFFFF;
    }

    return 0xFFFFFFFF;/* 找不到数据 */
}
