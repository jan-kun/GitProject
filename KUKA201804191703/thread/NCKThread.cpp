#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>

#include "kuka.h"

#include "NCKThread.h"
#include "globalapplication.h"
#include "Protocol/robot.h"
#include "Protocol/auto.h"
#include "Protocol/vm.h"
#include "Protocol/auto.h"
#include "Port/port.h"
#include "data.h"

#define  MY_STACK_SIZE    0x12

static u8   ErrNo = 0;

static void inc_period(struct period_info *pinfo);
static void periodic_task_init(struct period_info *pinfo);
static void wait_rest_of_period(struct period_info *pinfo);
static void *Cyclic_NCKBus(void *data);
static void NCKSendFunction(void);

static void GetIOState(void);

extern void CreatThread(void)
{
/*=============================thread init=======================================*/
    struct sched_param param;
    pthread_attr_t attr;
    pthread_t thread;

    /* Initialize pthread attributes (default values) */
    pthread_attr_init(&attr);

    /* Set a specific stack size  */
    pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);

    /* Set scheduler policy and priority of pthread */
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    param.sched_priority = 99;
    pthread_attr_setschedparam(&attr, &param);

    /* Use scheduling parameters of attr */
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    /* Create a pthread with specified attributes */
    pthread_create(&thread, &attr, Cyclic_NCKBus, NULL);

    /* Lock memory */
   if(mlockall(MCL_CURRENT) == -1) {
       printf("mlockall failed: %m\n");
       exit(-2);
   }

}

static u8 i = 0;

void *Cyclic_NCKBus(void *data)
{
    struct period_info pinfo;
    timeval start,end;
    long dif_sec, dif_usec;

    periodic_task_init(&pinfo);

    while (true) {
        NCKSendFunction();
        wait_rest_of_period(&pinfo);
    }

    return NULL;
}

static void inc_period(struct period_info *pinfo)
{
    pinfo->next_period.tv_nsec += pinfo->period_ns;

    while (pinfo->next_period.tv_nsec >= 1000000000) {
        /* timespec nsec overflow */
        pinfo->next_period.tv_sec++;
        pinfo->next_period.tv_nsec -= 1000000000;
    }
}

static void periodic_task_init(struct period_info *pinfo)
{
    /* for simplicity, hardcoding a 30ms period */
    pinfo->period_ns = 30 * 1000000;

    clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}

