#include "parser.h"

GrameErr gErrTab[GERRLENGTH];
int pErr;
QMap<QString,int> ParserRes;
Parser::Parser()
{

}

Parser::~Parser()
{

}

void Parser::lrparser(void)
{
    scaner();
    if(syn == SYN_FOR)
    {
        forCmd();
    }
    else if(syn == SYN_LOOP)
    {
        loopCmd();
    }
    else if(syn == SYN_IF)
    {
        ifCmd();
    }
    else if(syn == SYN_WHILE)
    {
        whileCmd();
    }
    else if(syn == SYN_REPEAT)
    {
        repeatCmd();
    }
    else if(syn == SYN_SWITCH)
    {
        switchCmd();
    }
    else if(syn == SYN_WAIT)
    {

    }
    else if(syn == SYN_PTP)
    {

    }
    else if(syn == SYN_LIN)
    {

    }
    else if(syn == SYN_CIRC)
    {

    }
    else if(syn == SYN_SLIN)
    {

    }
    else if(syn == SYN_SCIRC)
    {

    }
    else if( (syn >= SYN_ENDIF && syn <= SYN_ENDSWITCH)||syn ==SYN_END )
    {
        Closure();
    }
    else if(syn == SYN_DECL)
        declCmd();
    else if(syn == SYN_DEF)
        defFunc();
    else if(syn >= SYN_INT && syn <= SYN_CHAR)
        declCmd();
    else if(syn == SYN_COMMENT)
        return;
    else
        return;
    return;
}

void Parser::yucu(void)
{
    statement();         /*调用函数statement();*/

    while(syn==26)
    {
        scaner();          /*读下一个单词符号*/
        if(syn!=6)
            statement();         /*调用函数statement();*/
    }

    return;
}

/*
** 函数功能：开始进行语法检查
** 函数参数：path , 待检查的文件路径
** 函数返回值：无
*/
void Parser::startParser(QString path)
{
    QString datfilepath,codefilepath;
    QFileInfo finfo(path);
    if( !finfo.isFile() )
        return;
    if(!(finfo.suffix().isEmpty() || finfo.suffix() == "dat"))
        return;

    if(symTable == NULL)
        symTable = new Symbol();
    symTable->Init_sym();
    if(finfo.suffix() == "dat")
    {
        datfilepath = path;
        codefilepath = path.remove(".dat");
        parserOneFile(datfilepath);
        return;
    }
    else if(finfo.suffix().isEmpty())
    {
        datfilepath = path+".dat";
        codefilepath = path;
    }
    parserOneFile(datfilepath);
    parserOneFile(codefilepath);
}

void Parser::parserOneFile(QString path)
{
    for(int i = 0 ; i < GERRLENGTH ; i ++)
    {
        gErrTab[i].line = 0;
        gErrTab[i].detail.clear();
    }
    pErr = 0;
    Linenum = 0;
    forlist.clear();
    forStack.clear();
    iflist.clear();
    ifStack.clear();
    whilelist.clear();
    whileStack.clear();
    looplist.clear();
    loopStack.clear();
    repeatlist.clear();
    repeatStack.clear();
    switchlist.clear();
    switchStack.clear();
    deflist.clear();
    defStack.clear();

    QFile file(path);
    if( file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&file);
        QString linestr;
        while( !in.atEnd() )
        {
            linestr.clear();
            linestr = in.readLine()+"\n";
            initParser();
            Linenum++;
            QByteArray ba = linestr.toLatin1();
            strcpy(prog,ba.data());
            lrparser();
        }
        checkClosure();
        endParser(path);
    }
}

int Parser::endParser(QString path)
{
    int res;
    if(pErr > 0)
        res = -1;
    else
        res = 1;
    ParserRes.insert(path,res);
    return res;
}

void Parser::initParser()
{
    syn = -1,p = 0,m = 0,n = 0,sum = 0;
}

