#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QFrame>
#include<QMouseEvent>
#include<QTimer>
class MenuWidget : public QFrame
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = 0);

    QTimer *timer;
    void mousePressEvent(QMouseEvent *event);
    void showEvent( QShowEvent * event);
    void hideEvent(QHideEvent *);
    void focusInEvent(QFocusEvent * event);

signals:
    void aboutToHide();
    void aboutToShow(MenuWidget* wig);

public slots:
    void slot_TimerOut();
};

#endif // MENUWIDGET_H
