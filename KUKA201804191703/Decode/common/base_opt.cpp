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
#include <stdlib.h>
#include <string>
#include <stdbool.h>

#include "base_opt.h"
#include "var.h"
#include "macro.h"

/* 函数说明：比较字符
 * 参   数：@pCdBuf:代码缓冲区的头地址
 *         @pCdAdd:代码缓冲区的偏移指针
 *         @Dst:目标字符的地址
 * 返 回 值：字符匹配：返回值为TRUE
 *         字符不匹配：返回值为FALSE
 * 特殊说明：这个函数已经跳过空格了
*/
bool CmpStr(char *pCdBuf,u32 *pCdAdd,char *Dst)
{
    u32 pP = *pCdAdd;

    while(*Dst){
        if(*(Dst++) != *(pCdBuf + pP))
            return false;
        pP++;
    }

    /* 这个为防止前面几个字符相同,后面几个字符不同的情况 */
    if(!(*Dst))        *pCdAdd = pP;

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    return true;
}

/* 函数说明：复制变量名到结构体的变量名成员
 * 参   数：@pCdBuf:代码缓冲区的头地址
 *         @pCdAdd:代码缓冲区的偏移指针
 *         @Dst:目标字符的地址
 *         @Length:目标字符的长度
 * 返 回 值：无
*/
void CpyStr(char *pCdBuf, u32 *pCdAdd, char *Dst, u8 Length)
{    
    for(;Length > 0;Length--)
        *(Dst++) = *(pCdBuf + (*pCdAdd)++);

    *(Dst) = '\0';
}

/* 函数说明:
 * 参   数:
 * 返 回 值:
*/
void GetVarName(char *pCdBuf, u32 *pCdAdd, char *VarName)
{
    char cName[33] = "";/*32 + 1*/

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    for(cName[32] = 0;*(pCdBuf + *pCdAdd) != '=' && *(pCdBuf + *pCdAdd) != ' ';cName[32]++){
        if(cName[32] > 32)   break;
        *(cName + cName[32]) = *(pCdBuf + (*pCdAdd)++);
    }

    cName[cName[32]] = '\0';

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    memcpy(VarName,cName,cName[32]);

    return;
}

/* 函数说明:跳过字符={
 * 参   数:
 * 返 回 值:无
*/
void EvaData(char *pCdBuf,u32 *pCdAdd)
{

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    /* '='符号的跳过 */
    if(*(pCdBuf + *pCdAdd) == '=')      (*pCdAdd)++;

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    /* '{'符号的跳过 */
    if(*(pCdBuf + *pCdAdd) == '{')      (*pCdAdd)++;

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    return;
}

/* 函数说明:获取E6POS里面的各个参数的值
 * 参   数:
 *
 *        @sPara:代表什么参数的值
 * 返 回 值:
*/
double GetE6posValue(char *pCdBuf,u32 *pCdAdd,u16 sPara)
{
    double dValue;
    char   cValue[32];
    u8     i;

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    switch(sPara){
    case X_DEF:
    case Y_DEF:
    case Z_DEF:
    case A_DEF:
    case B_DEF:
    case C_DEF:
    case S_DEF:
    case T_DEF:
        (*pCdAdd)++;
        break;
    case E1_DEF:
    case E2_DEF:
    case E3_DEF:
    case E4_DEF:
    case E5_DEF:
    case E6_DEF:
        return NULL;
        (*pCdAdd) += 2;
    default:/* 程序有误的情况 */
        break;
    }
    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    /* 取值 */
    for(i = 0;*(pCdBuf + *pCdAdd) != ',';i++){
        *(cValue + i) = *(pCdBuf + (*pCdAdd)++);
    }
    (*pCdAdd)++;
    cValue[i] = '\0';

    dValue = (double)atof(cValue);
    return dValue;
}

/* 函数说明:
 * 参   数:
 * 返 回 值:
 * 特别说明:APO_MODE在上一层函数做判断,在这一层函数不好做判断
*/
double GetPdatValue(char *pCdBuf,u32 *pCdAdd,u16 sPara)
{
    double dValue;
    char   cValue[32];
    u8     i;

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    switch(sPara){
    case VEL_DEF:
        *pCdAdd += SizeVel;
        break;
    case ACC_DEF:
        *pCdAdd += SizeAcc;
        break;
    case APO_DIST_DEF:
        *pCdAdd += SizeApoDist;
        break;
    case APO_MODE_DEF:
        if(*(pCdBuf + *pCdAdd) == 'A')  return 0x01;
        else                            return 0x00;
    default:
        break;
    }
    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    /* 取值 */
    for(i = 0;*(pCdBuf + *pCdAdd) != ',';i++){
        *(cValue + i) = *(pCdBuf + (*pCdAdd)++);
    }
    (*pCdAdd)++;
    cValue[i] = '\0';

    dValue = (double)atof(cValue);
    return dValue;
}