/*
** 函数功能：因子
** 函数参数：无
** 函数返回值：无
*/
void Parser::factor(void)
{
    /* ID | NUM |（<表达式>）*/
    if((syn==SYN_ID)||(syn==SYN_NUM))   /*因子是ID或NUM*/
    {
        scaner();
    }
    else if(syn==SYN_LEFTBRACK)   /*因子是带括号的表达式*/
    {
        scaner();           /*读下一个单词符号*/
        expression();        /*调用函数statement();*/

        if(syn==SYN_RIGHTBRACK)
        {
            scaner();          /*读下一个单词符号*/
        }
        else
        {
            gErrTab[pErr].line = Linenum;
            gErrTab[pErr].detail = "Missing ')' ";
            pErr++;
            qDebug()<<"the error on '('"<<Linenum;
        }
    }
    else
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Invalid expression ";
        pErr++;
        qDebug()<<"the expression error!"<<Linenum;
    }

    return;
}

void Parser::logicFactor()
{
    if( syn==SYN_ID||syn==SYN_NUM||syn == SYN_BOOL)   /*因子是ID NUM BOOL IO*/
    {
        scaner();
    }
    else if(syn==SYN_LEFTBRACK)   /*因子是带括号的逻辑表达式*/
    {
        scaner();           /*读下一个单词符号*/
        logicExpress();        /*逻辑表达式*/

        if(syn==SYN_RIGHTBRACK)   /*右圆括号*/
        {
            scaner();          /*读下一个单词符号*/
        }
        else
        {
            gErrTab[pErr].line = Linenum;
            gErrTab[pErr].detail = "Missing ')' ";
            pErr++;
            qDebug()<<"the error on '('"<<Linenum;
        }
    }
    else
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Invalid logical expression ";
        pErr++;
        qDebug()<<"the err on logicExpress"<<Linenum;
    }
}

/*
** 函数功能：项
** 函数参数：无
** 函数返回值：无
*/
void Parser::term(void)
{
    /* <因子> ( *<因子> | /<因子> ) */
    factor();

    while((syn==SYN_MUTI)||(syn==SYN_DIVI))
    {
        scaner();             /*读下一个单词符号*/
        factor();              /*调用函数factor(); */
    }

    return;
}

/*
** 函数功能：逻辑项
** 函数参数：无
** 函数返回值：无
*/
void Parser::logicTerm()
{
    logicFactor();
    while(syn >= SYN_GREATER && syn <= SYN_LESSEQU)    /* > < ==  != */
    {
        scaner();
        logicFactor();
    }
    return;
}


/*
** 函数功能：表达式
** 函数参数：无
** 函数返回值：无
*/
void Parser::expression(void)
{
    /*<项> (+ <项> | - <项>) */
    term();

    while((syn==SYN_ADD)||(syn==SYN_SUBTRACT))
    {
        scaner();             /*读下一个单词符号*/
        term();               /*调用函数term();*/
    }

    return;
}

void Parser::logicExpress()
{
    logicTerm();

    while(syn == SYN_AND || syn == SYN_OR)  /* AND OR*/
    {
        scaner();
        logicTerm();
    }
    return;
}

void Parser::forCmd()
{
    forStack.insert(0,Linenum);
    /*FOR <赋值表达式> TO <表达式> STEP <表达式>*/
    scaner();
    statement();

    if(syn != SYN_TO)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "error on For statement : Missing \"TO\" ";
        pErr++;
        qDebug()<<"error on for:lose \"TO\""<<Linenum;
    }
    scaner();
    expression();

    if(syn == SYN_STEP)
    {
        scaner();
        expression();
    }
    if( syn != SYN_LINEND)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Invalid For statement ";
        pErr++;
        qDebug()<<"for error"<<Linenum;
    }
}

void Parser::loopCmd()
{
    loopStack.insert(0,Linenum);
    scaner();
    if(syn != SYN_LINEND)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Invalid For statement ";
        pErr++;
        qDebug()<<"error on loop"<<Linenum;
    }
}

