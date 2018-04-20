#include "symbol.h"

Symbol::Symbol()
{
    Init_sym();
}

/*
** 函数功能：初始化符号表
** 函数参数：无
** 函数返回值：无
*/
void Symbol::Init_sym()
{
    char* init = "NULL"; // init means the name's original state
    for (int i = 0; i < SIZE; i++)
    {
        memcpy(symbol_table[i].name , init ,20);
        symbol_table[i].state = 0; // 0 means this location is empty
    }
}

/*
** 函数功能：对符号表名称进行哈希映射
** 函数参数：无
** 函数返回值：hash地址
** 函数说明：根据名称映射到对应的地址
*/
int Symbol::Hash(char *key)
{
    int address = 0; // the address of symbol
    int count = 0;
    while (key[count] != '\0')
    {
        address = (address + static_cast<int>(key[count]));// Hash founction
        count++;
    }
    return address % SIZE;
}

/*
** 函数功能：添加一个符号表ID
** 函数参数：name，变量名称；pValue，变量值
** 函数返回值：成功，true；失败，false
** 函数说明：无
*/
bool Symbol::Insert_sym(char *name , value pValue)
{
    if (records == SIZE)
        return false;
    int location = Hash(name); // 计算该符号的存储位置
    if(location < 0 || location >= SIZE)
        return false;
    /* 冲突处理：如果该位置已经有符号，则存储位置后移一个。如果没有，则存储在该位置 */
    while (symbol_table[location].state == 1)
        location = (location + 1) % SIZE; // 防止自加后超过符号表的限定范围
    memcpy(symbol_table[location].name , name ,strlen(name)+1);
    symbol_table[location].sym_value = pValue;  //存放符号的值
    symbol_table[location].state = 1; // 将该位置状态标注为 1 ， 表示该位置已存储
    symbol_table[location].addr = location;
    records++; // 存储好后，记录加 1
    return true; // 存储成功
}

/*
** 函数功能：删除一个符号表ID
** 函数参数：name，变量名称
** 函数返回值：成功，true；失败，false
** 函数说明：无
*/
bool Symbol::Delete_sym(char *name)
{
    int location = Hash(name);
    if(location < 0 || location >= SIZE)
        return false;
    if(symbol_table[location].state == 0)
        return false;
    if( !(strcmp(symbol_table[location].name  , name)) )
    {
        symbol_table[location].state = 0; // 状态标示为0，为未存储状态
        records--;
        return true;
    }
    int counter = 0; // 用于下面计数
    while ( (strcmp(symbol_table[location].name  , name) ) && counter < SIZE)
    {
        location = (location+1) % SIZE; // % SIZE 防止自加后超出符号表的大小范围
        counter++; // 防止符号表满了，每个符号状态都为1，但是所查找的符号不在里面
    }
    if (counter == SIZE)
        return false;
    else
    {
        symbol_table[location].state = 0; // 状态标示为0，为未存储状态
        records--;
        return true;
    }
}

/*
** 函数功能：查询该变量名称是否已经加入进符号表中
** 函数参数：name，变量名称
** 函数返回值：是，true；不是，false
** 函数说明：无
*/
bool Symbol::Query(char *name)
{
    int location = Hash(name);
    if(location < 0 || location >= SIZE)
        return false;
    if (symbol_table[location].state == 0)
        return false;
    if(!(strcmp(symbol_table[location].name  , name)))
        return true;
    /* 该位置已存储，但存储的不是要查找的符号。说明该符号插入时发生冲突，已经被后移。所以相后查找该符号*/
    int counter = 0; // 用于下面计数
    while ( (strcmp(symbol_table[location].name  , name) ) && counter < SIZE)
    {
        location = (location+1) % SIZE; // % SIZE 防止自加后超出符号表的大小范围
        counter++; // 防止符号表满了，每个符号状态都为1，但是所查找的符号不在里面
    }
    if (counter == SIZE)
        return false;
    else
        return true;
}

