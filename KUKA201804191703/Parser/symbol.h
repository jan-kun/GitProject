#ifndef SYMBOL_H
#define SYMBOL_H

#include<iostream>
#include<string>
#include<fstream>
#include"math.h"
#include<string.h>

#define SIZE 500
class Symbol
{
public:
    Symbol();
    typedef union
    {
        int ival;
        float rval;
        bool bval;
        char cval;
    }value;

    typedef struct sym // construct of symbol
    {
        char name[20]; // the name of symbol
        int state; // 用于判断符号是否在符号表中，如果state等于1，则符号表中该位置已存储，若为0，则该位置尚未存储
        value sym_value;  //值------------
        int addr; // 表示符号的地址
    }My_Sym;

    My_Sym symbol_table[SIZE];
    int records;    //表示表中当前记录的个数
    int m,p;
    value val;

    void Init_sym();  //初始化整个符号表
    int Hash(char* key);   //哈希映射
    bool Insert_sym(char* name, value pValue);  //添加符号
    bool Query(char* name);  //查询符号是否已被创建
    bool Delete_sym(char* name);   //删除符号
};

#endif // SYMBOL_H
