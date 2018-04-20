#include "port.h"
#include"Protocol/IO.h"

#define ROCKER_SP "/dev/ttyS1"
#define NCK_SP "/dev/ttyS4"
#define MAX(a,b)   ( (  a > b ) ? ( a ) : ( b )   )
#define STOP_RANGE   120
#define ORI          0

Rock_PKG RockerStruct; //摇杆一次发送过来的数据保存在此结构体里面
i16 Axis_Info[6];   //保存最新的XYZABC值
static int VelC = 100;

int fd = -1;

Port::Port()
{
    pNckTx=&(xImdT.aImdT);
    pNckTx->cHeader = 0x55;
    pNckTx->cNck = NCK_IMD;
    pNckTx->cAddr = 0;
    pNckTx->cAppend =0;
    crc_Set(pNckTx,sizeof(nckt));

    node = "/dev/ttyS4";

/*-------------------------------------------------------------------------------*/
    fd = open(node, O_RDWR | O_NONBLOCK);
    if (fd < 0)
    {
        printf("open Serial Port node faild\n");
        exit(0);
    }
    else
    {
        qDebug()<<"open Serial Port node... ...";
    }

    set_speed(fd,460800); //设置串口波特率
    set_Parity(fd,8,2,'N'); //设置8位数据位，1位停止位，无校验等其他设置

    memcpy(&Repetition, pNckTx, sizeof(nckt));

    write(fd, (void *)pNckTx , sizeof(nckt));

    usleep(20 * 1000);

//    NckSend = new  NckThread();
//    NckSend->start(QThread::HighestPriority);

/*-------------------------------------------------------------------------------*/
    QSerialPortInfo RockPortInfo,NckPortInfo;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(info.systemLocation() == ROCKER_SP)
            RockPortInfo = info;
        else if(info.systemLocation() == NCK_SP)
            NckPortInfo = info;
    }

    SerialRock.setPort(RockPortInfo);
    if(SerialRock.open(QIODevice::ReadWrite))
    {
        qDebug()<<"Open Rock...";
        SerialRock.setBaudRate(QSerialPort::Baud38400);
        SerialRock.setParity(QSerialPort::NoParity);
        SerialRock.setDataBits(QSerialPort::Data8);
        SerialRock.setStopBits(QSerialPort::OneStop);
        SerialRock.setFlowControl(QSerialPort::NoFlowControl);
        SerialRock.setDataTerminalReady(true);
        SerialRock.clearError();
        SerialRock.clear();
        connect(&SerialRock , SIGNAL(readyRead()) , this , SLOT(func_receiveRocker()));
    }
    else
        qDebug()<<"Open Rock Fail";

    /*向摇杆发送请求数据指令*/
    u8 SET_ZERO = 0xAD;
    u8 START_T = 0xAE;
    u8 REQUEST_DATA = 0xAC;
    SerialRock.write( (char*)(&SET_ZERO) , sizeof(u8));
    SerialRock.write( (char*)(&START_T) , sizeof(u8));
    SerialRock.write( (char*)(&REQUEST_DATA) , sizeof(u8));
}

/*
** 函数功能：使能一个方向的运动，失能其余所有方向
** 函数参数：n，使能的方向
** 函数返回值：无
*/
void Port::func_axisEnable(int n)
{
    for(int i=0;i<12;i++)
    {
        if(i == n)
            IO_Set(ioDIState.ioDIc,NCK_DI_XSP+n);
        else
            IO_Reset(ioDIState.ioDIc,NCK_DI_XSP+i);
    }
    ioDIState.flag=1;
}

void Port::func_handleRocker(int n)
{
    if( n == 0 )
    {
        if(KukaRob.Cor_Mouse == 0)
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_XSM);  //1+
            else
                func_axisEnable(NCK_DI_XSP);   //1-
        }
        else
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_XSP);  //X+
            else
                func_axisEnable(NCK_DI_XSM);   //X-
        }
    }
    else if( n == 1 )
    {
        if(KukaRob.Cor_Mouse == 0)
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_YLP);  //2+
            else
                func_axisEnable(NCK_DI_YLM);   //2-
        }
        else
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_YLP);  //Y+
            else
                func_axisEnable(NCK_DI_YLM);   //Y-
        }
    }
    else if( n == 2 )
    {
        if(KukaRob.Cor_Mouse == 0)
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_ZUM);  //3+
            else
                func_axisEnable(NCK_DI_ZUP);   //3-
        }
        else
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_ZUP);  //Z+
            else
                func_axisEnable(NCK_DI_ZUM);   //Z-
        }
    }
    else if( n == 3 )
    {
        if(KukaRob.Cor_Mouse == 0)
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_ZTM);  //6+
            else
                func_axisEnable(NCK_DI_ZTP);   //6-
        }
        else
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_XRM);  //重定位绕X+
            else
                func_axisEnable(NCK_DI_XRP);   //重定位绕X-
        }
    }
    else if( n == 4 )
    {
        if(KukaRob.Cor_Mouse == 0)
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_YBM);  //5+
            else
                func_axisEnable(NCK_DI_YBP);   //5-
        }
        else
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_YBM);  //重定位绕Y+
            else
                func_axisEnable(NCK_DI_YBP);   //重定位绕Y-
        }
    }
    else if( n == 5 )
    {
        if(KukaRob.Cor_Mouse == 0)
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_ZTM);  //4+
            else
                func_axisEnable(NCK_DI_ZTP);   //4-
        }
        else
        {
            if(Axis_Info[n] > ORI)
                func_axisEnable(NCK_DI_ZTM);  //重定位绕Z+
            else
                func_axisEnable(NCK_DI_ZTP);   //重定位绕Z-
        }
    }

    NcktCmd(&(xImdT.aImdT), NCK_JOG, 0, 100);
    xImdT.flag = 1;
}

