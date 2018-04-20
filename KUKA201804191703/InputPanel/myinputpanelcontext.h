#ifndef MYINPUTPANELCONTEXT_H
#define MYINPUTPANELCONTEXT_H

#include<QtGui>
#include<QApplication>
#include<QtGui/qinputcontext.h>
#include"keyboard.h"
#include"data.h"
#include"Conditional.h"

class KeyBoard;

class MyInputPanelContext : public QInputContext
{
    Q_OBJECT

public:
    MyInputPanelContext();
    ~MyInputPanelContext();

    bool filterEvent(const QEvent* event);

    QString identifierName();
    QString language();

    bool isComposing() const;

    void reset();

private slots:
    void sendCharacter(QChar character);
    void funcCharacter(QKeyEvent e, QKeyEvent er);

private:
    void updatePosition();

private:
    KeyBoard *inputPanel;
};


#endif // MYINPUTPANELCONTEXT_H
