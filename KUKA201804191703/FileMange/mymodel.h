#ifndef MYMODEL_H
#define MYMODEL_H

#include <QFileSystemModel>
#include"data.h"

class MyModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit MyModel(QObject *parent = 0);

    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
signals:

public slots:

};

#endif // MYMODEL_H
