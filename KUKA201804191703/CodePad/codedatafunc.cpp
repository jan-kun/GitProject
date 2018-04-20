#include "codedatafunc.h"

CodeDataFunc::CodeDataFunc(QObject *parent) :
    QObject(parent)
{
}

/*
** 函数功能：创建一个程序数据
** 函数参数：数据所需信息
** 函数返回值：成功，1；失败，0
** 函数说明：无
*/
int CodeDataFunc::func_BuildData(QString name ,QString module ,QString subcode,int type,QStringList valuelist,QString SValue)
{
    if( name.isEmpty() )
        return 0;
    if( func_JudgeDataExist( name ) == 1)   //名称已存在
    {
        QMessageBox::warning(NULL,tr("RAPID 数据处理错误"),\
                             tr("当前声明无效。\n\n请检查变量名称是否为保留文字。还应检查变量名称是否已用于程序,并检查数据类型在选定范围内是否有效"),\
                             tr("确定"));
        return 0;
    }
    if(type < 0 || type >= CNT_Type)  //数据类型不合法
        return 0;

    int res;
    CodeData tempdata;
    tempdata.name = name;
    tempdata.Svalue = SValue;
    tempdata.dtype = ( CodeDataType )type;
    tempdata.module = module;
    tempdata.code = subcode;
    tempdata.dimension = 1;
    switch (type) {
    case 0:
        res = tempdata.val.boolVal.setboolvalue(valuelist);
        break;
    case 1:
        res = tempdata.val.intVal.setintval(valuelist);
        break;
    case 2:
        res = tempdata.val.realVal.setrealval(valuelist);
        break;
    case 3:
        res = tempdata.val.charVal.setcharval(valuelist);
        break;
    case 4:
        res = tempdata.val.axisVal.setaxisval(valuelist);
        break;
    case 5:
        res = tempdata.val.e6axisVal.sete6axisval(valuelist);
        break;
    case 6:
        res = tempdata.val.frameVal.setframeval(valuelist);
        break;
    case 7:
        res = tempdata.val.posVal.setposval(valuelist);
        break;
    case 8:
        res = tempdata.val.e6posVal.sete6posval(valuelist);
        break;
    case 9:
        res = tempdata.val.fdatVal.setfdatval(valuelist);
        break;
    case 10:
        res = tempdata.val.pdatVal.setpdatval(valuelist);
        break;
    case 11:
        res = tempdata.val.ldatVal.setldatval(valuelist);
        break;
    default:
        break;
    }
    if(res < 0)
        return 0;
    codedata[type].append(tempdata);
    emit sig_hasNewData(tempdata);
    return 1;
}

/*
** 函数功能：删除一个程序数据
** 函数参数：程序数据
** 函数返回值：成功，1；失败，-1
** 函数说明：无
*/
int CodeDataFunc::func_deleteData(CodeData data)
{
    QString name,type;
    int index,idtype;
    name = data.name;
    idtype = (int)data.dtype;
    if(idtype < 0 || idtype >= CNT_Type)
        return -1;
    index = func_getDataIndex(name,type);
    if(index < 0)
        return -1;
    codedata[idtype].takeAt(index);
    emit sig_deleteData(data);
    return 1;
}

/*
** 函数功能：计算数据在文件中的声明字符串
** 函数参数：data，程序数据
** 函数返回值：数据的声明
** 函数说明：无
*/
QString CodeDataFunc::func_strInsertDataDecl(CodeData data)
{
    CodeDataType type;
    QString name,value;
    QString insertstr = "\rDECL ";
    type = data.dtype;
    name = data.name;
    value = data.Svalue;
    switch (type) {
    case booltype:

        break;
    case inttype:
        insertstr += tr("INT  ");
        break;
    case realtype:

        break;
    case chartype:

        break;
    case axistype:

        break;
    case e6axistype:

        break;
    case frametype:

        break;
    case postype:

        break;
    case e6postype:
        insertstr += "E6POS X"+name+" = ";
        break;
    case fdattype:
        insertstr += "FDAT "+name+" = ";
        break;
    case pdattype:
        insertstr += "PDAT P"+name+" = ";
        break;
    case ldattype:
        insertstr += "LDAT L"+name+" = ";
        break;
    case basedattype:

        break;
    case loaddattype:

        break;
    case tooldatatype:

        break;
    default:
        break;
    }
    insertstr += value;
    return insertstr;
}

/*
** 函数功能：根据程序数据找到其在文件中的声明
** 函数参数：程序数据
** 函数返回值：返回其声明
** 函数说明：无
*/
QString CodeDataFunc::func_calcDataDecl(CodeData data)
{
    CodeDataType type;
    QString name,value;
    QString insertstr = "DECL ";
    type = data.dtype;
    name = data.name;
    value = data.Svalue;
    switch (type) {
    case booltype:

        break;
    case inttype:
        insertstr += tr("INT  ");
        break;
    case realtype:

        break;
    case chartype:

        break;
    case axistype:

        break;
    case e6axistype:

        break;
    case frametype:

        break;
    case postype:

        break;
    case e6postype:
        insertstr += "E6POS X"+name+" = ";
        break;
    case fdattype:
        insertstr += "FDAT "+name+" = ";
        break;
    case pdattype:
        insertstr += "PDAT P"+name+" = ";
        break;
    case ldattype:
        insertstr += "LDAT L"+name+" = ";
        break;
    case basedattype:

        break;
    case loaddattype:

        break;
    case tooldatatype:

        break;
    default:
        break;
    }
    insertstr += value;
    return insertstr;
}
