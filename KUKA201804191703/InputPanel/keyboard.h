#ifndef KEYBOARD_H
#define KEYBOARD_H

#include"Conditional.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif

#include <QMainWindow>
#include<string.h>
#define LETTER 1
#define NUM 2
#define HANZI 3
#define ZIKU ":/source/source/ziku.data"
class KeyBoard : public QDialog
{
    Q_OBJECT

public:
    KeyBoard(QWidget *parent = 0);
    QFrame *BaseFrame;
    int mode_flag=LETTER;          //设置mode
    QPoint  m_DragPos;

    QLineEdit *edit;
    QPushButton *previous_btn,*next_btn,*del_btn,*back_btn,*closeb_btn,*return_btn;
    QPushButton *cap_btn,*space_btn,*shift_btn,*home_btn,*end_btn,*selectRow_btn;
    QPushButton *pun_btn0,*pun_btn1,*pun_btn2,*pun_btn3;
    QPushButton *up_btn,*down_btn,*right_btn,*left_btn;
    QPushButton * letter_btn[26];
    QPushButton * num_btn[10];

    QStringList str_list;
    QString out_letter;
    QChar letters[26]={'q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m'};
    QString pinyin="";
    QTextCursor tc;

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void Init_KeyBoard(void);
    void Signal_Slot();

private slots:
        void closeb_btn_slot();
        void return_btn_slot();
        void cap_btn_slot();
        void space_btn_slot();
        void slot_move();
        void slot_shift_btn(bool state);
        void slot_letter_btn_press();
        void slot_home();
        void slot_end();
        void del_btn_slot();
        void slot_back();

signals:
        void characterGenerated(QChar);
        void characterFunc(QKeyEvent e,QKeyEvent er);
public:
    ~KeyBoard();
};

#endif // KEYBOARD_H
