#ifndef MYDIAL_H
#define MYDIAL_H

#include <QObject>
#include <QWidget>
#include<QDial>
#include<QPaintEvent>
#include<QPainter>

class MyDial : public QDial
{
    Q_OBJECT
public:
    MyDial(QWidget *parent = 0);
    QString image;
    int pixWidth;                       //图片宽度
    int pixHeight;                      //图片高度

    void setImage(QString image);
protected:
    void paintEvent(QPaintEvent *);
};

#endif // MYDIAL_H
