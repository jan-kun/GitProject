#include "menuwidget.h"
#include<QDebug>

MenuWidget::MenuWidget(QWidget *parent) :
    QFrame(parent)
{
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slot_TimerOut()));
    timer->start(8000);
    setFocusPolicy(Qt::ClickFocus);
}

void MenuWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint p = mapFromGlobal(event->globalPos());
    if(!rect().contains(p))
    {
        hide();
        emit aboutToHide();
        timer->stop();
    }
    else
    {
        timer->start(8000);
    }
}

void MenuWidget::showEvent(QShowEvent *event)
{
    timer->start(8000);
    emit aboutToShow(this);
}

void MenuWidget::hideEvent(QHideEvent *)
{
    emit aboutToHide();
}

void MenuWidget::focusInEvent(QFocusEvent *event)
{
    qDebug()<<"12";
    timer->start(8000);
}

void MenuWidget::slot_TimerOut()
{
    hide();
    emit aboutToHide();
    timer->stop();
}
