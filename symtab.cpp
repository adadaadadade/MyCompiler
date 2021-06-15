#include "symtab.h"
#include "symbol.h"

#define SEMERROR(code,name) Error::semError(code,name)

Symtab::Symtab(/* args */)
{

}


Symtab::~Symtab()
{

}



void Symtab::add_typedef(Var* v)
{
    if(typedef_tab.find(v->get_name()) == typedef_tab.end())
	{ //没有该名字的变量
		typedef_tab[v->get_name()] = v;
	}
	else
	{
		printf("ERROR\n");
	}
}


Var* Symtab::get_typedef(string name)
{
	if(typedef_tab.find(name) == typedef_tab.end())
		return NULL;
	else
		return typedef_tab[name];
}