void Parser::ifCmd()
{
    ifStack.insert(0,Linenum);
    scaner();
    logicExpress();

    if(syn == SYN_THEN)
        return;
    else
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Invalid IF statement : Missing \"THEN\" ";
        pErr++;
        qDebug()<<"error on if : lose \"THEN\""<<Linenum;
    }
}

void Parser::whileCmd()
{
    whileStack.insert(0,Linenum);
    scaner();
    logicExpress();

    if(syn == SYN_LINEND)
        return;
    else
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Invalid WHILE statement ";
        pErr++;
        qDebug()<<"error on while"<<Linenum;
    }
}

void Parser::repeatCmd()
{
    repeatStack.insert(0,Linenum);
    scaner();
    if(syn != SYN_LINEND)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Invalid REPEAT statement ";
        pErr++;
        qDebug()<<"error on repeat"<<Linenum;
    }
}

void Parser::switchCmd()
{
    switchStack.insert(0,Linenum);
    scaner();
    if(syn != SYN_ID)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Invalid SWITCH statement ";
        pErr++;
        qDebug()<<"error on switch"<<Linenum;
    }
    scaner();
    if(syn != SYN_LINEND)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Invalid SWITCH statement ";
        pErr++;
        qDebug()<<"error on switch"<<Linenum;
    }
}

void Parser::caseCmd()
{
    scaner();
    if(syn != SYN_NUM)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Invalid CASE statement ";
        pErr++;
        qDebug()<<"error on case"<<Linenum;
    }
}

void Parser::Closure()
{
    int up,down;
    switch (syn) {
    case SYN_ENDIF:
    {
        if( ifStack.isEmpty())
        {
            gErrTab[pErr].line = Linenum;
            gErrTab[pErr].detail = "No matching IF ";
            pErr++;
            qDebug()<<"error on \"ENDIF\""<<Linenum;
            return;
        }
        up = ifStack.takeFirst();
        down = Linenum;
        iflist.insert(0,down);
        iflist.insert(0,up);
        break;
    }
    case SYN_ENDFOR:
    {
        if( forStack.isEmpty())
        {
            gErrTab[pErr].line = Linenum;
            gErrTab[pErr].detail = "No matching FOR ";
            pErr++;
            qDebug()<<"error on \"ENDFOR\""<<Linenum;
            return;
        }
        up = forStack.takeFirst();
        down = Linenum;
        forlist.insert(0,down);
        forlist.insert(0,up);
        break;
    }
    case SYN_ENDLOOP:
    {
        if( loopStack.isEmpty())
        {
            gErrTab[pErr].line = Linenum;
            gErrTab[pErr].detail = "No matching LOOP ";
            pErr++;
            qDebug()<<"error on \"ENDLOOP\""<<Linenum;
            return;
        }
        up = loopStack.takeFirst();
        down = Linenum;
        looplist.insert(0,down);
        looplist.insert(0,up);
        break;
    }
    case SYN_ENDWHILE:
    {
        if( whileStack.isEmpty())
        {
            gErrTab[pErr].line = Linenum;
            gErrTab[pErr].detail = "No matching WHILE ";
            pErr++;
            qDebug()<<"error on \"ENDWHILE\""<<Linenum;
            return;
        }
        up = whileStack.takeFirst();
        down = Linenum;
        whilelist.insert(0,down);
        whilelist.insert(0,up);
        break;
    }
    case SYN_UNTIL:
    {
        if( repeatStack.isEmpty())
        {
            gErrTab[pErr].line = Linenum;
            gErrTab[pErr].detail = "No matching REPEAT ";
            pErr++;
            qDebug()<<"error on \"ENDPEREAT\""<<Linenum;
            return;
        }
        up = repeatStack.takeFirst();
        down = Linenum;
        repeatlist.insert(0,down);
        repeatlist.insert(0,up);
        break;
    }
    case SYN_ENDSWITCH:
    {
        if( switchStack.isEmpty())
        {
            gErrTab[pErr].line = Linenum;
            gErrTab[pErr].detail = "No matching SWITCH ";
            pErr++;
            qDebug()<<"error on \"ENDSWITCH\""<<Linenum;
            return;
        }
        up = switchStack.takeFirst();
        down = Linenum;
        switchlist.insert(0,down);
        switchlist.insert(0,up);
        break;
    }
    case SYN_END:
    {
        if( defStack.isEmpty())
        {
            gErrTab[pErr].line = Linenum;
            gErrTab[pErr].detail = "No matching DEF function_name ( ) ";
            pErr++;
            qDebug()<<"No matching DEF function_name ( )"<<Linenum;
            return;
        }
        up = defStack.takeFirst();
        down = Linenum;
        deflist.insert(0,down);
        deflist.insert(0,up);
        break;
    }
    default:
        break;
    }
}

