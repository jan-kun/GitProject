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
#include "code2psd.h"
#include "var_init.h"

static int pPsdNum;

static bool GetSubFunc(vm *pVM, char *pSubName);
static __inline void GetKeyName(char *pKeyName, char *pLineCrn, u32 *pBuffCrn);
static __inline void __GetKeyName(char *pKeyName, char *pLineCrn, u32 *pBuffCrn, char EndofWord);
static void GetMovePara(vm *pVM, char *pLineCrn, u32 *pBuffCrn, char *pKeyWord);

static void SkipFront(vm *pVM,  u32 *CodeAddCrn);      /* 跳过开头的;FOLD   */
static void SkipSub2End(vm *pVM, u32 *CodeAddCrn);     /* 遇到DEF ....的情况 */
static bool SkipLoopCircle(vm *pVM, u32 *pCodeAddCrn);  /* 遇到了EXIT        */
static bool GotoEndLoop(vm *pVM, u32 *CodeAddCrn, wifl *pWiflCrn);
static bool ExitSub(vm *pVM);
static bool JudgeCondition(char *pLineCrn, u32 *pBuffCrn);
static bool Run2Endif(vm *pVM, u32 *CodeAddCrn, u32 *pLineNumCrn);

/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
/* 函数说明:获取模块的入口函数
 * 参   数:
 * 返 回 值:
*/
extern bool  GetMdlEnter(vm *pVM, char *pDatPath, char *pSrcPath)
{
    char  MdlEnter[32];
    int   i,j;

    InitBuff(pVM);

    for(i = strlen(pDatPath); *(pDatPath + i) != '/'; i--){}
    for(j = strlen(pDatPath); *(pDatPath + j) != '.'; j--){}

    memcpy(MdlEnter, pDatPath + ++i, j - i);/* 获取跟文件名一样的函数名 */

    ScanFile(pVM,pDatPath,pSrcPath);
    ReadDatFile(pVM);

    /* 跟文件相同的文件名为执行入口 */
    if(GetSubFunc(pVM, MdlEnter))    return true;
    else                             return false;
}

