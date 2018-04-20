#ifndef PORT_H
#define PORT_H

#include <QObject>
#include"data.h"
#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

#include<QString>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<errno.h>
#include<linux/types.h>
#include<pthread.h>

#include<QAtomicInt>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include"Protocol/bus.h"
#include"Protocol/IO.h"
#include"Protocol/crc16.h"
#include"Protocol/robot.h"
#include"Protocol/message.h"
#include<stdlib.h>

class Port : public QWidget
{
    Q_OBJECT
public:
    Port();
    QSerialPort SerialRock,SerialNck;
    QByteArray singleReadRock;
    char* chRock;
    char* node;
    char* node2;

    int speed_arr[11]={B921600,B460800,B230400,B115200,B38400,B19200,B9600,B4800,B2400,B1200,B300};
    int name_arr[11]={921600,460800,230400,115200,38400,19200,9600,4800,2400,1200,300};

    i16 aa,bb,cc,dd,ee,ff,maxaxis;
    void set_speed(int fd, int speed);
    int set_Parity(int fd,int databits,int stopbits,int parity);
    void func_axisEnable(int n);
    void func_handleRocker(int n);

    QTimer *NckTimer;

    void ResetIO();

public slots:
    void func_receiveRocker();

};

#endif // PORT_H

