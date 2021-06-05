#pragma once

#include "common.h"

#include "ast.h"
class Var
{
private:
    bool literal;           // 是否字面量
    vector<int> scope_path; // 作用域路径

    bool is_typedef_aid; // 是否为typedef aid
    Tp *type;            // 变量类型
    string name;         // 变量类型

    bool is_pointer;
    bool is_array;
    int array_size;
    union
    {
        int int_var;
        char str_var;
        bool bool_var;
        string str_var;
    };

    string ptrVal; //初始化字符指针常量字符串的名称

    Var *ptr;

public:
    Var(Tp *tp, string name); //typedef aid
    Var();

    ~Var(); //
    string get_name();
    void set_name(string name);
};

class Func
{
private:
    /* data */
public:
    Func(/* args */);
    ~Func();
};
