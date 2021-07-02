#include "symbol.h"
#include "error.h"
#include "symtab.h"

Var::Var()
{
}

Var::Var(Tp* tp, string name) : name(name), tp(tp)//tp 和 name 生成 Var
{
    switch (tp->type)
    {
    case Tp::INT:
        this->base_size = int_size;
        this->base_type = INT;
        break;
    case Tp::CHAR :
        this->base_size = char_size;
        this->base_type = CHAR;
        break;
    case Tp::BOOL :
        this->base_size = bool_size;
        this->base_type = BOOL;
        break;
    case Tp::STRING :
        this->base_size = string_size;
        this->base_type = STRING;
        break;
    case Tp::STRUCT :
        this->base_size = Symtab::get_symtab()->get_struct(tp->sid->name)->get_size();
        this->base_type = STRUCT;
        this->is_struct = true;
        break;
    //应该不会有ARRAY和POINTER
    default:
        break;
    }
    if(tp->tp_tail == NULL)
    {
        this->size = base_size;
    }
    //为数组
    else
    {
        if(tp->tp_tail->type == Tp::ARRAY)
        {
            this->is_array = true;
            Tp* i;
            i = tp;
            int s = 0;
            int cnt = 1;
            while (i->tp_tail != NULL)
            {
                i = i->tp_tail;
                s = tp->size;
                
                this->array_deep ++;
                this->array_size.push_back(s);

                cnt *= s;
            }
            this->size = cnt * base_size;
        }
        else if(tp->tp_tail->type == Tp::POINTER)
        {
            this->base_size = pointer_size;
            this->size = base_size;
            this->is_pointer = true;
            Tp* i;
            i = tp;
            while (i->tp_tail != NULL)
            {
                i = i->tp_tail;
                this->pointer_deep ++;
                if(i->type != Tp::POINTER)
                {
                    
                }
            }
        }
        
    }
}

Var::~Var()
{
}

string Var::get_name()
{
    return name;
}

void Var::set_name(string name)
{
    this->name = name;
}

string Var::get_str()
{
    if (is_literal)
    {
        if (tp->type == Tp::INT)
        {
            return to_string(int_var);
        }
        else if (tp->type == Tp::CHAR)
        {
            return to_string(chr_var);
        }
        else if (tp->type == Tp::STRING)
        {
            return str_var;
        }
        else if (tp->type == Tp::BOOL)
        {
            return to_string(bool_var);
        }
    }
    else
    {
        return name;
    }
}

int Var::get_size()
{
    if (tp != NULL)
    {
        if (is_pointer)
        {
            size = Var::pointer_size;
        }
        //////////////////////////未加数组头大小
        else if (is_array)
        {
            size = 0;
            int cnt = 1;
            for (int i = 0; i < array_deep; i++)
            {
                cnt *= array_size[i];
            }
            size = cnt * base_size;
        }
        else
        {
            size = base_size;
        }
    }
    else
    {
        Error::fatal();
    }
    return size;
}

int Var::get_offset(vector<int> &array_index)
{
    if (!is_array)
    {
        Error::sem_error(Error::ARR_TYPE_ERR);
    }
    if (array_index.size() != array_size.size())
    {
        Error::sem_error(Error::ARRAY_LEN_INVALID);
    }
    int offset = 0;
    int cnt = 0;
    int size = 1;
    for (int i = array_deep - 1; i >= 0; i--)
    {
        if (i != array_deep - 1)
        {
            size *= array_size[i + 1];
        }
        cnt += array_index[i] * size;
    }
    offset = cnt * base_size;
    return offset;
}

Scope::Scope(int id) : scope_id(id)
{
}

Scope::~Scope()
{
}

void Scope::add_var(Var *var)
{
    if (var_tab.find(var->get_name()) == var_tab.end())
    { //没有该名字的变量
        var_tab[var->get_name()] = var;
        size += var->get_size();
    }
    else
    {
        Error::sem_error(Error::VAR_RE_DEF, var->get_name());
    }
}

Var *Scope::get_var(string name)
{
    if (var_tab.find(name) != var_tab.end())
    { //有该名字的变量
        return var_tab[name];
    }
    else
    {
        if (parent != NULL)
        {
            return parent->get_var(name);
        }
        else
        {
            return NULL;
        }
    }
}

int Scope::get_size()
{
    if (size == -1)
    {
        size = 0;
        hash_map<string, Var *, string_hash>::iterator var_iter;
        for (var_iter = var_tab.begin(); var_iter != var_tab.end(); var_iter++)
        {
            size += var_iter->second->get_size();
        }
        hash_map<int, Scope *>::iterator scope_iter;
        for (scope_iter = scope_tab.begin(); scope_iter != scope_tab.end(); scope_iter++)
        {
            size += scope_iter->second->get_size();
        }
    }
    //如果有size则直接返回
    return size;
}

Struct::Struct(string name, vector<Tp *> &tp_list, vector<string> &fname_list) : name(name)
{
    int f_size = tp_list.size();
    int offset = 0;
    for (int i = 0; i < f_size; i++)
    {
        Var* var = new Var(tp_list[i], fname_list[i]);
        this->var_list.push_back(var);
        offset += var->get_size();
        this->offset_list[fname_list[i]] = offset;
    }
}

Struct::~Struct()
{
}

string Struct::get_name()
{
    return name;
}

void Struct::set_name(string name)
{
    this->name = name;
}

int Struct::get_size()
{

}

int Struct::get_offset(string fname)
{

}

Typedef::Typedef(Tp *tp, string name) : tp(tp), name(name)
{
}

Typedef::~Typedef()
{
}

string Typedef::get_name()
{
    return name;
}

void Typedef::set_name(string name)
{
    this->name = name;
}

Func::Func(/* args */)
{

}

Func::~Func()
{

}


