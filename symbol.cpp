#include "symbol.h"
#include "error.h"
#include "symtab.h"
#include "selector.h"
#include "iloc.h"
#include "platform.h"

void Var::init()
{
    tp = NULL;
    base_type = INT;
    scope = NULL;
    name = "";
    base_size = -1;
    size = -1;
    array_dim = 0;
    pointer_deep = 0;
    s_offset = 0;
    f_offset = 0;
    stru = NULL;
    is_struct = false;
    literal = false;
    is_temp = false;
    is_offset_var = false;
    ptr = NULL;
    reg_id = -1;
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

Var::Var(Tp *tp, string name, bool is_temp) //tp 和 name 生成 Var
{
    init();
    this->name = name;
    this->tp = tp;
    this->is_temp = is_temp;

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
                pointer_deep ++;
            }
            else if (i->type == Tp::ARRAY)
            {
                array_sizes.push_back(i->size);
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
            int tp_tail_size = array_sizes.size();
            int s = 0;
            int cnt = 1;
            for (int i = tp_tail_size - 1; i >= 0; i--)
            {
                if (array_sizes[i] == -1)
                {
                    base_type = POINTER;
                    base_size = pointer_size;
                    break;
                }
                else if (array_sizes[i] == 0)
                {
                    Error::sem_error(Error::ARRAY_LEN_INVALID);
                }
                else
                {
                    cnt *= array_sizes[i];
                    array_dim++;
                }
            }
            this->size = cnt * base_size;
        }
    }
}

Var::Var(Var::BaseType type, string structname, bool is_temp)
{
    init();
    this->base_type = type;
    this->is_temp = is_temp;
    set_from_basetype(type);
    if (is_temp)
        name = ".L" + std::to_string(Symtab::get_symtab()->new_label_id());
    else
        name = "";
}

Var::Var(Var::BaseType type, bool is_temp)
{
    init();
    this->base_type = type;
    this->is_temp = is_temp;
    set_from_basetype(type);
    if (is_temp)
        name = ".L" + std::to_string(Symtab::get_symtab()->new_label_id());
    else
        name = "";
}

Var::Var(int int_var)
{
    //init();
    this->int_var = int_var;
    literal = true;
    set_from_basetype(INT);
}

Var::Var(char char_var)
{
    init();
    this->chr_var = char_var;
    literal = true;
    set_from_basetype(CHAR);
}

Var::Var(bool bool_var)
{
    init();
    this->bool_var = bool_var;
    literal = true;
    set_from_basetype(BOOL);
}

Var::Var(string str_var)
{
    init();
    this->str_var = str_var;
    literal = true;
    set_from_basetype(STRING);
}

Var::Var(Var *var, bool is_temp)
{
    init();
    this->is_temp = is_temp;
    tp = var->tp;
    base_type = var->base_type;
    scope = var->scope;

    base_size = var->base_size;
    size = var->size;
    array_dim = var->array_dim;
    pointer_deep = var->pointer_deep;
    //s_offset = 0;
    stru = var->stru;
    is_struct = var->is_struct;
    //literal = var->literal;
    //is_temp = var->is_temp;
    is_offset_var = var->is_offset_var;
    ptr = NULL;
    array_sizes.clear();
    int length = var->array_sizes.size();
    for (int i = 0; i < length; i++)
    {
        array_sizes.push_back(var->array_sizes[i]);
    }

    if (is_temp)
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

void Var::set_f_offset(int offset)
{
    this->f_offset = offset;
}

int Var::get_f_offset()
{
    //函数内变量 偏移为负
    return -f_offset;
}

void Var::set_pointer_deep(int deep)
{
    pointer_deep = deep;
}

int Var::get_pointer_deep()
{
    return pointer_deep;
}

int Var::get_basesize()
{
    return base_size;
}

string Var::to_string()
{
    if (literal)
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
    int tp_tail_size = array_sizes.size();
    for (int i = tp_tail_size - 1; i >= index_cnt; i--)
    {
        if (i != tp_tail_size - 1)
        {
            cnt *= array_sizes[i + 1];
        }
    }
    offset = cnt * base_size;
    return -offset;
}

Var *Var::get_step()
{
    if(is_base())
        return new Var(1);
}

Var::BaseType Var::get_basetype()
{
    return base_type;
}

Struct *Var::get_struct()
{
    return stru;
}

int Var::get_val()
{
    switch (base_type)
    {
    case INT:
        return int_var;
        break;
    case CHAR:
        return chr_var;
        break;
    case BOOL:
        return bool_var;
        break;
    default:
        break;
    }
}

void Var::array_sizes_pop_back()
{
    int back = array_sizes.back();
    size = size / back;
    array_sizes.pop_back();
}

void Var::array_sizes_pop_front()
{
    array_sizes.erase(array_sizes.begin());
}

void Var::array_sizes_clear()
{
    array_sizes.clear();
}

bool Var::is_base()
{
    return (array_sizes.size() == 0) && (base_type != STRING);
}

bool Var::is_half()
{
    return is_base() && (base_size == 2);
}

bool Var::is_pointer()
{
    /*
    if ((array_sizes.size() > 0) && (array_sizes.back() == -1))
        return true;
    else
        return false;
    */
    return (pointer_deep != 0);
}

bool Var::is_ref()
{
    if(is_array())
        return true;
    if(base_type == STRUCT && !is_pointer())
        return true;
    return false;
}

bool Var::is_array()
{
    if ((array_sizes.size() > 0) && (array_sizes.back() >= 0))
        return true;
    else
        return false;
}

bool Var::is_left()
{
    return left;
}

bool Var::is_string()
{
    return (is_pointer() && (get_pointer_deep() == 1) && (base_type == CHAR)) || (!is_pointer() && base_type == STRING);
}

bool Var::has_init()
{
    return true;
}

bool Var::is_literal()
{
    return literal;
}

bool Var::check_type(Var *lvar, Var *rvar)
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
    int tp_tail_size = array_sizes.size();
    for (int i = tp_tail_size - 1; i >= tp_tail_size - array_dim; i--)
    {
        if (i != tp_tail_size - 1)
        {
            size *= array_sizes[i + 1];
        }
        cnt += array_index[i] * size;
    }
    offset = cnt * base_size;
    return -offset;
}

