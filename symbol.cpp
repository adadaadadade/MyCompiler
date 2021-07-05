#include "symbol.h"
#include "error.h"
#include "symtab.h"

void Var::init()
{
    tp = NULL;
    base_type = INT;
    scope = NULL;
    name = "";
    base_size = -1;
    size = -1;
    array_dim = 0;
    s_offset = 0;
    stru = NULL;
    is_struct = false;
    is_literal = false;
    is_temp = false;
    is_offset_var = false;
    ptr = NULL;
}

void Var::set_from_basetype(BaseType type)
{
    switch (type)
    {
    case INT:
        this->base_size = int_size;
        this->base_type = INT;
        break;
    case CHAR:
        this->base_size = char_size;
        this->base_type = CHAR;
        break;
    case BOOL:
        this->base_size = bool_size;
        this->base_type = BOOL;
        break;
    case POINTER:
        this->base_size = pointer_size;
        this->base_type = POINTER;
        break;
    case STRING:
        this->base_size = string_size;
        this->base_type = STRING;
        break;
    case STRUCT:
        this->base_size = Symtab::get_symtab()->get_struct(tp->sid->name)->get_size();
        this->base_type = STRUCT;
        break;
    //应该不会有ARRAY和POINTER
    default:
        break;
    }
    size = base_size;
}

Var::Var()
{
}

Var::Var(Tp *tp, string name, bool is_temp) : name(name), tp(tp), is_temp(is_temp) //tp 和 name 生成 Var
{
    switch (tp->type)
    {
    case Tp::INT:
        this->base_size = int_size;
        this->base_type = INT;
        break;
    case Tp::CHAR:
        this->base_size = char_size;
        this->base_type = CHAR;
        break;
    case Tp::BOOL:
        this->base_size = bool_size;
        this->base_type = BOOL;
        break;
    case Tp::STRING:
        this->base_size = string_size;
        this->base_type = STRING;
        break;
    case Tp::STRUCT:
        this->base_type = STRUCT;
        this->stru = Symtab::get_symtab()->get_struct(tp->sid->name);
        this->base_size = this->stru->get_size();
        this->is_struct = true;
        break;
    //应该不会有ARRAY和POINTER
    default:
        break;
    }
    if (tp->tp_tail == NULL)
    {
        this->size = base_size;
    }
    //有后缀
    else
    {
        Tp *i = tp;
        while (i->tp_tail != NULL)
        {
            i = i->tp_tail;
            if (i->type == Tp::POINTER)
            {
                a_p_tail.push_back(-1);
            }
            else if (i->type == Tp::ARRAY)
            {
                a_p_tail.push_back(i->size);
            }
            else
            {
                Error::fatal();
            }
        }

        if (this->is_pointer())
        {
            base_type = POINTER;
            base_size = pointer_size;
            size = pointer_size;
        }
        else if (this->is_array())
        {
            array_dim = 0;
            int tp_tail_size = a_p_tail.size();
            int s = 0;
            int cnt = 1;
            for (int i = tp_tail_size - 1; i >= 0; i--)
            {
                if (a_p_tail[i] == -1)
                {
                    base_type = POINTER;
                    base_size = pointer_size;
                    break;
                }
                else if (a_p_tail[i] == 0)
                {
                    Error::sem_error(Error::ARRAY_LEN_INVALID);
                }
                else
                {
                    cnt *= a_p_tail[i];
                    array_dim++;
                }
            }
            this->size = cnt * base_size;
        }
    }
}

Var::Var(Var::BaseType type, string structname, bool is_temp) : base_type(type), is_temp(is_temp)
{
    init();
    set_from_basetype(type);
    if(is_temp)
        name = ".L" + std::to_string(Symtab::get_symtab()->new_label_id());
    else
        name = "";
}

Var::Var(Var::BaseType type, bool is_temp) : base_type(type), is_temp(is_temp)
{
    init();
    set_from_basetype(type);
    if(is_temp)
        name = ".L" + std::to_string(Symtab::get_symtab()->new_label_id());
    else
        name = "";
}

Var::Var(int int_var) : int_var(int_var)
{
    init();
    is_literal = true;
    set_from_basetype(INT);
}