void Parser::checkClosure()
{
    if(!ifStack.isEmpty())
    {
        gErrTab[pErr].line = ifStack.first();
        gErrTab[pErr].detail = "No matching ENDIF ";
        pErr++;
        qDebug()<<"err on IF"<<ifStack.first();
    }
    if(!forStack.isEmpty())
    {
        gErrTab[pErr].line = forStack.first();
        gErrTab[pErr].detail = "No matching ENDFOR ";
        pErr++;
        qDebug()<<"err on FOR"<<forStack.first();
    }
    if(!whileStack.isEmpty())
    {
        gErrTab[pErr].line = whileStack.first();
        gErrTab[pErr].detail = "No matching ENDWHILE ";
        pErr++;
        qDebug()<<"err on WHILE"<<whileStack.first();
    }
    if(!loopStack.isEmpty())
    {
        gErrTab[pErr].line = loopStack.first();
        gErrTab[pErr].detail = "No matching ENDLOOP ";
        pErr++;
        qDebug()<<"err on LOOP"<<loopStack.first();
    }
    if(!repeatStack.isEmpty())
    {
        gErrTab[pErr].line = repeatStack.first();
        gErrTab[pErr].detail = "No matching UNTIL ";
        pErr++;
        qDebug()<<"err on REPEAT"<<repeatStack.first();
    }
    if(!switchStack.isEmpty())
    {
        gErrTab[pErr].line = switchStack.first();
        gErrTab[pErr].detail = "No matching ENDSWITCH ";
        pErr++;
        qDebug()<<"err on SWITCH"<<switchStack.first();
    }
    if(!defStack.isEmpty())
    {
        gErrTab[pErr].line = defStack.first();
        gErrTab[pErr].detail = "No matching function END ";
        pErr++;
        qDebug()<<"No matching function END"<<defStack.first();
    }
}

void Parser::declCmd()
{
    if(syn >= SYN_INT && syn <= SYN_CHAR)
        declbasedataCmd();
    else if(syn == SYN_DECL)
    {
        scaner();
        if(syn >= SYN_INT && syn <= SYN_CHAR)
            declbasedataCmd();
        else if(syn >= SYN_E6OS && syn <=SYN_LDAT)
            declrobdataCmd();
        else
        {
            gErrTab[pErr].line = Linenum;
            gErrTab[pErr].detail = "The variable type is illegal ";
            pErr++;
            qDebug()<<"The variable type is illegal"<<Linenum;
            return;
        }
    }
    else
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Unknown data type ";
        pErr++;
        qDebug()<<"Unknown data type"<<Linenum;
    }
}

/*
** 函数功能：声明基本数据类型（int real char bool之类 ）
** 函数参数：无
** 函数返回值：无
*/
void Parser::declbasedataCmd()
{
    scaner();
    if( syn != SYN_LETTER)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Variable name is illegal ";
        pErr++;
        qDebug()<<"Variable name is illegal"<<Linenum;
        return;
    }
    char temp[TOKENSIZE];
    for(int i = 0 ; i < TOKENSIZE ; i++)
        temp[i] = token[i];
    char* name = temp;

    scaner();
    if(syn == SYN_LINEND)
    {
        symTable->val.ival = 0;
        symTable->Insert_sym(name,symTable->val);
        return;
    }
    if(syn != SYN_ASSIGN)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Variable declaration is illegal ";
        pErr++;
        qDebug()<<"Variable declaration is illegal"<<Linenum;
        return;
    }
    scaner();
    expression();
    symTable->val.ival = sum;
    symTable->Insert_sym(name,symTable->val);
}

