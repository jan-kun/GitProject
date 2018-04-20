#include "my_inputdialog.h"

My_InputDialog::My_InputDialog(QWidget *parent) :
    QWidget(parent)
{
    Init_Interface();
}

void My_InputDialog::Init_Interface()
{
    QWidget *widget = new QWidget(this);
    lab_tip=new QLabel(widget);
    lab_tip->resize(80,30);
    line_input=new QLineEdit(widget);
    line_input->resize(150,30);
    btn_cancel=new QPushButton(tr("Cancel"),widget);
    btn_cancel->resize(60,30);
    btn_ok=new QPushButton(tr("OK"),widget);
    connect(btn_ok,SIGNAL(clicked(bool)),this,SLOT(slot_btn_ok()));
    layout_dialog=new QGridLayout(widget);
    layout_dialog->addWidget(lab_tip,0,0,1,2);
    layout_dialog->addWidget(line_input,1,0,1,2);
    layout_dialog->addWidget(btn_cancel,2,0,1,1);
    layout_dialog->addWidget(btn_ok,2,1,1,1);
    widget->setLayout(layout_dialog);
    this->hide();
}

void My_InputDialog::set_text(QString title,QString label,QString linetext)
{
    setWindowTitle(title);
    lab_tip->setText(label);
    line_input->setText(linetext);
}

QString My_InputDialog::show_Dialog()
{
    line_input->setText(tr("新建文件夹"));
    show();
}

/*新建程序对话框上点击OK按钮后，发出“创建文件的信号”*/
void My_InputDialog::slot_btn_ok()
{
    hide();
}

void My_InputDialog::slot_btn_cancel()
{
    hide();
}

