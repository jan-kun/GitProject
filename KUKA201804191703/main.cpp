#include "kuka.h"
#include"Protocol/robot.h"
#include"Protocol/auto.h"
#include"Port/port.h"
#include <QApplication>
#include"globalapplication.h"
#include"InputPanel/myinputpanelcontext.h"
#include"thread/NCKThread.h"

int main(int argc, char *argv[])
{
    GlobalApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#if ARM_PC
    QFont font("wenquanyi",14);
#else
    QFont font("Song Ti",14);
#endif
    a.setFont(font);
    MyInputPanelContext *ic = new MyInputPanelContext;
    a.setInputContext(ic);

//    QWSServer::setCursorVisible(false);
//    QApplication::setOverrideCursor(Qt::BlankCursor);

    InitHMI();

    Port *SendPort = new Port();

    CreatThread();

    InitCTL();

    KUKA w;
    w.show();

    return a.exec();
}
