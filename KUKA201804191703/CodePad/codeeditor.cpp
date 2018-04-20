#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    /*设置程序编辑器中选中的文本前景色为白色，背景色为蓝色*/
    setStyleSheet(" QPlainTextEdit { selection-color: red;selection-background-color:rgb(230,230,230);color:red; }");
    tc = textCursor();
    td = document();
    td->setDocumentMargin(15);  //文档四周宽度
    setDocument(td);
    setLineWrapMode(QPlainTextEdit::NoWrap);

    lineNumberArea = new LineNumberArea(this);
    lineNumberArea->setStyleSheet("background-color:white");
    lab_tc = new QLabel(this);
    lab_tc->resize(18,15);
    lab_tc->setPixmap(QPixmap(":/image/image/tc_arrow.png"));
    lab_tc->hide();

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    updateLineNumberAreaWidth(0);
    setObjectName("codeeditor");
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(2, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignHCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits + 20;

    return space;
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

/*
** 函数功能：选中光标所在行
** 函数参数：无
** 函数返回值：无
*/
void CodeEditor::func_SelectLine()
{
    QTextCursor temptc = textCursor();
    temptc.movePosition(QTextCursor::StartOfBlock , QTextCursor::MoveAnchor);
    temptc.movePosition(QTextCursor::EndOfBlock , QTextCursor::KeepAnchor);
    setTextCursor(temptc);
}

QString CodeEditor::func_LineContent()
{
    QTextCursor temptc = textCursor();
    if(temptc.hasSelection())
        temptc.setPosition(temptc.anchor() , QTextCursor::MoveAnchor);
    temptc.movePosition(QTextCursor::StartOfBlock , QTextCursor::MoveAnchor);
    temptc.movePosition(QTextCursor::EndOfBlock , QTextCursor::KeepAnchor);
    return temptc.selectedText();
}

/*
** 函数功能：将光标移动到指定行
** 函数参数：行数
** 函数返回值：无
*/
int CodeEditor::func_CursorGoLine(int line)
{
    int total = blockCount();
    if(line < 1 || line > total)
        return -1;
    QTextBlock block = document()->findBlockByNumber(line);
    setTextCursor(QTextCursor(block));
    return 1;
}

/*
** 函数功能：将光标图标移动到指定行
** 函数参数：行数
** 函数返回值：移动成功返回1，失败返回-1
*/
int CodeEditor::func_ArrowGoLine(int line)
{
    if( func_CursorGoLine(line) == -1 )
        return -1;
    QTextBlock block = textCursor().block();
    int y=(int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int b=y+(int) blockBoundingRect(block).height()-15;
    lab_tc->move(lineNumberAreaWidth(),b);
    lab_tc->show();
    return 1;
}

/*
** 函数功能：将光标图标移动到指定文件指定行
** 函数参数：filepath，文件路径；line，行数
** 函数返回值：成功 : 1，失败 : -1
*/
int CodeEditor::func_ArrowMoveTFileLine(QString filepath, int line)
{
    QFile file(filepath);
    if ( !file.exists())
        return -1;
    bool ok = file.open( QIODevice::ReadOnly );
    if( !ok )
        return -1;
    QTextStream in(&file);
    clear();
    while(!in.atEnd())
    {
        this->appendPlainText(in.readLine());
    }

    if(func_ArrowGoLine(line) == -1)
    {
        file.close();
        return -1;
    }
    file.close();
    return 1;
}

int func_CursorGoStr(QPlainTextEdit* edit , QString str)
{
    int pos = edit->textCursor().position();   //记录初始位置
    edit->moveCursor(QTextCursor::End);
    if(edit->find(str,QTextDocument::FindBackward | QTextDocument::FindCaseSensitively | QTextDocument::FindWholeWords))
    {
        QTextCursor temptc = edit->textCursor();
        temptc.clearSelection();
        temptc.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
        edit->setTextCursor(temptc);
        return 0;
    }
    else
    {
        QTextCursor temptc = edit->textCursor();
        temptc.setPosition(pos,QTextCursor::MoveAnchor);
        edit->setTextCursor(temptc);  //回到初始位置
        return -1;
    }
}

/*
** 函数功能：将箭头移至光标所在行
** 函数参数：无
** 函数返回值：无
*/
void CodeEditor::func_ArrorMoveTCursor()
{
    QTextCursor temptc = textCursor();
    QTextBlock block = temptc.block();
    int y=(int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int b=y+(int) blockBoundingRect(block).height()-15;
    lab_tc->move(lineNumberAreaWidth(),b);
    lab_tc->show();
}

/*
** 函数功能：返回当前光标所在一整行的内容
** 函数参数：所在行光标
** 函数返回值：这行字符
** 函数说明：只是返回这一行的内容，并不会选定这一行
*/
QString CodeEditor::func_RowContents(QTextCursor temptc)
{
    temptc.movePosition(QTextCursor::StartOfBlock , QTextCursor::MoveAnchor);
    temptc.movePosition(QTextCursor::EndOfBlock , QTextCursor::KeepAnchor);
    return temptc.selectedText().simplified();
}

/*
** 函数功能：计算cmdline应该出现位置的Y坐标
** 函数参数：将要插入命令的所在行光标
** 函数返回值：cmdline出现的Y坐标
** 函数说明：此函数应该要考虑到在文本末端插入时，cmdline应该出现在插入行上方
*/
int CodeEditor::func_GetCMDY(QTextCursor temptc)
{
    int t,b;
    QTextBlock tempblock = temptc.block();
    t = (int) blockBoundingGeometry(tempblock).translated(contentOffset()).top();
    b = t + (int) blockBoundingRect(tempblock).height();
    if( b > (height()-80) )
    {
        b = t - (int) blockBoundingRect(tempblock).height()-50;
    }
    return b;
}


int CodeEditor::func_GetArrowY(QTextCursor temptc)
{
    int t,b;
    QTextBlock tempblock = temptc.block();
    t = (int) blockBoundingGeometry(tempblock).translated(contentOffset()).top();
    b = t + (int) blockBoundingRect(tempblock).height()-15;
    return b;
}

void CodeEditor::updateLineNumberAreaWidth(int )
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    /*用来设置插入命令框的位置，根据当前文本块所在矩形，计算出坐标*/
    block = textCursor().block();
    /*blockBoundingGeometry():Returns the bounding rectangle of the text block in content coordinates.
     * Translate the rectangle with the contentOffset() to get visual coordinates on the viewport.
     * 经过实验，发现blockBoundingGeometry（block）返回的是
     * 当前文本块block矩形在整个文档中的绝对位置，
     * 即文本块下移的时候其Y坐标会每次增加一个文本块的宽度
    */
    top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    /*Returns the bounding rectangle of the text block in the block's own coordinates.
     *所以说blockBoundingRect(block)函数和blockBoundingGeometry(block)函数是有区别的，
     * blockBoundingRect(block)函数返回的是文本块在当前文本块中的位置，
     *而不是在文档中的位置，所以其XY坐标都是0
     *不会随着文本块的移动而改变其坐标值
    */
    bottom = top + (int) blockBoundingRect(block).height();
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
    {
        lineNumberArea->scroll(0, dy);
        lab_tc->move(lineNumberAreaWidth() , lab_tc->y()+dy);
    }

    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}
CodeEditor::~CodeEditor()
{

}
