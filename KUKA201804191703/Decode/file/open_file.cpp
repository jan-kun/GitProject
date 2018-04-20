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
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "open_file.h"

/* 函数说明:打开文件.src跟.dat文件
 * 参   数:@pVM:要填充的结构体
 *        @pSrcPath:程序文件的路径
 *        @pDatPath:数据文件的路径
 * 返 回 值:读取文件成功:true
 *         读取文件失败:false
 * 特别说明:kuka的文件分为数据文件跟代码文件.
 *         数据文件放在模块0
 *         代码文件放在模块1
*/
bool ScanFile(vm *pVM,char *pDatPath,char *pSrcPath)
{
    FILE *pSrcStr,*pDataStr;
    u32 srcsize,datasize;
    mdl *datamdl = &(pVM->aModule[0]), \
        *srcmdl  = &(pVM->aModule[1]);
    int i;

    /* 数据文件跟代码文件不存在立马返回 */
    if(access(pDatPath,F_OK))  return false;/*  */
    if(access(pSrcPath,F_OK))  return false;/*  */

    if((pDataStr = fopen(pDatPath,"r")) == NULL){
        return false;
    }
    if((pSrcStr = fopen(pSrcPath,"r")) == NULL){
        return false;
    }

    for(i = strlen(pDatPath);*(pDatPath + i) != '/';i--){}

    /* 数据文件 */
    fseek(pDataStr,0,SEEK_END);
    strcpy(datamdl->aMdl,pDatPath + ++i);
    datamdl->nStart = 0;
    datamdl->nSize  = datasize = ftell(pDataStr);
    fclose(pDataStr);

    /* 代码文件 */
    fseek(pSrcStr,0,SEEK_END);
    strcpy(srcmdl->aMdl,pSrcPath + i);
    srcmdl->nStart = datasize;
    srcmdl->nSize  = srcsize = ftell(pSrcStr);
    fclose(pSrcStr);

    /* 优先存放数据文件的内容 */
    pDataStr = fopen(pDatPath,"r");
    pSrcStr = fopen(pSrcPath,"r");
    fread(pVM->xCode.aCode,1,datasize,pDataStr);/* 数据文件的存储 */
    fread(pVM->xCode.aCode + datasize,1,srcsize,pSrcStr);/* 代码文件的存储 */

    return true;
}

/* 函数说明:获取IO状态
 *
*/
extern bool GetIOState(u8 *pIOArray, u16 Addr)
{

}
