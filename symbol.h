#pragma once

#include "common.h"
#include "ast.h"
#include <ext/hash_map>

using namespace __gnu_cxx;

class Struct;
class Typedef;
class Var;
class Scope;
class Func;

class Struct
{
    //hash函数
	struct string_hash{
		size_t operator()(const string& str) const{
			return __stl_hash_string(str.c_str());
		}
	};

    string name;
    //vector<Tp*> &tp_list;
    //vector<string> &name_list;
    vector<Var*> var_list;
    hash_map<string, int, string_hash> offset_list;
    int size;

public:
    Struct(string name, vector<Tp*> &tp_list, vector<string> &fname_list);
    ~Struct();

    string get_name();
    void set_name(string name);
    int get_size();
    int get_offset(string fname);
};

class Typedef
{
    string name;         // 变量名
    Tp *tp;            // 变量类型
    int size;
public:
    Typedef(Tp *tp, string name);
    ~Typedef();

    string get_name();
    void set_name(string name);
};

class Var
{
    enum BaseType
    {
        INT,
        CHAR,
        BOOL,
        STRUCT,
        STRING
    };
    const static int int_size = 4;
    const static int char_size = 2;
    const static int bool_size = 2;
    const static int pointer_size = 8;  //64位
    const static int string_size = 8; //char*

    Tp *tp;            // 变量类型
    BaseType base_type;
    
    Scope *scope; // 作用域路径
    string name;         // 变量类型
    int base_size = -1;    //基本类型大小 单位字节
    
    int size = -1; //分配空间大小 单位字节

    bool is_pointer = false;
    int pointer_deep = 0;

    bool is_array = false;
    int array_deep = 0;
    vector<int> array_size;

    bool is_struct = false;
    Struct* stru;

    bool is_literal;           // 是否字面量
    union
    {
        int int_var;
        char chr_var;
        bool bool_var;
        string str_var;
    };
    //string ptrVal; //初始化字符指针常量字符串的名称
    Var *ptr;
public:

    Var();
    //tp 和 name 生成 Var；
    Var(Tp* tp, string name); 
    ~Var(); 

    string get_name();
    void set_name(string name);
    string get_str();
    int get_size();
    int get_offset(vector<int> &array_index);
    //static bool tp_equal(Var* a, Var* b);
};

class Scope
{
    int scope_id;
    Scope *parent;
    int size = -1;

    //hash函数
	struct string_hash{
		size_t operator()(const string& str) const{
			return __stl_hash_string(str.c_str());
		}
	};

    hash_map<string, Var*, string_hash> var_tab; //变量表,每个元素是同名变量的链表
	hash_map<int, Scope*> scope_tab;
    
public:
    Scope(int id);
    ~Scope();
    void add_var(Var* var);
    Var* get_var(string name);
    int get_size();
};


class Func
{
    Tp* tp;
    Scope* scope;
    string name;
    int func_id;
    vector<Var*>paraVar;
    InterInst* LFB;
    InterInst* LFE;
    InterCode* ir;

public:
    Func(/* args */);
    ~Func();
};
