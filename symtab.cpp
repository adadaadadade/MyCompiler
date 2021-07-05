#include "symtab.h"
#include "symbol.h"
#include "error.h"

#define SEMERROR(code, name) Error::semError(code, name)

Symtab *Symtab::symtab;

int Symtab::new_func_id()
{
	func_id++;
	return func_id;
}

int Symtab::new_scope_id()
{
	scope_id++;
	return scope_id;
}

int Symtab::new_label_id()
{
	label_id++;
	return label_id;
}

Symtab::Symtab(/* args */)
{
	cur_func = NULL; //当前分析的函数
	func_id = -1;	 //要+1 故初始值为-1， 从0开始
	scope_id = 0;	 //作用域唯一编号 默认全局为 0
	label_id = -1;	 //要+1 故初始值为-1， 从0开始
}

Symtab::~Symtab()
{
}

void Symtab::add_typedef(Typedef *v)
{
	if (typedef_tab.find(v->get_name()) == typedef_tab.end())
	{ //没有该名字的变量
		typedef_tab[v->get_name()] = v;
	}
	else
	{
		Error::sem_error(Error::VAR_RE_DEF, v->get_name());
	}
}

Typedef *Symtab::get_typedef(string name)
{
	if (typedef_tab.find(name) == typedef_tab.end())
		return NULL;
	else
		return typedef_tab[name];
}

void Symtab::add_struct(Struct *v)
{
	if (struct_tab.find(v->get_name()) == struct_tab.end())
	{ //没有该名字的变量
		struct_tab[v->get_name()] = v;
	}
	else
	{
		Error::sem_error(Error::STRUCT_RE_DEF, v->get_name());
	}
}

Struct *Symtab::get_struct(string name)
{
	if (struct_tab.find(name) == struct_tab.end())
		return NULL;
	else
		return struct_tab[name];
}

void Symtab::enter_func(Func *func)
{
	// 无此类型funcdef
	if (func_tab.find(func->get_name()) == func_tab.end())
	{
		this->func_tab[func->get_name()] = func;
		this->func_list.push_back(func->get_name());
		cur_func = func;
	}
	else
	{
		Error::sem_error(Error::FUN_RE_DEF);
	}
}

Func *Symtab::get_func(string name)
{
	if (func_tab.find(name) != func_tab.end())
	{
		return func_tab[name];
	}
	else
	{
		Error::sem_error(Error::FUN_UN_DEC);
		return NULL;
	}
}

void Symtab::leave_func()
{
	cur_func = NULL;
}

void Symtab::enter_scope(Scope *scope)
{
	scope_path.push_back(scope_id);
	cur_scope = scope;
	return;
}

void Symtab::leave_scope()
{
	scope_path.pop_back();
	cur_scope = cur_scope->get_parent();
	return;
}

void Symtab::add_var(Var *var)
{
	cur_scope->add_var(var);
}

Var *Symtab::get_var(string name)
{
	return cur_scope->get_var(name);
}

void Symtab::add_inst(InterInst *inst)
{
	if (cur_func)
		cur_func->add_inst(inst);
	else
		Error::fatal();
}

void Symtab::set_symtab(Symtab *st)
{
	Symtab::symtab = st;
}

Symtab *Symtab::get_symtab()
{
	return Symtab::symtab;
}

void Symtab::print()
{
	hash_map<string, Typedef *, string_hash>::iterator t_iter;
	for (t_iter = typedef_tab.begin(); t_iter != typedef_tab.end(); t_iter++)
	{
		t_iter->second->print();
	}
	hash_map<string, Struct *, string_hash>::iterator s_iter;
	for (s_iter = struct_tab.begin(); s_iter != struct_tab.end(); s_iter++)
	{
		s_iter->second->print();
	}
	int func_size = func_list.size();
	for (int i = 0; i < func_size; i++)
	{
		func_tab[func_list[i]]->print_scopes();
	}
}

void Symtab::print_ir()
{
	int func_size = func_list.size();
	for (int i = 0; i < func_size; i++)
	{
		func_tab[func_list[i]]->print_ir();
	}
}

/*
void Symtab::gen_asmdata(FILE *file)
{
	//生成常量字符串,.rodata段
	fprintf(file, ".section .rodata\n");
	hash_map<string, Var *, string_hash>::iterator strIt, strEnd = strTab.end();
	for (strIt = strTab.begin(); strIt != strEnd; ++strIt)
	{
		Var *str = strIt->second;									  //常量字符串变量
		fprintf(file, "%s:\n", str->getName().c_str());				  //var:
		fprintf(file, "\t.ascii \"%s\"\n", str->getRawStr().c_str()); //.ascii "abc\000"
	}
	//生成数据段和bss段
	fprintf(file, ".data\n");
	vector<Var *> glbVars = getGlbVars(); //获取所有全局变量
	for (unsigned int i = 0; i < glbVars.size(); i++)
	{
		Var *var = glbVars[i];
		fprintf(file, "\t.global %s\n", var->getName().c_str()); //.global var
		if (!var->unInit())
		{													//变量初始化了,放在数据段
			fprintf(file, "%s:\n", var->getName().c_str()); //var:
			if (var->isBase())
			{ //基本类型初始化 100 'a'
				const char *t = var->isChar() ? ".byte" : ".word";
				fprintf(file, "\t%s %d\n", t, var->getVal()); //.byte 65  .word 100
			}
			else
			{															 //字符指针初始化
				fprintf(file, "\t.word %s\n", var->getPtrVal().c_str()); //.word .L0
			}
		}
		else
		{																			  //放在bss段
			fprintf(file, "\t.comm %s,%d\n", var->getName().c_str(), var->getSize()); //.comm var,4
		}
	}
}
*/

void Symtab::gen_asm(char *filename)
{
	FILE *file = fopen(filename, "w"); //创建输出文件
	//生成数据段
	gen_asmdata(file);
	//生成代码段

	fprintf(file, ".text\n");
	for (int i = 0; i < func_list.size(); i++)
	{
		//printf("-------------生成函数<%s>--------------\n",funTab[funList[i]]->getName().c_str());
		func_tab[func_list[i]]->gen_asm(file);
	}
	fclose(file);
}