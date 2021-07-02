#include "symtab.h"
#include "symbol.h"
#include "error.h"

#define SEMERROR(code,name) Error::semError(code,name)

Symtab* Symtab::symtab;

int Symtab::new_func_id()
{
	func_id ++;
	return func_id;
}

int Symtab::new_scope_id()
{
	scope_id ++;
	return scope_id;
}

int Symtab::new_label_id()
{
	label_id ++;
	return label_id;
}


Symtab::Symtab(/* args */)
{
	cur_func = NULL;//当前分析的函数
	scope_id = 0;//作用域唯一编号
	label_id = 0;
}


Symtab::~Symtab()
{

}



void Symtab::add_typedef(Typedef* v)
{
    if(typedef_tab.find(v->get_name()) == typedef_tab.end())
	{ //没有该名字的变量
		typedef_tab[v->get_name()] = v;
	}
	else
	{
		Error::sem_error(Error::VAR_RE_DEF, v->get_name());
	}
}


Typedef* Symtab::get_typedef(string name)
{
	if(typedef_tab.find(name) == typedef_tab.end())
		return NULL;
	else
		return typedef_tab[name];
}

void Symtab::add_struct(Struct* v)
{
	if(struct_tab.find(v->get_name()) == struct_tab.end())
	{ //没有该名字的变量
		struct_tab[v->get_name()] = v;
	}
	else
	{
		Error::sem_error(Error::STRUCT_RE_DEF, v->get_name());
	}
}


Struct* Symtab::get_struct(string name)
{
	if(struct_tab.find(name) == struct_tab.end())
		return NULL;
	else
		return struct_tab[name];
}

void Symtab::set_symtab(Symtab* st)
{
	Symtab::symtab = st;
}

Symtab *Symtab::get_symtab()
{
	return Symtab::symtab;
}