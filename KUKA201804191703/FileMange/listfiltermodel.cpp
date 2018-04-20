#include "listfiltermodel.h"

ListFilterModel::ListFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool ListFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QFileSystemModel *model = (QFileSystemModel*)sourceModel();
    QModelIndex source_index = model->index(source_row, 0, source_parent);
    if (model->isDir(source_index))
        return true;
    else
        return QSortFilterProxyModel::filterAcceptsRow(source_row,source_parent);
}