/* 函数说明:
 * 参   数:
 * 返 回 值:
*/
extern  bool GetCmd2Run(vm *pVM)
{
    char  LineCrn[512];
    u32   pLine;
    int   j;

    char  cKeyWord[32];

    char *pCodeBuf   = pVM->xCode.aCode;
    sub  *pSubCrn    = &(pVM->aSubStk[pVM->nSubCrn]);
    u32  *CodeAddCrn = &(pSubCrn->nBlck);

    while(true){

        for(j = 0;*(pCodeBuf + *CodeAddCrn) != '\n';j++,(*CodeAddCrn)++){
            LineCrn[j] = *(pCodeBuf + *CodeAddCrn);
        }
        LineCrn[j] = '\n';
        (*CodeAddCrn)++;

        for(pLine = 0;LineCrn[pLine] == ' ' || LineCrn[pLine] == '\t';pLine++){}

        if(LineCrn[pLine] == '\r' || LineCrn[pLine] == '\n'){
            CodeRecord.LineCrn++;
            break;
        }
        else if(LineCrn[pLine] == '&'){}
        else{

            GetKeyName(cKeyWord,LineCrn,&pLine);
/*----------------------------------运动指令--------------------------------------------*/
            if(!strcmp(cKeyWord, _FOLD)){

                for(;LineCrn[pLine] == ' ';pLine++){}

                GetKeyName(cKeyWord,LineCrn,&pLine);

                if(!strcmp(cKeyWord, INI)){
                    SkipFront(pVM, CodeAddCrn);/* 跳过开头  */
                    break;
                }
                else{
                    for(pPsdNum = 0;;){/* pPsdNum用于后面填充的psd数组 */
                        for(j = 0; *(pCodeBuf + *CodeAddCrn) != '\n'; j++, (*CodeAddCrn)++)
                            LineCrn[j] = *(pCodeBuf + *CodeAddCrn);
                        LineCrn[j] = '\n';
                        (*CodeAddCrn)++;

                        for(pLine = 0; LineCrn[pLine] == ' '; pLine++){}

                        GetKeyName(cKeyWord, LineCrn, &pLine);

                        if(!strcmp(cKeyWord, _ENDFOLD)){
                            CodeRecord.LineCrn++;
                            return true;
                        }
                        else
                            GetMovePara(pVM, LineCrn, &pLine, cKeyWord);
                    }/* end of for() */
                }/* end of judge INI */
            }/* end of judge FOLD */
/*-----------------------------------------------------------------------------------*/
/*-------------------------------LOOP循环指令------------------------------------------*/
            else if(!strcmp(cKeyWord, LOOP)){

                pSubCrn->uLogicCrn++;/* 入栈 */

                char *pWiflRecMdl = pSubCrn->aLogicStk[pSubCrn->uLogicCrn].pMdlCrn;  /* 当前模块 */
                char *pWiflRecSub = pSubCrn->aLogicStk[pSubCrn->uLogicCrn].pSubCrn;  /* 当前子程序 */

                pSubCrn->aLogicStk[pSubCrn->uLogicCrn].uLineStart = CodeRecord.LineCrn;
                strcpy(pWiflRecMdl, CodeRecord.pMdlCrn);
                strcpy(pWiflRecSub, CodeRecord.pSubCrn);

                pSubCrn->aLogicStk[pSubCrn->uLogicCrn].iWifl = WIFL_LOOP;
                pSubCrn->aLogicStk[pSubCrn->uLogicCrn].uAddStart = *CodeAddCrn;
            }
            else if(!strcmp(cKeyWord, ENDLOOP)){
                *CodeAddCrn = pSubCrn->aLogicStk[pSubCrn->uLogicCrn].uAddStart;

                char *pWiflRecMdl = pSubCrn->aLogicStk[pSubCrn->uLogicCrn].pMdlCrn;  /* 当前模块 */
                char *pWiflRecSub = pSubCrn->aLogicStk[pSubCrn->uLogicCrn].pSubCrn;  /* 当前子程序 */

                CodeRecord.LineCrn = pSubCrn->aLogicStk[pSubCrn->uLogicCrn].uLineStart;
                strcpy(CodeRecord.pMdlCrn, pWiflRecMdl);
                strcpy(CodeRecord.pSubCrn, pWiflRecSub);

                pSubCrn->uLogicCrn--;
            }
            else if(!strcmp(cKeyWord, EXIT)){/* 用于退出LOOP循环的 */
                if(!SkipLoopCircle(pVM, CodeAddCrn))
                    CodeRecord.LineCrn++;
                break;
            }
/*----------------------------------------------------------------------------------*/
/*--------------------------------IF判断语句-------------------------------------------*/
            else if(!strcmp(cKeyWord, IF)){

                for(; LineCrn[pLine] == ' '; pLine++){}

                __GetKeyName(cKeyWord, LineCrn, &pLine, '[');
                pLine++;

                if(!strcmp(cKeyWord, _IN)){
                    if(JudgeCondition(LineCrn, &pLine)){
                        CodeRecord.LineCrn++;
                        break;
                    }
                    else                     Run2Endif(pVM, CodeAddCrn, &(CodeRecord.LineCrn));
                }
                else                         Run2Endif(pVM, CodeAddCrn, &(CodeRecord.LineCrn));
            }
            else if(!strcmp(cKeyWord, ELSE)){
                break;
            }
            else if(!strcmp(cKeyWord, ENDIF)){
                break;
            }
/*----------------------------------------------------------------------------------*/
/*-------------------------------WHILE语句---------------------------------------------*/
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/
            else if(!strcmp(cKeyWord, DEF))
                SkipSub2End(pVM, CodeAddCrn);
            else if(!strcmp(cKeyWord, END)){/* 运行结束 */
                if(ExitSub(pVM))               return true;
                else                           return false;
            }
            else{/* 查找子函数 */
                if(GetSubFunc(pVM, cKeyWord))  return true;
                else                           return false;
            }
        }
    }

    return false;
}