/*
** 函数功能：声明机器人数据（位置点，坐标之类）
** 函数参数：无
** 函数返回值：无
*/
void Parser::declrobdataCmd()
{
    scaner();
    if( syn != SYN_LETTER)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Variable name is illegal ";
        pErr++;
        qDebug()<<"Variable name is illegal"<<Linenum;
        return;
    }
    char temp[TOKENSIZE];
    for(int i = 0 ; i < TOKENSIZE ; i++)
        temp[i] = token[i];
    char* name = temp;
    symTable->val.ival = 0;
    symTable->Insert_sym(name,symTable->val);
    return;
}

/*
** 函数功能：赋值指令
** 函数参数：无
** 函数返回值：无
*/
void Parser::statement(void)
{
    /*ID = <表达式>*/
    if(syn==SYN_ID)   //赋值指令以ID开头
    {
        scaner();        /*读下一个单词符号*/
        if(syn==SYN_ASSIGN)    /*ID后紧接着是赋值符号*/
        {
            scaner();      /*读下一个单词符号*/
            expression();      /*赋值号右侧是表达式*/
        }
        else                 /*如果ID后没有出现赋值语句，认为此赋值语句有语法错误*/
        {
            gErrTab[pErr].line = Linenum;
            gErrTab[pErr].detail = "Missing '=' ";
            pErr++;
            qDebug()<<"the sing \"=\" is wrong"<<Linenum;
        }
    }
    else
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Invalid Sentence! ";
        pErr++;
        qDebug()<<"wrong sentence!"<<Linenum;
    }

    return;
}

/*
** 函数功能：函数声明
** 函数参数：无
** 函数返回值：无
*/
void Parser::defFunc()
{
    defStack.insert(0,Linenum);
    scaner();
    if(syn != SYN_LETTER)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "The function name is illegal";
        pErr++;
        qDebug()<<"The function name is illegal"<<Linenum;
        return;
    }
    scaner();
    if(syn != SYN_LEFTBRACK)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Function declaration is illegal : Missing '(' ";
        pErr++;
        qDebug()<<"Function declaration is illegal : Missing '(' "<<Linenum;
        return;
    }
    scaner();
    if(syn != SYN_RIGHTBRACK)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Function declaration is illegal : Missing ')' ";
        pErr++;
        qDebug()<<"Function declaration is illegal : Missing ')' "<<Linenum;
        return;
    }
    scaner();
    if(syn != SYN_LINEND)
    {
        gErrTab[pErr].line = Linenum;
        gErrTab[pErr].detail = "Function declaration is illegal ";
        pErr++;
        qDebug()<<"Function declaration is illegal "<<Linenum;
        return;
    }
}