void Port::func_receiveRocker()
{
    u8 Rocket_RBuf[20];     //接收摇杆传来的数据
    i16 tChecksum = 0,cChecksum=0;  //checksum校验

    usleep(50);
    singleReadRock = SerialRock.readAll();
    chRock = singleReadRock.data();
    memcpy( &Rocket_RBuf , chRock , sizeof(Rocket_RBuf));
    singleReadRock.clear();

    if(Rocket_RBuf[0] == 0x96 && Rocket_RBuf[15] == 0x8D)  //表示之后收到的数据为位置信息
    {
        for(int i = 1 ; i < 13 ; i++)
            RockerStruct.data[i] = Rocket_RBuf[i];
        //进行checksum校验
        for(int i=0;i<13;i++)
            cChecksum += Rocket_RBuf[i];
        cChecksum &= 0x3FFF;
        tChecksum = Rocket_RBuf[13]*128+Rocket_RBuf[14];
        //计算各个轴的值
        if(cChecksum == tChecksum)//checksum校验通过
        {
            Axis_Info[0] = Rocket_RBuf[1]*128 + Rocket_RBuf[2] - 8192;
            Axis_Info[1] = Rocket_RBuf[3]*128 + Rocket_RBuf[4] - 8192;
            Axis_Info[2] = Rocket_RBuf[5]*128 + Rocket_RBuf[6]- 8192 ;
            Axis_Info[3] = Rocket_RBuf[7]*128 + Rocket_RBuf[8]- 8192;
            Axis_Info[4] = Rocket_RBuf[9]*128 + Rocket_RBuf[10]- 8192;
            Axis_Info[5] = Rocket_RBuf[11]*128 + Rocket_RBuf[12]- 8192;
        }
        else
            return;
        //计算摆动幅度
        aa = abs(Axis_Info[0] - ORI);
        bb = abs(Axis_Info[1] - ORI);
        cc = abs(Axis_Info[2] - ORI);
        dd = abs(Axis_Info[3] - ORI);
        ee = abs(Axis_Info[4] - ORI);
        ff = abs(Axis_Info[5] - ORI);
        //计算摆动幅度最大的轴，每次只动摆动幅度最大的那一个轴
        maxaxis = MAX( MAX( MAX( MAX( MAX(aa,bb) , cc ) , dd ) , ee ) , ff );
        if(maxaxis < STOP_RANGE){
            /* 在原点一定范围之类，则当做扰动 */
            ResetIO();
        }
        else
        {
            if(maxaxis == aa)            func_handleRocker(0);
            else if(maxaxis ==bb)     func_handleRocker(1);
            else if(maxaxis ==cc)     func_handleRocker(2);
            else if(maxaxis ==dd)    func_handleRocker(3);
            else if(maxaxis ==ee)    func_handleRocker(4);
            else if(maxaxis ==ff)     func_handleRocker(5);

        }
    }
    memset(&Rocket_RBuf,0,sizeof(char)*20);
}

void Port::ResetIO(void)
{
    for(int i=0;i<12;i++)
    {
        IO_Reset(ioDIState.ioDIc,NCK_DI_XSP+i);
    }
    ioDIState.flag=1;
}

void Port::set_speed(int fd, int speed)
{
    int   i;
    int   status;
    struct termios   Opt;
    tcgetattr(fd, &Opt);
    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
    {
        if (speed == name_arr[i])
        {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(&Opt, speed_arr[i]);
            cfsetospeed(&Opt, speed_arr[i]);
            status = tcsetattr(fd, TCSANOW, &Opt);
            if (status != 0)
            perror("tcsetattr fd1");
            return;
        }
        tcflush(fd,TCIOFLUSH);
    }
}

int Port::set_Parity(int fd, int databits, int stopbits, int parity)
{
    struct termios options;
    if  ( tcgetattr( fd,&options)  !=  0)
    {
        perror("SetupSerial 1");
        return(FALSE);
    }
    options.c_cflag &= ~CSIZE;
    switch (databits)
    {
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            fprintf(stderr,"Unsupported data size\n");
        return (FALSE);
    }
    switch (parity)
    {
    case 'n':
    case 'N':
        options.c_cflag &= ~PARENB;
        options.c_iflag &= ~INPCK;
        break;
    case 'o':
    case 'O':
        options.c_cflag |= (PARODD | PARENB);
        options.c_iflag |= INPCK;
        break;
    case 'e':
    case 'E':
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;
        break;
    case 'S':
    case 's':
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported parity\n");
    return (FALSE);
    }
    switch (stopbits)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
        default:
            fprintf(stderr,"Unsupported stop bits\n");
        return (FALSE);
    }


    options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);

    if (parity != 'n')
    options.c_iflag |= INPCK;
    options.c_cc[VTIME] = 150;
    options.c_cc[VMIN] = 0;

    tcflush(fd,TCIFLUSH);
    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
        perror("SetupSerial 3");
        return (FALSE);
    }
    return (TRUE);
}
