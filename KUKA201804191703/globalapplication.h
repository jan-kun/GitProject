#ifndef GLOBALAPPLICATION_H
#define GLOBALAPPLICATION_H

#include <QObject>
#include<QApplication>
#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif
class GlobalApplication : public QApplication
{
public:
     GlobalApplication(int&argc,char **argv);
     ~GlobalApplication();

     bool notify(QObject*, QEvent *);
};

#endif // GLOBALAPPLICATION_H
