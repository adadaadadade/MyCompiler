#pragma once

#include "common.h"
#include <ext/hash_map>
using namespace __gnu_cxx;

class Symtab
{
private:
    //声明顺序记录
	vector<string>var_list;//记录变量的添加顺序
	vector<string>func_list;//记录函数的添加顺序
	
	//hash函数
	struct string_hash{
		size_t operator()(const string& str) const{
			return __stl_hash_string(str.c_str());
		}
	};
    hash_map<string, Var*, string_hash> typedef_tab;

    //辅助分析数据记录
	Func * cur_func;//当前分析的函数
	int scope_id;//作用域唯一编号
	vector<int>scope_path;//动态记录作用域的路径，全局为0,0 1 2-第一个函数的第一个局部块

public:
    Symtab(/* args */);
    ~Symtab();

    void add_typedef(Var* v);
    Var* get_typedef(string name);
};
