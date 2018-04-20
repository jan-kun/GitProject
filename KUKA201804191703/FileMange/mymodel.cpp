#include "mymodel.h"

MyModel::MyModel(QObject *parent) :
    QFileSystemModel(parent)
{
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DecorationRole)
    {
        if(index.column() == 0)
        {
            QFileInfo info = MyModel::fileInfo(index);
            if(info.isFile())
            {
                int res = 1;
                if(ParserRes.contains(info.absoluteFilePath()))
                    res = ParserRes.value(info.absoluteFilePath());
                if(KukaRob.User == Operator)
                {
                    if(info.suffix().isEmpty())
                    {
                        if(res == 1)
                            return QPixmap(":/image/image/module.png");
                        else
                            return QPixmap(":/image/image/errmodule.png");
                    }
                }
                else
                {
                    if(info.suffix().isEmpty())
                    {
                        if(res == 1)
                            return QPixmap(":/image/image/src.png");
                        else
                            return QPixmap(":/image/image/errsrc.png");
                    }
                    else if(info.suffix() == "dat")
                    {
                        if(res == 1)
                            return QPixmap(":/image/image/dat.png");
                        else
                            return QPixmap(":/image/image/errdat.png");
                    }

                }
            }
            else if(info.isDir())
            {
                if(info.baseName() == "MAIZI-PC")
                    return QPixmap(":/image/image/robot.png");
                else if(info.baseName() == "(ARCHIVE:\\)")
                    return QPixmap(":/image/image/archi.png");
                else
                    return QPixmap(":/image/image/folder.png");
            }

        }
    }

    return QFileSystemModel::data(index, role);
}
