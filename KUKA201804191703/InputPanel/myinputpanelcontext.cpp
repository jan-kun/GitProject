#include <QtCore>
#include "myinputpanelcontext.h"


MyInputPanelContext::MyInputPanelContext()
{
    inputPanel = new KeyBoard;
    connect(inputPanel, SIGNAL(characterGenerated(QChar)), SLOT(sendCharacter(QChar)));
    connect(inputPanel,SIGNAL(characterFunc(QKeyEvent,QKeyEvent)),this,SLOT(funcCharacter(QKeyEvent,QKeyEvent)));
}


MyInputPanelContext::~MyInputPanelContext()
{
    delete inputPanel;
}


bool MyInputPanelContext::filterEvent(const QEvent* event)
{
    if(editableItem != NULL)
    {
        if(editableItem->objectName() == "codeeditor"&&flag_codeeditor == 0)
            return false;
    }
    if (event->type() == QEvent::RequestSoftwareInputPanel)
    {
        updatePosition();
        inputPanel->exec();
        return true;
    } else if (event->type() == QEvent::CloseSoftwareInputPanel)
    {
        inputPanel->hide();
        return true;
    }
    return false;
}


QString MyInputPanelContext::identifierName()
{
    return "MyInputPanelContext";
}

void MyInputPanelContext::reset()
{
}

bool MyInputPanelContext::isComposing() const
{
    return false;
}

QString MyInputPanelContext::language()
{
    return "en_US";
}


void MyInputPanelContext::sendCharacter(QChar character)
{
//    QPointer<QWidget> w = focusWidget();

    QObject* w = editableItem;

    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
    QApplication::sendEvent(w, &keyPress);

    if (!w)
        return;

    QKeyEvent keyRelease(QEvent::KeyRelease, character.unicode(), Qt::NoModifier, QString());
    QApplication::sendEvent(w, &keyRelease);
}


void MyInputPanelContext::funcCharacter(QKeyEvent e , QKeyEvent er)
{
    QObject* w = editableItem;

    if (!w)
        return;
    QApplication::sendEvent(w, &e);

    if (!w)
        return;
    QApplication::sendEvent(w, &er);
}

void MyInputPanelContext::updatePosition()
{
    QWidget *widget = focusWidget();
    if (!widget)
        return;

    QRect widgetRect = widget->rect();
    QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
    panelPos = widget->mapToGlobal(panelPos);
//    inputPanel->move(panelPos);
}
