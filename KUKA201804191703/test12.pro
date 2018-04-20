#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T10:03:54
#
#-------------------------------------------------

QT       += core gui
CONFIG += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test12
TEMPLATE = app

INCLUDEPATH += \
    Decode/file/    \
    Decode/data/    \
    Decode/common/  \
    Decode/command/  \
    Decode/hmi/  \

SOURCES += main.cpp\
        kuka.cpp \
    IO/dio.cpp \
    AIO/aio.cpp \
    Position/pos.cpp \
    INFO/info.cpp \
    INFO/mystandarditem.cpp \
    MyDial/mydial.cpp \
    Build_TCP/build_tcp.cpp \
    Build_Base/build_base.cpp \
    Port/port.cpp \
    Protocol/robot.cpp \
    Protocol/message.cpp \
    Protocol/io.cpp \
    Protocol/crc16.cpp \
    Protocol/bus.cpp \
    Protocol/auto.cpp \
    MyWidget/menuwidget.cpp \
    CodePad/cmd_line.cpp \
    CodePad/codeeditor.cpp \
    CodePad/codepad.cpp \
    FileMange/filemange.cpp \
    FileMange/my_inputdialog.cpp \
    FileMange/mymodel.cpp \
    InputPanel/keyboard.cpp \
    globalapplication.cpp \
    InputPanel/myinputpanelcontext.cpp \
    UserGroup/usergroup.cpp \
    SigVar/sigvar.cpp \
    Build_TCP/tcp_in.cpp \
    Build_Base/base_in.cpp \
    MenuBar/menubar.cpp \
    Power/power.cpp \
    Protocol/error.cpp \
    Protocol/RobConvert.cpp \
    Protocol/vm.cpp \
    thread/NCKThread.cpp \
    UpdateSys/updatesys.cpp \
    FileMange/listfiltermodel.cpp \
    CodePad/codedatafunc.cpp \
    Decode/command/code2psd.cpp \
    Decode/command/mdlenter.cpp \
    Decode/common/base_opt.cpp \
    Decode/common/initbuff.cpp \
    Decode/common/macro.cpp \
    Decode/common/var.cpp \
    Decode/data/get_data.cpp \
    Decode/data/get_var.cpp \
    Decode/data/var_init.cpp \
    Decode/file/open_file.cpp \
    Decode/formula/formula_opt.cpp \
    Decode/hmi/code2hmi.cpp \
    CodePad/gramerr.cpp \
    Parser/parser.cpp \
    Parser/symbol.cpp

HEADERS  += kuka.h \
    IO/dio.h \
    AIO/aio.h \
    Position/pos.h \
    INFO/info.h \
    INFO/mystandarditem.h \
    MyDial/mydial.h \
    Conditional.h \
    Build_TCP/build_tcp.h \
    Build_Base/build_base.h \
    Port/port.h \
    Protocol/baselcd.h \
    Protocol/bus.h \
    Protocol/crc16.h \
    Protocol/linuxtypes.h \
    Protocol/rtype.h \
    Protocol/robot.h \
    Protocol/IO.h \
    Protocol/auto.h \
    Protocol/message.h \
    Protocol/pop.h \
    Protocol/error.h \
    Protocol/autolib.h \
    Protocol/variable.h \
    Protocol/transform.h \
    MyWidget/menuwidget.h \
    CodePad/cmd_line.h \
    CodePad/codeeditor.h \
    CodePad/codepad.h \
    FileMange/filemange.h \
    FileMange/my_inputdialog.h \
    FileMange/mymodel.h \
    data.h \
    InputPanel/keyboard.h \
    globalapplication.h \
    InputPanel/myinputpanelcontext.h \
    UserGroup/usergroup.h \
    SigVar/sigvar.h \
    Build_TCP/tcp_in.h \
    Build_Base/base_in.h \
    MenuBar/menubar.h \
    Power/power.h \
    Protocol/RobConvert.h \
    Protocol/vm.h \
    thread/NCKThread.h \
    UpdateSys/updatesys.h \
    FileMange/listfiltermodel.h \
    CodePad/codedatafunc.h \
    Decode/command/code2psd.h \
    Decode/command/mdlenter.h \
    Decode/common/base_opt.h \
    Decode/common/initbuff.h \
    Decode/common/macro.h \
    Decode/common/var.h \
    Decode/data/get_data.h \
    Decode/data/get_var.h \
    Decode/data/var_init.h \
    Decode/file/open_file.h \
    Decode/formula/formula_opt.h \
    Decode/hmi/code2hmi.h \
    CodePad/gramerr.h \
    Parser/parser.h \
    Parser/symbol.h \
    Parser/syn.h
    


RESOURCES += \
    img.qrc
