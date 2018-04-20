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
 ***********************************************************************************/
#include "Protocol/linuxtypes.h"
#include "Protocol/bus.h"
#include <string.h>
#include "robot.h"

#include "vm.h"
#include "open_file.h"
#include "code2psd.h"
#include "auto.h"

vm   xVM;
vm   xMidTask;

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/

extern u16 Code2Psd(vm *pVM)
{
    pVM->aSubStk[pVM->nSubCrn].aPsd[0].uOp = OP_NULL;

    GetCmd2Run(pVM);

    if(pVM->aSubStk[pVM->nSubCrn].aPsd[0].uOp == OP_NULL || pVM->nSubCrn < 0)
        return false;
    else
        return true;
}

extern int InitVM(vm *pVM, char *pPgfPath)
{
}

extern void Decode(void)
{
    bool  rt;
    u16   uRet = AUTO_OK;

    if(xBufT.flag == 0 && xSys.iDecode == SYS_DECODE_YES){

        rt = Code2Psd(&xVM);

        if(rt == true){
            NcktCmd(xBufT.aBufT, NCK_LINE, NULL, CodeRecord.LineCrn);/* 发送行号 */
            uRet = PsdToNckt(xBufT.aBufT,&xSys,xVM.aSubStk[xVM.nSubCrn].aPsd);
            printf("this line num is %d!\n",CodeRecord.LineCrn);

            xBufT.flag = 1;
        }
        if(xVM.nSubCrn < 0){
//            xSys.iSdNum = 1;
//            NcktCmd(xBufT.aBufT,NCK_END, NULL, NULL);
//            xBufT.flag = 1;
//            xSys.iDecode = SYS_DECODE_NOT;
//            memcpy(&CodeRecorder, &xMidRecorder, sizeof(coderecorder));
           memcpy(&xVM, &xMidTask, sizeof(vm));
        }
    }

}
