#include "keyboard.h"
#include<QDebug>
static int current_page=0,all_page=0;
static int cap_flag=0;

#define Btn_StyleSheet " background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 rgb(230,230,230), stop: 0.3 rgb(187,187,187), stop:1 rgb(154,154,154));border:1px solid black;border-radius:3px"
KeyBoard::KeyBoard(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags( Qt::FramelessWindowHint);
    setFixedSize(QSize(494,300));
    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    Init_KeyBoard();
    Signal_Slot();
}

void KeyBoard::Init_KeyBoard()
{
    BaseFrame=new QFrame(this);
    BaseFrame->setGeometry(0,0,494,300);
    BaseFrame->setObjectName("bf");
    BaseFrame->setStyleSheet("#bf{background-color:rgb(160,165,170);}");
    int k=0;
    for(k ; k<10 ; k++)                 //添加数字键盘
    {
        num_btn[k]=new QPushButton(BaseFrame);
        num_btn[k]->setGeometry(5+k*49,5,44,45);
        num_btn[k]->setText(QString::number(k));
        num_btn[k]->setStyleSheet(Btn_StyleSheet);
        connect(num_btn[k],SIGNAL(clicked(bool)),this,SLOT(slot_letter_btn_press()));
    }

    int i=0;                    //添加26个英文字母
    for( i ; i<10 ; i++)
    {
        letter_btn[i]=new QPushButton(BaseFrame);
        letter_btn[i]->setGeometry(5+i*49,55,44,45);
        letter_btn[i]->setText(QString(letters[i]));
        letter_btn[i]->setFocusPolicy(Qt::NoFocus);
        letter_btn[i]->setStyleSheet(Btn_StyleSheet);
        connect(letter_btn[i],SIGNAL(clicked(bool)),this,SLOT(slot_letter_btn_press()));
    }
    int j=0;
    for( i ; i<19;i++)
    {
        letter_btn[i]=new QPushButton(BaseFrame);
        letter_btn[i]->setGeometry(27+j*49,105,44,45);
        letter_btn[i]->setText(QString(letters[i]));
        letter_btn[i]->setFocusPolicy(Qt::NoFocus);
        letter_btn[i]->setStyleSheet(Btn_StyleSheet);
        connect(letter_btn[i],SIGNAL(clicked(bool)),this,SLOT(slot_letter_btn_press()));
        j++;
    }
    j=0;
    for( i ; i<26 ; i++)
    {
        letter_btn[i]=new QPushButton(BaseFrame);
        letter_btn[i]->setGeometry(54+j*49,155,44,45);
        letter_btn[i]->setText(QString(letters[i]));
        letter_btn[i]->setFocusPolicy(Qt::NoFocus);
        letter_btn[i]->setStyleSheet(Btn_StyleSheet);
        connect(letter_btn[i],SIGNAL(clicked(bool)),this,SLOT(slot_letter_btn_press()));
        j++;
    }

    cap_btn=new QPushButton("Cap",BaseFrame);
    cap_btn->setGeometry(5,155,44,45);
    cap_btn->setStyleSheet(Btn_StyleSheet);

    return_btn=new QPushButton("Enter",BaseFrame);
    return_btn->setFocusPolicy(Qt::NoFocus);
    return_btn->setGeometry(397,155,92,45);
    return_btn->setStyleSheet(Btn_StyleSheet);

    home_btn=new QPushButton("Home",BaseFrame);
    home_btn->setFocusPolicy(Qt::NoFocus);
    home_btn->setGeometry(54,205,44,45);
    home_btn->setStyleSheet(Btn_StyleSheet);

    end_btn=new QPushButton("End",BaseFrame);
    end_btn->setFocusPolicy(Qt::NoFocus);
    end_btn->setGeometry(103,205,44,45);
    end_btn->setStyleSheet(Btn_StyleSheet);

    selectRow_btn=new QPushButton("Select",BaseFrame);
    selectRow_btn->setFocusPolicy(Qt::NoFocus);
    selectRow_btn->setGeometry(152,205,44,45);
    selectRow_btn->setStyleSheet(Btn_StyleSheet);

    space_btn=new QPushButton(tr("space"),BaseFrame);
    space_btn->setGeometry(223,205,147,45);
    space_btn->setStyleSheet(Btn_StyleSheet);

    back_btn=new QPushButton("Back",BaseFrame);
    back_btn->setGeometry(397,205,44,45);
    back_btn->setFocusPolicy(Qt::NoFocus);
    back_btn->setStyleSheet(Btn_StyleSheet);

    del_btn=new QPushButton("Del",BaseFrame);
    del_btn->setGeometry(446,205,44,45);
    del_btn->setFocusPolicy(Qt::NoFocus);
    del_btn->setStyleSheet(Btn_StyleSheet);

    shift_btn=new QPushButton(tr("SYM"),BaseFrame);
    shift_btn->setGeometry(47,255,50,45);
    shift_btn->setCheckable(true);
    shift_btn->setStyleSheet("background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1,stop:0 rgb(230,230,230), stop: 0.3 rgb(187,187,187), stop:1 rgb(154,154,154));border:1px solid black;border-radius:3px;color:blue");

    closeb_btn=new QPushButton("Close",BaseFrame);
    closeb_btn->setGeometry(102,255,50,45);
    closeb_btn->setStyleSheet(Btn_StyleSheet);

    up_btn=new QPushButton(tr("U"),BaseFrame);
    up_btn->setGeometry(272,255,50,45);
    up_btn->setStyleSheet(Btn_StyleSheet);

    down_btn=new QPushButton(tr("D"),BaseFrame);
    down_btn->setGeometry(327,255,50,45);
    down_btn->setStyleSheet(Btn_StyleSheet);

    left_btn=new QPushButton(tr("L"),BaseFrame);
    left_btn->setGeometry(382,255,50,45);
    left_btn->setStyleSheet(Btn_StyleSheet);

    right_btn=new QPushButton(tr("R"),BaseFrame);
    right_btn->setGeometry(437,255,50,45);
    right_btn->setStyleSheet(Btn_StyleSheet);

}

