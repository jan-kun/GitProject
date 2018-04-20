#include "mydial.h"
#include<QDebug>
MyDial::MyDial(QWidget *parent)
    :QDial(parent)
{
    pixWidth=0;
    pixHeight=0;
}


void MyDial::setImage(QString image)
{
    this->image = image;

    QPixmap pix(image);
    pixWidth = pix.width();
    pixHeight = pix.height();
    update();
}

void MyDial::paintEvent(QPaintEvent *)
{
    if (image.isEmpty()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPixmap pix(image);
    pix = pix.scaled(pixWidth, pixHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    int pixX = rect().center().x() - pixWidth / 2;
    int pixY = rect().center().y() - pixHeight / 2 - 10;
    QPoint point(pixX, pixY);
    painter.drawPixmap(point, pix);
}

