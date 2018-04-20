#include "globalapplication.h"

QObject *editableItem;
int flag_codeeditor;

GlobalApplication::GlobalApplication(int &argc,char **argv) : QApplication(argc,argv)
{

}

GlobalApplication::~GlobalApplication()
{

}


bool GlobalApplication::notify(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::RequestSoftwareInputPanel)
    {
        editableItem = obj;
        qDebug()<<"input";
    }
    return QApplication::notify(obj,e);
}