/* 函数说明:获取子函数
 * 参   数:
 * 返 回 值:
*/
static bool GetSubFunc(vm *pVM, char *pSubName)
{
    mdl  *pSrcMdl    = &(pVM->aModule[1]);
    u32   pSrcStart  = pSrcMdl->nStart;
    u32   FileSize   = pSrcMdl->nSize;
    u32   SrcEndAdd  = FileSize + pSrcStart;

    char *pCode      = pVM->xCode.aCode;
    u32   pLine;
    char  Line[512];
    char  KeyWord[32];

    code2hmi  SrchSub;
    memset(&SrchSub,0,sizeof(code2hmi));
    SrchSub.LineCrn = 1;

    while (pSrcStart < SrcEndAdd) {
        for(pLine = 0; *(pCode + pSrcStart) != '\n'; pSrcStart++,pLine++)
            Line[pLine] = *(pCode + pSrcStart);
        Line[pLine] = '\n';
        pSrcStart++;

        for(pLine = 0;Line[pLine] == ' ' || Line[pLine] == '/t';pLine++){}

        if(Line[pLine] == '\r' || Line[pLine] == '\n')
            SrchSub.LineCrn++;
        else{
            GetKeyName(KeyWord,Line,&pLine);

            if(!strcmp(KeyWord,_FOLD))    SrchSub.LineCrn++;
            else if(Line[pLine] == '('){/* 跳过函数名,例如:SUB() */
                for(;Line[pLine] == ' ';pLine++){}
                if(Line[++pLine] == ')')
                    SrchSub.LineCrn++;
            }
            else if(!strcmp(KeyWord,DEF)){
                SrchSub.LineCrn++;
                for(;Line[pLine] == ' ';pLine++){}
                GetKeyName(KeyWord,Line,&pLine);
                if(!strcmp(pSubName,KeyWord)){
/*------------------保存当前的栈信息-------------------------------------------------------*/
                    if(pVM->nSubCrn >= 0){
                        sub *pSubPre = &(pVM->aSubStk[pVM->nSubCrn]);

                        pSubPre->nLineCrn = CodeRecord.LineCrn + 2;
                        strcpy(pSubPre->cMdlCrn, CodeRecord.pMdlCrn);
                        strcpy(pSubPre->cSubCrn, CodeRecord.pSubCrn);
                    }
/*------------------入栈-----------------------------------------------------------------*/
                    pVM->nSubCrn++;
/*--------------------------------------------------------------------------------------*/
                    sub *pSubCrn = &(pVM->aSubStk[pVM->nSubCrn]);

                    memcpy(SrchSub.pSubCrn,KeyWord,strlen(KeyWord));
                    memcpy(SrchSub.pMdlCrn,pVM->aModule[1].aMdl,strlen(pVM->aModule[1].aMdl));
                    memcpy(&CodeRecord,&SrchSub,sizeof(code2hmi));

                    pSubCrn->nBlck         = pSrcStart;
                    pSubCrn->nLineCrn      = CodeRecord.LineCrn;
                    strcpy(pSubCrn->cMdlCrn, CodeRecord.pMdlCrn);
                    strcpy(pSubCrn->cSubCrn, CodeRecord.pSubCrn);

                    return true;
                }
            }
        }/* end of else */
    }/* end of while() */

    /* 这里搜索所有的模块 */

    CodeRecord.LineCrn++;
    return false;
}

