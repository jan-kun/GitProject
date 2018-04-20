#ifndef PARSER_H
#define PARSER_H
#include<QtGui>
#include"syn.h"
#include"symbol.h"
#include"data.h"

#define TOKENSIZE 50
#define LINESIZE    100
#define GERRLENGTH 100  //语法错误，最多记录100条
class Parser
{
public:
    Symbol *symTable = NULL;
    char prog[LINESIZE],token[TOKENSIZE],ch;

//    char *movetab[5]={"PTP","LIN","CIRC","SLIN","SCIRC"};
//    char *keytab[4]={"int","real","bool","char"};
//    char *logictab[7]={"if","for","while","loop","repeat","switch","wait"};
//    char *closuretab[6]={"endif","endfor","endloop","endwhile","until","endswitch"};
//    char *operatortab[9]={"+","-","*","/",">","<","==",">=","<="};
//    char *robdatatab[4]={"E6POS","FDAT","PDAT","LDAT"};

    QString movetab[5]={"PTP","LIN","CIRC","SLIN","SCIRC"};
    QString keytab[4]={"int","real","bool","char"};
    QString logictab[7]={"if","for","while","loop","repeat","switch","wait"};
    QString closuretab[6]={"endif","endfor","endloop","endwhile","until","endswitch"};
    QString operatortab[9]={"+","-","*","/",">","<","==",">=","<="};
    QString robdatatab[4]={"E6POS","FDAT","PDAT","LDAT"};
    int syn,p,m,n,sum;

    int Linenum = 0;
    char *idname;
    QList<int> forlist;
    QList<int> forStack;
    QList<int> iflist;
    QList<int> ifStack;
    QList<int> whilelist;
    QList<int> whileStack;
    QList<int> looplist;
    QList<int> loopStack;
    QList<int> repeatlist;
    QList<int> repeatStack;
    QList<int> switchlist;
    QList<int> switchStack;
    QList<int> deflist;
    QList<int> defStack;

    void startParser(QString path);
    void parserOneFile(QString path);
    int endParser(QString path);
    void initParser();
    void factor(void);
    void logicFactor(void);
    void term(void);
    void logicTerm(void);
    void expression(void);
    void logicExpress(void);
    void forCmd(void);
    void loopCmd(void);
    void ifCmd(void);
    void whileCmd(void);
    void repeatCmd(void);
    void switchCmd(void);
    void caseCmd(void);
    void Closure();
    void checkClosure();
    void declCmd(void);
    void declbasedataCmd(void);
    void declrobdataCmd(void);
    void waitCmd(void);
    void ptpCmd(void);
    void linCmd(void);
    void circCmd(void);
    void slinCmd(void);
    void scircCmd(void);

    void yucu(void);
    void statement(void);
    void defFunc(void);
    void lrparser(void);
    void scaner(void);
public:
    Parser();
    ~Parser();


};

#endif // PARSER_H