static void NCKSendFunction(void)
{
    int  nread1,nread2,nread3;
    bool bDtFlag;
    u8   RxMid[320];
    int  i;

    /*==============================接受数据===================================*/
    nread1 = read(fd,(void *)pNckRx,sizeof(nckr));/* first */
    if(nread1 < 152){
        memcpy(RxMid,(u8 *)pNckRx,nread1);/* 读到的数据放在缓冲区里面 */
        memset(pNckRx,0,sizeof(nckr));
        nread2 = read(fd,(void *)pNckRx,sizeof(nckr)); /* second */
        memcpy(RxMid+nread1,(u8 *)pNckRx,nread2);/* 读到的数据放在缓冲区里面 */
        memset(pNckRx,0,sizeof(nckr));

        if(nread2 < (152-nread1)){/* 第二次读的数据还不够,继续读 */
            nread3 = read(fd,(void *)pNckRx,sizeof(nckr));  /* third */
            memcpy(RxMid+nread1+nread2,(u8 *)pNckRx,nread3);  /* 读到的数据放在缓冲区里面 */
            memset(pNckRx,0,sizeof(nckr));
        }
        for(i = 0;i < 320;i++){
            if(RxMid[i] == 0x55)
                break;
        }
        memcpy(pNckRx,RxMid + i,sizeof(nckr));
    }

    bDtFlag = false;

    /*=======================判断数据正确与否,并作处理============================*/
    if(crc_Get((void *)pNckRx,sizeof(nckr)) == 0){
        if(pNckRx->cAppend == xAppend){/* 进一步验证数据是否正确 */
            xAppend++;
            ErrNo = 0;

            /* 数据正确情况下才做处理 */
            BusNckR(pNckRx);
            RunCode();
            memset(pNckRx, 0, sizeof(nckt));

            if(pNckTx == &xBufT.aBufT[xSendNum]){/* 判断上一串数据是不是数据缓冲区的,如果是,指针移向下一个 */
                memset(pNckTx,0,sizeof(nckt));
                xSendNum++;
                if(xSendNum == xSys.iSdNum){
                    xSendNum   = 0;
                    xBufT.flag = 0;
                }
            }
            else if(pNckTx == &(xImdT.aImdT)){
                memset(&xImdT,0,sizeof(sImdT));
            }
        }
        else{
            memset(pNckRx,0,sizeof(nckr));
            if(ErrNo >= 4){
                xAppend = 0;
                ErrNo = 0;
                Repetition.cAppend = 0;
                if(pNckTx == NULL)  pNckTx = &(xNulT.aNulT);
                memcpy(pNckTx, &Repetition, sizeof(nckt));
                crc_Set((void *)pNckTx,sizeof(nckt));
            }
            ErrNo++;
            bDtFlag = true;
            write(fd,(void *)pNckTx, sizeof(nckt));
        }
    }
    else{
        memset(pNckRx,0,sizeof(nckr));
        if(ErrNo >= 4){
            xAppend = 0;
            ErrNo = 0;
            Repetition.cAppend = 0;
            if(pNckTx == NULL)  pNckTx = &(xNulT.aNulT);
            memcpy(pNckTx, &Repetition, sizeof(nckt));
            crc_Set((void *)pNckTx,sizeof(nckt));
        }
        ErrNo++;
        bDtFlag = true;
        write(fd,(void *)pNckTx, sizeof(nckt));
    }

    /*=============================发送数据===================================*/
    if(bDtFlag == false){
        if(xImdT.flag == 1){
            pNckTx = &(xImdT.aImdT);
            pNckTx->cNck = NCK_IMD;
        }
        else if(xBufT.flag == 1){
            pNckTx = &(xBufT.aBufT[xSendNum]);
            pNckTx->cNck = NCK_BUF;
        }
        else{
            memset(&xNulT.aNulT,0,sizeof(nckt));
            pNckTx = &(xNulT.aNulT);
            pNckTx->cNck = NCK_NULL;
        }
        GetIOState();

        pNckTx->cHeader = 0x55;
        pNckTx->cAddr   = 0;
        pNckTx->cAppend = xAppend;

        if(IO_Get(ioDIc, NCK_DI_EMG))            IO_Set(ioDIc,NCK_DI_SERVO);
        else                                     IO_Reset(ioDIc,NCK_DI_SERVO);

        memcpy(pNckTx->aDI,ioDIc,20);
        memcpy(pNckTx->aDO,ioDOc,20);

        crc_Set((void *)pNckTx,sizeof(nckt));
        memcpy(&Repetition, pNckTx, sizeof(nckt));
        write(fd,(void *)pNckTx, sizeof(nckt));
    }
    else if(xSys.iNckBuf == SYS_NCKBUF_FULL && bDtFlag == false){
        pNckTx          = &(xNulT.aNulT);
        pNckTx->cNck    = NCK_NULL;
        pNckTx->cHeader = 0x55;
        pNckTx->cAddr   = 0;
        pNckTx->cAppend = xAppend;

        memset(pNckTx->aDI,0,20);
        crc_Set((void *)pNckTx,sizeof(nckt));
        memcpy(&Repetition, pNckTx, sizeof(nckt));
        write(fd,(void *)pNckTx, sizeof(nckt));
    }
}

static void wait_rest_of_period(struct period_info *pinfo)
{
        inc_period(pinfo);

        /* for simplicity, ignoring possibilities of signal wakes */
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &pinfo->next_period, NULL);
}

static void GetIOState(void)
{
    if(ioDIState.flag == 1){
        memcpy(ioDIc,ioDIState.ioDIc,2);
        memset(ioDIState.ioDIc, 0, 20);
        ioDIState.flag = 0;
    }
}