/* 函数说明
 * 参   数
 * 返 回 值
*/
static void GetMovePara(vm *pVM, char *pLineCrn, u32 *pBuffCrn, char *pKeyWord)
{
    char  VarName[32];
    u32   GetVarAdd;

    sub  *pSubCrn = &(pVM->aSubStk[pVM->nSubCrn]);
    mdl  *pDatMdl = &(pVM->aModule[0]);
    char *pDatBuf = pVM->xData.aData;

    if(!strcmp(pKeyWord, PDAT_ACT)){/* 获取PTP的速度 */
        (*pBuffCrn)++;/* 跳过等于号 */
        for(; pLineCrn[*pBuffCrn] == ' '; (*pBuffCrn)++){}

        GetKeyName(VarName, pLineCrn, pBuffCrn);

        if(!strcmp(VarName, PDEFAULT)){
            pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_VEL;
            pSubCrn->aPsd[pPsdNum++].uData = 50;
        }
        else{
            GetVarAdd = ProgramGetDat(pDatMdl, pDatBuf, VarName, VAR_PDAT);

            if(GetVarAdd != 0xFFFFFFFF){
                var  *pVar  = (var *)(pDatBuf + GetVarAdd);
                pdat *pPdat = (pdat *)(&(pVar->fData));

                pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_VEL;
                pSubCrn->aPsd[pPsdNum++].uData = pPdat->cVel;
            }
        }
    }
    else if(!strcmp(pKeyWord, FDAT_ACT)){/* 获取工具号跟基坐标号 */
        (*pBuffCrn)++;
        for(; pLineCrn[*pBuffCrn] == ' '; (*pBuffCrn)++){}

        GetKeyName(VarName, pLineCrn, pBuffCrn);

        if(!strcmp(VarName, FHOME)){
            pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_TOOL;
            pSubCrn->aPsd[pPsdNum++].uData = 0;

            pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_WOBJ;
            pSubCrn->aPsd[pPsdNum++].uData = 0;
        }
        else{
            GetVarAdd = ProgramGetDat(pDatMdl, pDatBuf, VarName, VAR_FDAT);

            if(GetVarAdd != 0xFFFFFFFF){
                var  *pVar  = (var *)(pDatBuf + GetVarAdd);
                fdat *pFdat = (fdat *)(&(pVar->fData));

                pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_TOOL;
                pSubCrn->aPsd[pPsdNum++].uData = pFdat->cTool;

                pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_WOBJ;
                pSubCrn->aPsd[pPsdNum++].uData = pFdat->cBase;
            }
        }
    }
    else if(!strcmp(pKeyWord, LDAT_ACT)){/* 获取LIN的速度 */
        (*pBuffCrn)++;
        for(; pLineCrn[*pBuffCrn] == ' '; (*pBuffCrn)++){}

        GetKeyName(VarName, pLineCrn, pBuffCrn);

        GetVarAdd = ProgramGetDat(pDatMdl, pDatBuf, VarName, VAR_LDAT);

        if(GetVarAdd != 0xFFFFFFFF){
            var  *pVar  = (var *)(pDatBuf + GetVarAdd);
            ldat *pLdat = (ldat *)(&(pVar->fData));

            pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_VEL;
            pSubCrn->aPsd[pPsdNum++].uData = pLdat->cVel;
        }
    }
    else if(!strcmp(pKeyWord, PTP_)){/* PTP运动指令 */
        for(; pLineCrn[*pBuffCrn] == ' '; (*pBuffCrn)++){}

        GetKeyName(VarName, pLineCrn, pBuffCrn);

        if(!strcmp(VarName, XHOME)){
            pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_END;
            pSubCrn->aPsd[pPsdNum++].pData = (void *)&HOME;
        }
        else{
            GetVarAdd = ProgramGetDat(pDatMdl, pDatBuf, VarName, VAR_E6POS);

            if(GetVarAdd != 0xFFFFFFFF){
                var   *pVar   = (var *)(pDatBuf + GetVarAdd);
                e6pos *pE6pos = (e6pos *)(&(pVar->fData));

                pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_END;
                pSubCrn->aPsd[pPsdNum++].pData = (void *)pE6pos;
            }
        }
        pSubCrn->aPsd[pPsdNum--].uOp = OP_NULL;

        for(;pPsdNum >= 0;pPsdNum--)  pSubCrn->aPsd[pPsdNum].uOp = OP_MOVJ;

    }
    else if(!strcmp(pKeyWord, LIN_)){/* LIN运动指令 */
        for(; pLineCrn[*pBuffCrn] == ' '; (*pBuffCrn)++){}

        GetKeyName(VarName, pLineCrn, pBuffCrn);

        if(!strcmp(VarName, XHOME)){
            pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_END;
            pSubCrn->aPsd[pPsdNum++].pData = (void *)&HOME;
        }
        else{
            GetVarAdd = ProgramGetDat(pDatMdl, pDatBuf, VarName, VAR_E6POS);

            if(GetVarAdd != 0xFFFFFFFF){
                var   *pVar   = (var *)(pDatBuf + GetVarAdd);
                e6pos *pE6pos = (e6pos *)(&(pVar->fData));

                pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_END;
                pSubCrn->aPsd[pPsdNum++].pData = (void *)pE6pos;
            }
        }

        pSubCrn->aPsd[pPsdNum--].uOp = OP_NULL;

        for(;pPsdNum >= 0;pPsdNum--)  pSubCrn->aPsd[pPsdNum].uOp = OP_MOVL;
    }
    else if(!strcmp(pKeyWord, CIRC_)){/* CIRC运动指令 */
        for(; pLineCrn[*pBuffCrn] == ' '; (*pBuffCrn)++){}

        GetKeyName(VarName, pLineCrn, pBuffCrn);

        if(!strcmp(VarName, XHOME)){
            pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_MID;
            pSubCrn->aPsd[pPsdNum++].pData = (void *)&HOME;
        }
        else{
            GetVarAdd = ProgramGetDat(pDatMdl, pDatBuf, VarName, VAR_E6POS);

            if(GetVarAdd != 0xFFFFFFFF){
                var   *pVar   = (var *)(pDatBuf + GetVarAdd);
                e6pos *pE6pos = (e6pos *)(&(pVar->fData));

                pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_MID;
                pSubCrn->aPsd[pPsdNum++].pData = (void *)pE6pos;
            }
        }

        (*pBuffCrn)++;/* 跳过逗号 */

        for(; pLineCrn[*pBuffCrn] == ' '; (*pBuffCrn)++){}

        GetKeyName(VarName, pLineCrn, pBuffCrn);

        if(!strcmp(VarName, XHOME)){
            pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_END;
            pSubCrn->aPsd[pPsdNum++].pData = (void *)&HOME;
        }
        else{
            GetVarAdd = ProgramGetDat(pDatMdl, pDatBuf, VarName, VAR_E6POS);

            if(GetVarAdd != 0xFFFFFFFF){
                var   *pVar   = (var *)(pDatBuf + GetVarAdd);
                e6pos *pE6pos = (e6pos *)(&(pVar->fData));

                pSubCrn->aPsd[pPsdNum].uSub  = OPMOV_END;
                pSubCrn->aPsd[pPsdNum++].pData = (void *)pE6pos;
            }
        }

        pSubCrn->aPsd[pPsdNum--].uOp = OP_NULL;

        for(;pPsdNum >= 0;pPsdNum--)  pSubCrn->aPsd[pPsdNum].uOp = OP_MOVC;
    }
    else{

    }
}

