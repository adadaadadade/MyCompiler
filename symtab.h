#pragma once

#include "common.h"
#include <ext/hash_map>
using namespace __gnu_cxx;

class Symtab
{
	static Symtab *symtab;
	//声明顺序记录
	vector<string> var_list;  //记录变量的添加顺序
	vector<string> func_list; //记录函数的添加顺序

	//hash函数
	struct string_hash
	{
		size_t operator()(const string &str) const
		{
			return __stl_hash_string(str.c_str());
		}
	};

	//hash_map<string, vector< Var* > *, string_hash> var_tab; //变量表,每个元素是同名变量的链表
	hash_map<int, Scope *> scope_tab;
	hash_map<string, Func *, string_hash> func_tab;

	hash_map<string, Typedef *, string_hash> typedef_tab;
	hash_map<string, Struct *, string_hash> struct_tab;

	int func_id;
	int scope_id; //作用域唯一编号
	int label_id;
	vector<int> scope_path; //动态记录作用域的路径，全局为0,0 1 2-第一个函数的第一个局部块

public:
	Func *cur_func; //当前分析的函数
	Scope *cur_scope;
	Symtab(/* args */);
	~Symtab();
	/*
	void add_var(Var* var);
	void get_var(string name);
	*/

	int new_func_id();
	int new_scope_id();
	int new_label_id();

	void add_typedef(Typedef *v);
	Typedef *get_typedef(string name);
	void add_struct(Struct *v);
	Struct *get_struct(string name);
	void add_var(Var *var);
	Var *get_var(string name);

	void enter_func(Func *func);
	Func* get_func(string name);
	void leave_func();

	void enter_scope(Scope *scope);
	void leave_scope();
	void add_inst(InterInst *inst);

	static void set_symtab(Symtab *st);
	static Symtab *get_symtab();

	void print();
	void print_ir();

	void gen_asmdata(FILE * file);
	void gen_asm(char* filename);
};