void KeyBoard::Signal_Slot()
{
    connect(closeb_btn,SIGNAL(clicked(bool)),this,SLOT(closeb_btn_slot()));
    connect(cap_btn,SIGNAL(clicked(bool)),this,SLOT(cap_btn_slot()));
    connect(return_btn,SIGNAL(clicked(bool)),this,SLOT(return_btn_slot()));
    connect(home_btn,SIGNAL(clicked()),this,SLOT(slot_home()));
    connect(end_btn,SIGNAL(clicked()),this,SLOT(slot_end()));
    connect(space_btn,SIGNAL(clicked(bool)),this,SLOT(space_btn_slot()));
    connect(back_btn,SIGNAL(clicked()),this,SLOT(slot_back()));
    connect(del_btn,SIGNAL(clicked(bool)),this,SLOT(del_btn_slot()));
    connect(shift_btn,SIGNAL(clicked(bool)),this,SLOT(slot_shift_btn(bool)));
    connect(up_btn,SIGNAL(clicked(bool)),this,SLOT(slot_move()));
    connect(down_btn,SIGNAL(clicked(bool)),this,SLOT(slot_move()));
    connect(left_btn,SIGNAL(clicked(bool)),this,SLOT(slot_move()));
    connect(right_btn,SIGNAL(clicked(bool)),this,SLOT(slot_move()));
}