void Parser::scaner(void)
{
    sum=0;

    for(m=0;m<TOKENSIZE;m++)
        token[m++]=NULL;

    m=0;
    ch=prog[p++];

    while(ch==' ' || ch == '\t')
        ch=prog[p++];

    if(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A')))
    {
        while(((ch<='z')&&(ch>='a'))||((ch<='Z')&&(ch>='A'))||((ch>='0')&&(ch<='9'))||(ch == '_'))
        {
            token[m++]=ch;
            ch=prog[p++];
        }
        p--;

        token[m++]='\0';
        /*比较是否是运动指令*/
        for(n = 0 ; n < 5 ; n++)
            if(QString::compare(token,movetab[n],Qt::CaseInsensitive) == 0)
            {
                syn = n+SYN_PTP;
                return;
            }
        /*比较是否是逻辑控制语句*/
        for(n = 0 ; n < 7 ; n++)
            if(QString::compare(token,logictab[n],Qt::CaseInsensitive) == 0)
            {
                syn = n+SYN_IF;
                return;
            }
        /*比较是否是逻辑控制语句的闭包结束标志*/
        for(n = 0 ; n < 6 ; n++)
            if(QString::compare(token,closuretab[n],Qt::CaseInsensitive) == 0)
            {
                syn = n+SYN_ENDIF;
                return;
            }
        /*比较是否是基本数据类型*/
        for(n = 0 ; n < 4 ; n++)
            if(QString::compare(token,keytab[n],Qt::CaseInsensitive) == 0)
            {
                syn = n+SYN_INT;
                return;
            }
        /*比较是否是机器人数据类型*/
        for(n = 0 ; n < 4 ; n++)
            if(QString::compare(token,robdatatab[n],Qt::CaseSensitive) == 0)
            {
                syn = n+SYN_E6OS;
                return;
            }
        if(QString::compare(token,"DEF",Qt::CaseInsensitive) == 0)
        {
            syn = SYN_DEF;
            return;
        }
        if(QString::compare(token,"end",Qt::CaseInsensitive) == 0)
        {
            syn = SYN_END;
            return;
        }
        if(QString::compare(token,"decl",Qt::CaseInsensitive) == 0)
        {
            syn = SYN_DECL;
            return;
        }
        if(QString::compare(token,"and",Qt::CaseInsensitive) == 0)
        {
            syn = SYN_AND;
            return;
        }
        if(QString::compare(token,"or",Qt::CaseInsensitive) == 0)
        {
            syn = SYN_OR;
            return;
        }
        if(QString::compare(token,"case",Qt::CaseInsensitive) == 0)
        {
            syn = SYN_CASE;
            return;
        }
        if(QString::compare(token,"true",Qt::CaseInsensitive) == 0 || QString::compare(token,"false",Qt::CaseInsensitive) == 0)
        {
            syn = SYN_BOOL;
            return;
        }
        if(QString::compare(token,"to",Qt::CaseInsensitive) == 0)
        {
            syn = SYN_TO;
            return;
        }
        if(QString::compare(token,"step",Qt::CaseInsensitive) == 0)
        {
            syn = SYN_STEP;
            return;
        }
        if(QString::compare(token,"then",Qt::CaseInsensitive) == 0)
        {
            syn = SYN_THEN;
            return;
        }
        if(symTable->Query(token))
        {
            syn = SYN_ID;
            return;
        }
        syn = SYN_LETTER;
        return;
    }
    else if((ch>='0')&&(ch<='9'))
    {
        while((ch>='0')&&(ch<='9'))
        {
            sum=sum*10+ch-'0';
            ch=prog[p++];
        }
        p--;
        syn=SYN_NUM;
    }
    else
    switch(ch)
    {
        /*加减乘除运算符*/
        case '+':
            syn=SYN_ADD;
        break;
        case '-':
            syn=SYN_SUBTRACT;
        break;
        case '*':
            syn=SYN_MUTI;
        break;
        case '/':
            syn=SYN_DIVI;
        break;
        /*比较运算符*/
        case '>':
            m=0;
            ch=prog[p++];
            if(ch=='=')
            {
                syn=SYN_GREATEQU;
            }
            else
            {
                syn=SYN_GREATER;
                p--;
            }
        break;
        case '<':
            m=0;
            ch=prog[p++];
            if(ch=='=')
            {
                syn=SYN_LESSEQU;
            }
            else
            {
                syn=SYN_LESS;
                p--;
            }
        break;
        case '=':
            m=0;
            ch=prog[p++];
            if(ch=='=')
            {
                syn=SYN_EQUAL;
            }
            else
            {
                syn=SYN_ASSIGN;
                p--;
            }
            break;   
        /*左右括号*/
        case '(':
            syn=SYN_LEFTBRACK;
        break;
        case ')':
            syn=SYN_RIGHTBRACK;
        break;
        /*注释*/
        case ';':
            syn=SYN_COMMENT;
        break;
        case '\n':
            syn=SYN_LINEND;
            break;
        default:
            syn=-1;
        break;
    }
}
