#ifndef CODEDATAFUNC_H
#define CODEDATAFUNC_H

#include <QObject>
#include<QtGui>
#include"data.h"
class CodeDataFunc : public QObject
{
    Q_OBJECT
public:
    explicit CodeDataFunc(QObject *parent = 0);
    int func_BuildData(QString name ,QString module ,QString subcode,int type,QStringList valuelist,QString SValue);
    int func_deleteData(CodeData data);

    QString func_strInsertDataDecl(CodeData data);
    QString func_calcDataDecl(CodeData data);

signals:
    void sig_hasNewData(CodeData data);
    void sig_deleteData(CodeData data);
public slots:

};

#endif // CODEDATAFUNC_H