void KeyBoard::slot_shift_btn(bool state)
{
    if(state == true)
    {
        letter_btn[0]->setText(tr("+"));
        letter_btn[1]->setText(tr("-"));
        letter_btn[2]->setText(tr("*"));
        letter_btn[3]->setText(tr("/"));
        letter_btn[4]->setText(tr("{"));
        letter_btn[5]->setText(tr("}"));
        letter_btn[6]->setText(tr("["));
        letter_btn[7]->setText(tr("]"));
        letter_btn[8]->setText(tr("("));
        letter_btn[9]->setText(tr(")"));

        letter_btn[10]->setText(tr("="));
        letter_btn[11]->setText(tr(":"));
        letter_btn[12]->setText(tr(";"));
        letter_btn[13]->setText(tr(","));
        letter_btn[14]->setText(tr("<"));
        letter_btn[15]->setText(tr(">"));
        letter_btn[16]->setText(tr("."));

        letter_btn[19]->setText(tr("!"));
        letter_btn[20]->setText(tr("@"));
        letter_btn[21]->setText(tr("#"));
        letter_btn[22]->setText(tr("%"));
        letter_btn[23]->setText(tr("?"));
        letter_btn[24]->setText(tr("^"));
        letter_btn[25]->setText(tr("&"));
    }
    else
    {
        for(int i=0;i<26;i++)
            letter_btn[i]->setText(letters[i]);
    }
}


void KeyBoard::closeb_btn_slot()
{
    close();
}

void KeyBoard::del_btn_slot()
{
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier, tr(""));
    QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier, tr(""));
    emit characterFunc(keyPress , keyRelease);
}

/*大写模式*/
void KeyBoard::cap_btn_slot()
{
    cap_flag++;
    int r = cap_flag%2;
    if(r == 1)
    {
        for(int i=0;i<26;i++)
        letter_btn[i]->setText(QString(letters[i]).toUpper());
    }
    else
    {
        for(int i=0;i<26;i++)
        letter_btn[i]->setText(QString(letters[i]));

    }
}


void KeyBoard::space_btn_slot()
{
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier, tr(""));
    QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier, tr(""));
    emit characterFunc(keyPress , keyRelease);
}


void KeyBoard::slot_move()
{
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    if(btn == up_btn)
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier, tr(""));
        QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Up, Qt::NoModifier, tr(""));
        emit characterFunc(keyPress , keyRelease);
    }
    else if(btn == down_btn)
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier, tr(""));
        QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Down, Qt::NoModifier, tr(""));
        emit characterFunc(keyPress , keyRelease);
    }
    else if(btn == left_btn)
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier, tr(""));
        QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Left, Qt::NoModifier, tr(""));
        emit characterFunc(keyPress , keyRelease);
    }
    else if(btn == right_btn)
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier, tr(""));
        QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Right, Qt::NoModifier, tr(""));
        emit characterFunc(keyPress , keyRelease);
    }
}

void KeyBoard::return_btn_slot()
{
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier, tr(""));
    QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Return, Qt::NoModifier, tr(""));
    emit characterFunc(keyPress , keyRelease);
}


void KeyBoard::slot_letter_btn_press()
{
    QPushButton *btn=qobject_cast <QPushButton*>(sender());
    if(!btn)
        return;
    QChar chr = btn->text().at(0);
    emit characterGenerated(chr);
}

void KeyBoard::slot_home()
{
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Home, Qt::NoModifier, tr(""));
    QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Home, Qt::NoModifier, tr(""));
    emit characterFunc(keyPress , keyRelease);
}

void KeyBoard::slot_end()
{
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_End, Qt::NoModifier, tr(""));
    QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_End, Qt::NoModifier, tr(""));
    emit characterFunc(keyPress , keyRelease);
}

void KeyBoard::slot_back()
{
    QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, tr(""));
    QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier, tr(""));
    emit characterFunc(keyPress , keyRelease);
}

void KeyBoard::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_DragPos = e->globalPos() - frameGeometry().topLeft();
        e->accept();
    }
}

void KeyBoard::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton)
    {
        move(e->globalPos() - m_DragPos);
        e->accept();
    }
}

KeyBoard::~KeyBoard()
{

}
