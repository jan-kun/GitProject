#ifndef MYSTANDARDITEM_H
#define MYSTANDARDITEM_H

#include <QObject>
#include<QStandardItem>

class MyStandardItem : public QStandardItem
{
public:
    MyStandardItem();
    int ID;
};

#endif // MYSTANDARDITEM_H