/* 函数说明:
 * 参   数:
 * 返 回 值:
*/
u32 GetFdatValue(char *pCdBuf,u32 *pCdAdd,u16 sPara)
{
    u32    lValue;
    char   cValue[32];
    u8     i;

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    switch(sPara){
    case TOOL_NO_DEF:
        *pCdAdd += SizeToolNo;
        break;
    case BASE_NO_DEF:
        *pCdAdd += SizeBaseNo;
        break;
    case IPO_FRAME_DEF:
        *pCdAdd += SizeIpoFrame;

        for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}
        /* 取值 */
        for(i = 0;*(pCdBuf + *pCdAdd) != ',';i++){
            *(cValue + i) = *(pCdBuf + (*pCdAdd)++);
        }
        (*pCdAdd)++;
        cValue[i] = '\0';

        if(!strcmp(cValue,TCP))    return  0x01;
        else                       return  0x00;

    case POINT2_DEF:
        *pCdAdd += SizePoint2;

        for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

        (*pCdAdd)++; /* “ */

        if(*(pCdBuf + *pCdAdd) == ' '){
            for(;*(pCdBuf + *pCdAdd) != ',';(*pCdAdd)++){}
            lValue = NULL;
        }
        else{/* 搜索值 */
            lValue = 0xFFF;
        }
        (*pCdAdd)++;

        return lValue;
    case TQ_STATE_DEF:
        *pCdAdd += SizeTqState;

        for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}
        /* 取值 */
        for(i = 0;*(pCdBuf + *pCdAdd) != ',';i++){
            *(cValue + i) = *(pCdBuf + (*pCdAdd)++);
        }
        (*pCdAdd)++;
        cValue[i] = '\0';

        if(!strcmp(cValue,TRUE_))    return  0x01;
        else                         return  0x00;

    default:
        break;
    }
    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    /* 取值 */
    for(i = 0;*(pCdBuf + *pCdAdd) != ',';i++){
        *(cValue + i) = *(pCdBuf + (*pCdAdd)++);
    }
    (*pCdAdd)++;
    cValue[i] = '\0';

    lValue = (u32)atol(cValue);
    return lValue;
}

/* 函数说明:
 * 参   数:
 * 返 回 值:
*/
double GetLdatValue(char *pCdBuf,u32 *pCdAdd,u16 sPara)
{
    double dValue;
    char   cValue[32];
    int    i;

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    switch(sPara){
    case VEL_DEF:
        *pCdAdd += SizeVel;
        break;
    case ACC_DEF:
        *pCdAdd += SizeAcc;
        break;
    case APO_DIST_DEF:
        *pCdAdd += SizeApoDist;
        break;
    case APO_FAC_DEF:
        *pCdAdd += SizeApoDist;
        break;
    case AXIS_ACC_DEF:
        *pCdAdd += SizeAxisAcc;
        break;
    case AXIS_VEL_DEF:
        *pCdAdd += SizeAxisVel;
        break;
    case ORI_TYP_DEF:/* 方向导引:#VAR #JOINT #CONSTANT */
        *pCdAdd += SizeOriTyp;

        for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}
        /* 取值 */
        for(i = 0;*(pCdBuf + *pCdAdd) != ',' && *(pCdBuf + *pCdAdd) != '}';i++){
            *(cValue + i) = *(pCdBuf + (*pCdAdd)++);
        }
        (*pCdAdd)++;
        cValue[i] = '\0';

        if(!strcmp(cValue,VAR))         return 0x01;
        else if(!strcmp(cValue,JOINT))  return 0x02;
        else                            return 0x03;
    case CIRC_TYP_DEF:
        *pCdAdd += SizeCircTyp;
        for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}
        /* 取值 */
        for(i = 0;*(pCdBuf + *pCdAdd) != ',' && *(pCdBuf + *pCdAdd) != '}';i++){
            *(cValue + i) = *(pCdBuf + (*pCdAdd)++);
        }
        (*pCdAdd)++;
        cValue[i] = '\0';

        if(!strcmp(cValue,BASE))        return 0x01;
        else                            return 0x02;
    case JERK_FAC_DEF:
        *pCdAdd += SizeJerkFac;
        break;
    case GEAR_JERK_DEF:
        *pCdAdd += SizeGearJerk;
        break;
    case EXAX_IGN_DEF:
        *pCdAdd += SizeExaxIgn;
        break;
    default:
        break;
    }

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    /* 取值 */
    for(i = 0;*(pCdBuf + *pCdAdd) != ',' && *(pCdBuf + *pCdAdd) != '}';i++){
        *(cValue + i) = *(pCdBuf + (*pCdAdd)++);
    }
    (*pCdAdd)++;
    cValue[i] = '\0';

    dValue = (double)atof(cValue);
    return dValue;
}

extern double GetTqmdatValue(char *pCdBuf, u32 *pCdAdd, u16 sPara)
{
    double dValue;
    char   cValue[32];
    int    i;

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    switch(sPara){
    case T11_DEF:
    case T12_DEF:
    case T13_DEF:
    case T14_DEF:
    case T15_DEF:
    case T16_DEF:
    case T21_DEF:
    case T22_DEF:
    case T23_DEF:
    case T24_DEF:
    case T25_DEF:
    case T26_DEF:
        *pCdAdd += SizeTxx;
        break;
    case K1_DEF:
    case K2_DEF:
    case K3_DEF:
    case K4_DEF:
    case K5_DEF:
    case K6_DEF:
    case O1_DEF:
    case O2_DEF:
    case ID_DEF:
        *pCdAdd += SizeKx;
        break;
    case OVM_DEF:
    case TMF_DEF:
        *pCdAdd += SizeOvm;
        break;
    }

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    /* 取值 */
    for(i = 0;*(pCdBuf + *pCdAdd) != ',' && *(pCdBuf + *pCdAdd) != '}';i++){
        *(cValue + i) = *(pCdBuf + (*pCdAdd)++);
    }
    (*pCdAdd)++;
    cValue[i] = '\0';

    dValue = (double)atof(cValue);
    return dValue;
}

/* 函数说明:
 * 参   数:
 * 返 回 值:
*/
void EndOfLine(char *pCdBuf,u32 *pCdAdd)
{
    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}

    if(*(pCdBuf + *pCdAdd) == '}'){
        (*pCdAdd)++;
    }
    else{
        ErrFlag.flag = true;
    }

    for(;*(pCdBuf + *pCdAdd) == ' ';(*pCdAdd)++){}
}