/* 函数说明:遇到定义子函数的代码段,跳过此代码段
 * 参   数:
 * 返 回 值:
*/
static void SkipSub2End(vm *pVM, u32 *CodeAddCrn)
{
    char *pCodeBuf   = pVM->xCode.aCode;

    char  Line[512];
    u32   pLine;

    char  KeyWord[32];

    code2hmi  SrchSub;
    memset(&SrchSub,0,sizeof(code2hmi));
    SrchSub.LineCrn = CodeRecord.LineCrn;

    for(;;){
        for(pLine = 0; *(pCodeBuf + *CodeAddCrn) != '\n'; (*CodeAddCrn)++,pLine++)
            Line[pLine] = *(pCodeBuf + *CodeAddCrn);
        Line[pLine] = '\n';
        (*CodeAddCrn)++;

        for(pLine = 0;Line[pLine] == ' '; pLine++){}

        GetKeyName(KeyWord, Line, &pLine);

        if(Line[pLine] == '\r' || Line[pLine] == '\n')        SrchSub.LineCrn++;
        else if(!strcmp(KeyWord,_FOLD))                       SrchSub.LineCrn++;
        else if(Line[pLine] == '(')                           SrchSub.LineCrn++;
        else if(!strcmp(KeyWord, END)){
            SrchSub.LineCrn++;
            break;
        }
    }
    memcpy(&CodeRecord, &SrchSub, sizeof(code2hmi));
}

/* 函数说明:跳过程序的开头的;FOLD INI-> ;ENDFOLD(INI)
 * 参   数:
 * 返 回 值:
*/
static void SkipFront(vm *pVM,  u32 *CodeAddCrn)
{
    char *pCodeBuf   = pVM->xCode.aCode;

    char  Line[512];
    u32   pLine;

    char  KeyWord[32];

    while(true){
        for(pLine = 0; *(pCodeBuf + *CodeAddCrn) != '\n'; pLine++,(*CodeAddCrn)++)
            Line[pLine] = *(pCodeBuf + *CodeAddCrn);
        Line[pLine] = '\n';
        (*CodeAddCrn)++;

        for(pLine = 0;Line[pLine] == ' ';pLine++){}

        GetKeyName(KeyWord,Line,&pLine);

        if(!strcmp(KeyWord, _ENDFOLD)){
            for(;Line[pLine] == ' ';pLine++){}
            pLine++;/*(*/
            for(;Line[pLine] == ' ';pLine++){}
            GetKeyName(KeyWord,Line,&pLine);
            if(!strcmp(KeyWord, INI)){
                pVM->aModule[1].nStart = *CodeAddCrn;
                break;
            }
        }
    }
}