Var::Var(char char_var) : chr_var(char_var)
{
    init();
    is_literal = true;
    set_from_basetype(CHAR);
}

Var::Var(bool bool_var) : bool_var(bool_var)
{
    init();
    is_literal = true;
    set_from_basetype(BOOL);
}

Var::Var(string str_var) : str_var(str_var)
{
    init();
    is_literal = true;
    set_from_basetype(STRING);
}

Var::Var(Var *var, bool is_temp)
{
    init();
    tp = var->tp;
    base_type = var->base_type;
    scope = var->scope;
    
    base_size = var->base_size;
    size = var->size;
    array_dim = var->array_dim;
    //s_offset = 0;
    stru = var->stru;
    is_struct = var->is_struct;
    //is_literal = var->is_literal;
    //is_temp = var->is_temp;
    is_offset_var = var->is_offset_var;
    ptr = NULL;
    int a_p_tail_size = var->a_p_tail.size();
    for (int i = 0; i < a_p_tail_size; i ++)
    {
        a_p_tail[i] = var->a_p_tail[i];
    }
    
    if(is_temp)
        name = ".L" + std::to_string(Symtab::get_symtab()->new_label_id());
    else
        name = "";
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

void Var::set_s_offset(int offset)
{
    this->s_offset = offset;
}

int Var::get_s_offset()
{
    return s_offset;
}

string Var::to_string()
{
    if (is_literal)
    {
        if (base_type == INT)
        {
            return std::to_string(int_var);
        }
        else if (base_type == CHAR)
        {
            return std::to_string(chr_var);
        }
        else if (base_type == STRING)
        {
            return str_var;
        }
        else if (base_type == BOOL)
        {
            return std::to_string(bool_var);
        }
    }
    return name;
}

int Var::get_size()
{
    /*
    if (tp != NULL)
    {
        if (is_pointer())
        {
            size = Var::pointer_size;
        }
        //////////////////////////未加数组头大小
        else if (is_array())
        {
            size = 0;
            int cnt = 1;
            int tp_tail_size = tp_tail.size();
            for (int i = tp_tail_size - 1; i >= tp_tail_size - array_dim; i --)
            {
                cnt *= tp_tail[i];
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
    */
    return size;
}

//得到一个下标的offset index_cnt为第几个下标，从0开始
int Var::get_arr_offset(int index_cnt)
{
    if (!is_array())
    {
        Error::sem_error(Error::ARR_TYPE_ERR);
    }
    if (index_cnt >= array_dim || index_cnt < 0)
    {
        Error::sem_error(Error::ARRAY_LEN_INVALID);
    }
    int offset = 0;
    int cnt = 1;
    int tp_tail_size = a_p_tail.size();
    for (int i = tp_tail_size - 1; i >= index_cnt; i--)
    {
        if (i != tp_tail_size - 1)
        {
            cnt *= a_p_tail[i + 1];
        }
    }
    offset = cnt * base_size;
    return offset;
}

Var* Var::get_basevar()
{
    return NULL;
}

Var::BaseType Var::get_basetype()
{
    return base_type;
}

Struct* Var::get_struct()
{
    return stru;
}

void Var::sub_pointer()
{
    if(is_pointer())
    {
        a_p_tail.pop_back();
    }
    else
        Error::fatal();
}

bool Var::is_pointer()
{
    if ((a_p_tail.size() > 0) && (a_p_tail.back() == -1))
        return true;
    else
        return false;
}

bool Var::is_array()
{
    if ((a_p_tail.size() > 0) && (a_p_tail.back() >= 0))
        return true;
    else
        return false;
}

bool Var::is_left()
{
    return left;
}

bool Var::check_type(Var* lvar, Var *rvar)
{
    return true;
}

int Var::get_offset_lit(vector<int> &array_index)
{
    if (!is_array())
    {
        Error::sem_error(Error::ARR_TYPE_ERR);
    }
    if (array_index.size() != array_dim)
    {
        Error::sem_error(Error::ARRAY_LEN_INVALID);
    }
    int offset = 0;
    int cnt = 0;
    int size = 1;
    int tp_tail_size = a_p_tail.size();
    for (int i = tp_tail_size - 1; i >= tp_tail_size - array_dim; i--)
    {
        if (i != tp_tail_size - 1)
        {
            size *= a_p_tail[i + 1];
        }
        cnt += array_index[i] * size;
    }
    offset = cnt * base_size;
    return offset;
}

Scope::Scope(int id) : scope_id(id)
{
    parent = NULL;
    size = 0;
}

Scope::~Scope()
{
}

void Scope::add_var(Var *var)
{
    if (var_tab.find(var->get_name()) == var_tab.end())
    { //没有该名字的变量
        var->set_s_offset(size);
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

void Scope::add_scope(Scope *scope)
{
    this->scope_tab[scope->scope_id] = scope;
    scope->set_parent(this);
    return;
}

int Scope::get_size()
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
    //如果有size则直接返回
    return size;
}

Scope *Scope::get_parent()
{
    return parent;
}

void Scope::set_parent(Scope* parent)
{
    this->parent = parent; 
}

void Scope::print()
{
    printf("Scope %d , size : %d, list: \n", scope_id, get_size());
    hash_map<string, Var *, string_hash>::iterator var_iter;
    for (var_iter = var_tab.begin(); var_iter != var_tab.end(); var_iter++)
    {
        printf("%s\ts_offset : %d\n", var_iter->second->to_string().c_str(), var_iter->second->get_s_offset());
    }
    hash_map<int, Scope *>::iterator scope_iter;
    for (scope_iter = scope_tab.begin(); scope_iter != scope_tab.end(); scope_iter++)
    {
        scope_iter->second->print();
    }
    printf("Scope %d end\n", scope_id);
}

Struct::Struct(string name, vector<Tp *> &tp_list, vector<string> &fname_list) : name(name)
{
    int f_size = tp_list.size();
    int offset = 0;
    for (int i = 0; i < f_size; i++)
    {
        Var *var = new Var(tp_list[i], fname_list[i]);
        this->var_tab[var->get_name()] = var;
        this->offset_tab[fname_list[i]] = offset;
        offset += var->get_size();
    }
    size = offset;
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

Var *Struct::get_var(string fname)
{
    if (offset_tab.find(fname) != offset_tab.end())
    {
        return var_tab[fname];
    }
    else
        return NULL;
}

int Struct::get_size()
{
    return size;
}

int Struct::get_offset(string fname)
{
    if (offset_tab.find(fname) != offset_tab.end())
    {
        return offset_tab[fname];
    }
    else
        return -1;
}

void Struct::print()
{
    printf("Sturct name : %s, size = %d\n", name.c_str(), size);
    hash_map<string, Var*, string_hash>::iterator v_iter;
    for (v_iter = var_tab.begin(); v_iter != var_tab.end(); v_iter ++)
    {
        printf("\t%s", v_iter->second->to_string().c_str());
        printf("\t%d\n", offset_tab[v_iter->first]);
    }
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

Tp *Typedef::get_tp()
{
    return tp;
}

void Typedef::print()
{
    printf("Typedef name : %s\n", name.c_str());
    tp->print();
}

Func::Func(Var *ret, string name, int f_id, Scope *scope, vector<Var *> &para_vars) : name(name), ret(ret), func_id(f_id), scope(scope), para_vars(para_vars)
{
    this->lfb = new InterInst(".LFB" + to_string(f_id));
    this->lfe = new InterInst(".LFE" + to_string(f_id));
}

Func::~Func()
{
}

string Func::get_name()
{
    return name;
}

void Func::set_name(string name)
{
    this->name = name;
}

void Func::add_para_var(Var *para)
{
    para_vars.push_back(para);
}

void Func::enter_scope()
{
    scope_esp.push_back(0);
}

void Func::leave_scope()
{
    max_depth = (cur_esp > max_depth) ? cur_esp : max_depth;
    cur_esp -= scope_esp.back();
    scope_esp.pop_back();
}

void Func::add_inst(InterInst *inst)
{
    intercode.add_inst(inst);
}

Var* Func::get_ret()
{
    return ret;
}

void Func::print_scopes()
{
    scope->print();
}

void Func::print_ir()
{
    intercode.print();
}
