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
#include <Protocol/error.h>
#include <Protocol/linuxtypes.h>
#include <Protocol/bus.h>
#include <Protocol/robot.h>
#include <stdlib.h>

void (*ErrHandle[ERR_COUNT])(u32 uErrNum);

/* 函数说明:机器人手动操作，第一轴到第六轴的运动超出范围
 * 参   数:轴数
 * 返 回 值:无
*/
void ErrJointJog(u32 uErrNum)
{
    switch(uErrNum){
    case ERR_JOINT_A1:
        xSys.iJogErr = SYS_JOGERR_A1;
        break;
    case ERR_JOINT_A2:
        xSys.iJogErr = SYS_JOGERR_A2;
        break;
    case ERR_JOINT_A3:
        xSys.iJogErr = SYS_JOGERR_A3;
        break;
    case ERR_JOINT_A4:
        xSys.iJogErr = SYS_JOGERR_A4;
        break;
    case ERR_JOINT_A5:
        xSys.iJogErr = SYS_JOGERR_A5;
        break;
    case ERR_JOINT_A6:
        xSys.iJogErr = SYS_JOGERR_A6;
        break;
    case ERR_JOINT_A7:
        break;
    case ERR_JOINT_A8:
        break;
    default:break;
    }
}

extern void InitErr(void)
{
    for(int i=0;i<ERR_COUNT;i++)
    {
        ErrHandle[i]=NULL;
    }
    for(int i=0;i<8;i++)   ErrHandle[i]=ErrJointJog;

}