/* 函数说明:退出当前的Loop循环
 * 参   数:
*/
static bool SkipLoopCircle(vm *pVM, u32 *pCodeAddCrn)
{
    i16  *pLogicCrn = &(pVM->aSubStk[pVM->nSubCrn].uLogicCrn);
    wifl *pWifl     =   pVM->aSubStk[pVM->nSubCrn].aLogicStk;
    u32  CodeAddCrn;

    if(*pLogicCrn < 0)  return false;
    else{
        for(i16 i = *pLogicCrn; i >= 0;i--){
            if(pWifl[i].iWifl == WIFL_LOOP){
                CodeAddCrn = pWifl[i].uAddStart;
                if(GotoEndLoop(pVM,&CodeAddCrn,&(pWifl[i]))){
                    *pCodeAddCrn = CodeAddCrn;
                    *pLogicCrn = --i;
                    return true;
                }
                else return false;
            }/* if(pWifl[i].iWifl == WIFL_LOOP) */
        }/* end of for */
    }/* end of else */
}

/* 函数说明:
 * 参   数:
 * 返 回 值:
*/
static bool GotoEndLoop(vm *pVM, u32 *CodeAddCrn,wifl *pWiflCrn)
{
    mdl  *pSrcMdl    = &(pVM->aModule[1]);
    u32   pSrcStart  = pSrcMdl->nStart;
    u32   FileSize   = pSrcMdl->nSize;
    u32   SrcEndAdd  = FileSize + pSrcStart;

    char *pCode      = pVM->xCode.aCode;
    u32   pLine;
    char  Line[512];
    char  KeyWord[32];

    code2hmi  SrchSub;
    memset(&SrchSub,0,sizeof(code2hmi));
    SrchSub.LineCrn = pWiflCrn->uLineStart;

    while(*CodeAddCrn < SrcEndAdd){
        for(pLine = 0; *(pCode + *CodeAddCrn) != '\n'; (*CodeAddCrn)++,pLine++)
            Line[pLine] = *(pCode + *CodeAddCrn);
        Line[pLine] = '\n';
        (*CodeAddCrn)++;

        for(pLine = 0;Line[pLine] == ' ';pLine++){}

        if(Line[pLine] == '\r' || Line[pLine] == '\n')
            SrchSub.LineCrn++;
        else{
            GetKeyName(KeyWord,Line,&pLine);

            if(!strcmp(KeyWord,_FOLD))
                SrchSub.LineCrn++;
            else if(Line[pLine] == '('){/* 函数 */
                for(;Line[pLine] == ' ';pLine++){}
                if(Line[++pLine] == ')')
                    SrchSub.LineCrn++;
            }
            else if(!strcmp(KeyWord,ENDLOOP)){
                memcpy(&CodeRecord, &SrchSub, sizeof(code2hmi));
                return true;
            }
        }
    }
    return false;
}

/* 函数说明:遇到END退出当前函数
 * 参   数:
*/
static bool ExitSub(vm *pVM)
{   
    pVM->nSubCrn--;/* 出栈 */

    if(pVM->nSubCrn >= 0){
        sub  *pSubCrn = &(pVM->aSubStk[pVM->nSubCrn]);

        CodeRecord.LineCrn = pSubCrn->nLineCrn;
        strcpy(CodeRecord.pMdlCrn, pSubCrn->cMdlCrn);
        strcpy(CodeRecord.pSubCrn, pSubCrn->cSubCrn);

        return true;
    }
    else  return false;
}

