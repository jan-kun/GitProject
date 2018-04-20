#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include"Conditional.h"
#include"data.h"
#if LINUX_ARM
#include<QtGui>
#else
#include<QtWidgets>
#endif
#include <QWidget>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);
    ~CodeEditor();
    QTextBlock block;
    QTextCursor tc;
    QTextDocument *td;
    QLabel *lab_tc;   //箭头标签

    int top,bottom;

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void resizeEvent(QResizeEvent *e);
    void func_SelectLine();
    QString func_LineContent();
    int func_CursorGoLine(int line);
    int func_ArrowGoLine(int line);
    int func_ArrowMoveTFileLine(QString filepath, int line);
    void func_ArrorMoveTCursor();
    QString func_RowContents(QTextCursor temptc);
    int func_GetCMDY(QTextCursor temptc);  //获取光标所在位置的Y坐标，以更新cmdline的位置
    int func_GetArrowY(QTextCursor temptc);  //获取当前所在行的Y坐标，以让光标标签移动到此

signals:

public slots:
    void updateLineNumberAreaWidth(int );
    void updateLineNumberArea(const QRect &, int );
//    void highlightCurrentLine();

private:
    QWidget *lineNumberArea;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint()
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event)
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
