#ifndef LISTFILTERMODEL_H
#define LISTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include<QFileSystemModel>

class ListFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ListFilterModel(QObject *parent = 0);
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
signals:

public slots:

};

#endif // LISTFILTERMODEL_H