static bool JudgeCondition(char *pLineCrn, u32 *pBuffCrn)
{
    char  cPortNum[8];  /* IO端口号 */
    char  cKeyWord[32];
    u16   uPortNum;
    for(;pLineCrn[*pBuffCrn] == ' ';(*pBuffCrn)++){}

    __GetKeyName(cPortNum, pLineCrn, pBuffCrn, ']');
    (*pBuffCrn)++;

    uPortNum = atoi(cPortNum);

    if(uPortNum <= 16){
        if(pLineCrn[(*pBuffCrn)++] == '=' && pLineCrn[(*pBuffCrn)++] == '='){
            for(;pLineCrn[*pBuffCrn] == ' ';(*pBuffCrn)++){}
            __GetKeyName(cKeyWord, pLineCrn, pBuffCrn, NULL);

            if(IO_Get(ioDIc, uPortNum)){
                if(!strcmp(cKeyWord, TRUE_))   return  true;
                else                          return  false;
            }
            else{
                if(!strcmp(cKeyWord, FALSE_))  return  true;
                else                          return  false;
            }
        }
        else{/* 直接获取IO的状态,作为判断条件 */
            if(IO_Get(ioDIc, uPortNum))       return true;
            else                              return false;
        }
    }/* end of if(uPortNum <= 16) */
    else                                      return false;
}

/* 函数说明:IF中判断了非法语句,直接跳过直接到ENDIF
 * 参   数
 * 返 回 值
*/
static bool Run2Endif(vm *pVM, u32 *CodeAddCrn, u32 *pLineNumCrn)
{
    mdl  *pSrcMdl    = &(pVM->aModule[1]);
    u32   pSrcStart  = pSrcMdl->nStart;
    u32   FileSize   = pSrcMdl->nSize;
    u32   SrcEndAdd  = FileSize + pSrcStart;

    char *pCode      = pVM->xCode.aCode;
    u32   pLine;
    char  Line[512];
    char  KeyWord[32];

    code2hmi  SrchSub;
    memset(&SrchSub,0,sizeof(code2hmi));
    SrchSub.LineCrn = *pLineNumCrn;

    while(*CodeAddCrn < SrcEndAdd){
        for(pLine = 0; *(pCode + *CodeAddCrn) != '\n'; (*CodeAddCrn)++,pLine++)
            Line[pLine] = *(pCode + *CodeAddCrn);
        Line[pLine] = '\n';
        (*CodeAddCrn)++;

        for(pLine = 0;Line[pLine] == ' ';pLine++){}

        if(Line[pLine] == '\r' || Line[pLine] == '\n')
            SrchSub.LineCrn++;
        else{
            GetKeyName(KeyWord,Line,&pLine);

            if(!strcmp(KeyWord,_FOLD))
                SrchSub.LineCrn++;
            else if(!strcmp(KeyWord, EXIT))
                SrchSub.LineCrn++;
            else if(Line[pLine] == '('){/* 函数 */
                for(;Line[pLine] == ' ';pLine++){}
                if(Line[++pLine] == ')')
                    SrchSub.LineCrn++;
            }
            else if(!strcmp(KeyWord,ENDIF)){
                memcpy(&CodeRecord, &SrchSub, sizeof(code2hmi));
                return true;
            }
        }
    }
    return false;
}

/* 函数说明:获得关键字
 * 参   数
 * 返 回 值
*/
static __inline void GetKeyName(char *pKeyName, char *pLineCrn, u32 *pBuffCrn)
{
    int i;

    for(i = 0;pLineCrn[*pBuffCrn] != ',' && pLineCrn[*pBuffCrn] != ' ' && pLineCrn[*pBuffCrn] != '=' && pLineCrn[*pBuffCrn] != '(' && pLineCrn[*pBuffCrn] != '\n' && pLineCrn[*pBuffCrn] != '\r' && pLineCrn[*pBuffCrn] != ')';(*pBuffCrn)++,i++){
        pKeyName[i] = pLineCrn[*pBuffCrn];
    }/* end of for */
    pKeyName[i] = '\0';

}

/* 函数说明:
 * 参   数:
 *
 *
 *        @EndofWord:关键字的结尾
 * 返 回 值:
*/
static __inline void __GetKeyName(char *pKeyName, char *pLineCrn, u32 *pBuffCrn, char EndofWord)
{
    int i;

    for(i = 0; pLineCrn[*pBuffCrn] != ' ' && pLineCrn[*pBuffCrn] != EndofWord; (*pBuffCrn)++,i++){
        pKeyName[i] = pLineCrn[*pBuffCrn];
    }/* end of for */
    pKeyName[i] = '\0';

    return;
}