Scope::Scope(int id, bool is_func_scope) : scope_id(id)
{
    parent = NULL;
    size = 0;
    arg_size = 0 - 4;
    f_offset = 0;
    if(is_func_scope)
    {
        size = Arm32Plat::stack_base + 4;
    }
}

Scope::~Scope()
{
}

void Scope::add_var(Var *var)
{
    if (var_tab.find(var->get_name()) == var_tab.end())
    { //没有该名字的变量
        int v_size = var->get_size();
        v_size += (4 - v_size % 4) % 4; //按4的正数倍取大小
        var->set_s_offset(size);
        var->set_f_offset(f_offset + size);
        size += v_size;
        var_tab[var->get_name()] = var;
        var_list.push_back(var->get_name());
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

void Scope::add_arg(Var *var)
{
    if (var_tab.find(var->get_name()) == var_tab.end())
    { //没有该名字的变量
        int v_size = var->get_size();
        v_size += (4 - v_size % 4) % 4; //按4的正数倍取大小
        var->set_s_offset(arg_size);
        var->set_f_offset(arg_size);
        arg_size -= v_size;
        var_tab[var->get_name()] = var;
        var_list.push_back(var->get_name());
    }
    else
    {
        Error::sem_error(Error::VAR_RE_DEF, var->get_name());
    }
}

Var *Scope::get_arg(string name)
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
    scope->set_f_offset(size);
    return;
}

int Scope::get_size()
{
    /*
    size = 0;
    hash_map<string, Var *, string_hash>::iterator var_iter;
    for (var_iter = var_tab.begin(); var_iter != var_tab.end(); var_iter++)
    {
        size += var_iter->second->get_size();
    }
    */
    int max_child_size = 0;
    hash_map<int, Scope *>::iterator scope_iter;
    for (scope_iter = scope_tab.begin(); scope_iter != scope_tab.end(); scope_iter++)
    {
        int this_size = scope_iter->second->get_size();
        max_child_size = (this_size > max_child_size) ? this_size : max_child_size;
    }
    size += max_child_size;
    //如果有size则直接返回
    return size;
}

Scope *Scope::get_parent()
{
    return parent;
}

void Scope::set_parent(Scope *parent)
{
    this->parent = parent;
}

void Scope::set_f_offset(int offset)
{
    f_offset = offset;
}

int Scope::get_f_offset()
{
    return f_offset;
}

void Scope::print()
{
    printf("Scope %d , size : %d, list: \n", scope_id, get_size());
    int var_cnt = var_list.size();
    for(int i = 0; i < var_cnt; i ++)
    {
        printf("%s\tf_offset : %d\n", var_list[i].c_str(), var_tab[var_list[i]]->get_f_offset());
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
        int v_size = var->get_size();
        v_size += (4 - v_size % 4) % 4; //按4的正数倍取大小
        offset += v_size;
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
    hash_map<string, Var *, string_hash>::iterator v_iter;
    for (v_iter = var_tab.begin(); v_iter != var_tab.end(); v_iter++)
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
    relocated = false;

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

Var *Func::get_ret()
{
    return ret;
}

Scope *Func::get_scope()
{
    return scope;
}

vector<Var*> &Func::get_para_vars()
{
    return para_vars;
}

bool Func::is_relocated()
{
    return relocated;
}

void Func::print_scopes()
{
    scope->print();
}

void Func::print_ir()
{
    intercode.print();
}

void Func::gen_asm(FILE *file)
{
    //导出最终的代码,如果优化则是优化后的中间代码，否则就是普通的中间代码
    vector<InterInst *> code;
    code = intercode.get_code();
    const char *pname = name.c_str();
    fprintf(file, "#函数%s代码\n", pname);
    fprintf(file, "\t.global %s\n", pname); //.global fun\n
    fprintf(file, "%s:\n", pname);          //fun:\n
    ILoc il;                                //ILOC代码
    //将最终的中间代码转化为ILOC代码
    Selector sl(code, il); //指令选择器
    sl.select();
    //将优化后的ILOC代码输出为汇编代码
    il.output(file);
}