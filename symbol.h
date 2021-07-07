#pragma once

#include "common.h"
#include "ast.h"
#include "intercode.h"
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
    hash_map<string, Var*, string_hash> var_tab;
    hash_map<string, int, string_hash> offset_tab;
    int size;

public:
    Struct(string name, vector<Tp*> &tp_list, vector<string> &fname_list);
    ~Struct();

    string get_name();
    void set_name(string name);
    Var *get_var(string fname);
    int get_size();
    int get_offset(string fname);
    void print();
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
    Tp* get_tp();
    void print();
};

class Var
{
public:
    enum BaseType
    {
        INT,
        CHAR,
        BOOL,
        STRUCT,
        STRING,
        POINTER
    };
private:
    const static int int_size = 4;
    const static int char_size = 2;
    const static int bool_size = 2;
    const static int pointer_size = 4;  //64位
    const static int string_size = 0; //

    Var::BaseType base_type;
    
    Tp *tp;            // 变量类型
    
    Scope *scope; // 作用域路径
    string name;         // 变量类型

    int base_size;    //基本类型大小 单位字节 单位大小或单位结构体大小
    // 
    int size; //分配空间大小 单位字节
    
    //几维数组
    int array_dim;
    //正数为数组 定义信息, 表示大小
    vector<int> array_sizes;
    int pointer_deep;
    //对应scope
    int s_offset;
    //对于func
    int f_offset;
    bool is_struct;
    Struct* stru;

    bool literal;           // 是否字面量
    bool ref; // 是否为引用
    bool is_temp;
    bool is_offset_var;
    bool left;

    union
    {
        int int_var;
        char chr_var;
        bool bool_var;
        string str_var;
    };
    //string ptrVal; //初始化字符指针常量字符串的名称
    Var *ptr;
    
    void init();
    //通过basetype 设置basetype， basesize 和size
    void set_from_basetype(BaseType type);
public:
    int reg_id;

    Var();
    //tp 和 name 生成 Var；
    Var(Tp* tp, string name, bool is_temp = false);
    Var(Var::BaseType type, string structname = "", bool is_temp = false);
    Var(Var::BaseType type, bool is_temp = false);
    //生成字面量
    Var(int int_var);
    Var(char char_var);
    Var(bool bool_var);
    Var(string str_var);
    //复制一个var
    Var(Var *var, bool is_temp = false);

    ~Var(); 

    string get_name();
    void set_name(string name);
    void set_s_offset(int offset);
    int get_s_offset();
    void set_f_offset(int offset);
    int get_f_offset();
    void set_pointer_deep(int deep);
    int get_pointer_deep();
    int get_basesize();

    void set_left(bool left);
    string to_string();
    int get_size();
    int get_offset_lit(vector<int> &array_index);
    int get_arr_offset(int index_cnt);
    Var* get_step();
    BaseType get_basetype();
    Struct *get_struct();
    int get_val();

    void array_sizes_pop_back();
    void array_sizes_pop_front();
    void array_sizes_clear();

    bool is_base();
    bool is_half();
    bool is_pointer();
    bool is_ref();
    bool is_array();
    bool is_left();
    bool is_string();
    bool has_init();
    bool is_literal();

    static bool check_type(Var* lvar, Var* rvar);
};

class Scope
{
    int scope_id;
    Scope *parent;
    int size;
    int arg_size;
    int f_offset;

    //hash函数
	struct string_hash{
		size_t operator()(const string& str) const{
			return __stl_hash_string(str.c_str());
		}
	};

    hash_map<string, Var*, string_hash> var_tab; //变量表,每个元素是同名变量的链表
	vector<string> var_list;
    hash_map<int, Scope*> scope_tab;
    
public:
    Scope(int id, bool is_func_scope = false);
    ~Scope();
    void add_var(Var* var);
    Var* get_var(string name);
    void add_arg(Var* var);
    Var* get_arg(string name);
    void add_scope(Scope *scope);
    int get_size();
    Scope *get_parent();
    void set_parent(Scope* parent);
    void set_f_offset(int offset);
    int get_f_offset();

    void print();
};

class Func
{
    Var* ret;
    string name;
    int func_id;
    Scope* scope;
    vector<Var*> para_vars;

    vector<int> scope_esp;
    int max_depth;
    int cur_esp;

    bool relocated;

    InterCode intercode;



public:
    InterInst* lfb;
    InterInst* lfe;

    Func(Var*ret, string name, int f_id, Scope *scope, vector<Var*>& para_vars);
    ~Func();

    string get_name();
    void set_name(string name);
    void add_para_var(Var *para);

    void enter_scope();
    void leave_scope();
    void add_inst(InterInst* inst);
    Var* get_ret();
    Scope* get_scope();
    vector<Var*> &get_para_vars();
    bool is_relocated();

    void print_scopes();
    void print_ir();

    void gen_asm(FILE *file);
    
};
