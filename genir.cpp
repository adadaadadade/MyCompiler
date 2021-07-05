#include "genir.h"
#include "symbol.h"
#include "symtab.h"
#include "error.h"

#define SEMERROR(errorcode, name) Error::sem_error(errorcode, name);
#define NEWLABEL InterInst(".L" + std::to_string(symtab.new_label_id()))

void GenIR::loop_push(InterInst *head, InterInst *tail)
{
    loop_heads.push_back(head);
    loop_tails.push_back(tail);
}

void GenIR::loop_pop()
{
    loop_heads.pop_back();
    loop_tails.pop_back();
}

Var *GenIR::gen_lv(Lv *lv)
{
    Var *var;
    int pointer_cnt = 0;
    Lv* i = lv;
/*     while ((lv->lv_tail != NULL) && (lv->lv_tail->type == Lv::LVPOINTER))
    {
        lv = lv->lv_tail;
        pointer_cnt ++;
    } */
    
    if(lv->type == Lv::LVPOINTER)
    {

    }
    else
    {
        if(var = symtab.get_var(lv->vid->name))
        {
            if(lv->lv_tail == NULL)
            {
                return var;
            }
            else
            {

            }
        }
        else
        {
            SEMERROR(Error::VAR_UN_DEC, lv->vid->name);
        }
        
    }
}

Var *GenIR::gen_expr(Exp *exp)
{
    switch (exp->type)
    {
    case Exp::PAREN:
        return gen_expr(exp->exp1);
        break;
    case Exp::NUMLIT:
        return new Var(exp->numlit);
        break;
    case Exp::STRLIT:
        return new Var(exp->strlit);
        break;
    case Exp::CHRLIT:
        return new Var(exp->chrlit);
        break;
    case Exp::BOOLLIT:
        return new Var(exp->boollit);
        break;
    case Exp::NULLLIT:
        //return new Var()
        break;
    case Exp::VID:
        return gen_vid_expr(exp);
        break;
    case Exp::BINOP:
        return gen_binop_expr(exp);
        break;
    case Exp::UNOP:
        return gen_unop_expr(exp);
        break;
    case Exp::TRI:
        return gen_tri_expr(exp);
        break;
    case Exp::FUNC:
        return gen_func_expr(exp);
        break;
        /*
    case Exp::DOTFID:
        return gen_dotfid_expr(exp);
        break;
    case Exp::TOFID:
        return gen_tofid_expr(exp);
        break;
    case Exp::ARRAY:
        return gen_array_expr(exp);
        break;
        */
    case Exp::ALLOC:
        return gen_alloc_expr(exp);
        break;
    case Exp::ALLOCARRAY:
        return gen_allocarray_expr(exp);
        break;
    default:
        break;
    }
    return NULL;
}

Var *GenIR::gen_vid_expr(Exp *exp)
{
    Var *var;
    if (var = symtab.get_var(exp->vid->name))
    {
        //是普通类型
        if (exp->exp_tail == NULL)
        {
            return var;
        }
        //有可能是  有可能是 数组索引，a.fid a->fid
        else
        {
            Exp *tail = exp->exp_tail;
            if (tail->is_array_index)
            {
                return gen_array_expr(var, tail);
            }
            else if (tail->type == Exp::DOTFID)
            {
                return gen_dotfid_expr(var, tail);
            }
            else if (tail->type == Exp::TOFID)
            {
                return gen_tofid_expr(var, tail);
            }
        }
    }
    //找不到 未定义
    else
    {
        SEMERROR(Error::VAR_UN_DEC, exp->vid->name)
        return NULL;
    }
}

Var *GenIR::gen_dotfid_expr(Var *head, Exp *tail)
{
    Struct *stru;
    if(stru = head->get_struct())
    {
        Var *meta;
        if(meta = stru->get_var(tail->fid->name))
        {
            Var *temp = new Var(meta, true);
            gen_tempdecl(temp);
            
            Var *temp_offset = gen_add(head, new Var(stru->get_offset(meta->get_name())));

            return gen_ptr(temp_offset);
            
        }
        else
        {
            Error::sem_error(Error::STRUCT_META_UN_DEF);
        }
    }
    else
    {
        Error::sem_error(Error::STRUCT_UN_DEF);
    }
}

Var *GenIR::gen_tofid_expr(Var *head, Exp *tail)
{
    Struct *stru;
    if(stru = head->get_struct())
    {
        Var *meta;
        if(meta = stru->get_var(tail->fid->name))
        {
            Var *temp1 = gen_get(head);
            Var *temp2 = new Var(meta, true);
            
        }
        else
        {
            Error::sem_error(Error::STRUCT_META_UN_DEF, tail->fid->name);
        }
    }
    else
    {
        Error::sem_error(Error::STRUCT_UN_DEF, head->get_name());
    }
}

Var *GenIR::gen_array_expr(Var *head, Exp *tail)
{
    int index_cnt = 0;
    Var *offset = new Var(Var::POINTER, true);
    gen_tempdecl(offset);
    Exp *i = tail;
    //symtab.add_inst(new InterInst(IROP_LEA, ));
    gen_assign(offset, head);

    // offset = offset + offset[i] * gen_expr(i)
    gen_assign(offset, gen_add(offset, gen_mul(new Var(head->get_arr_offset(index_cnt)), gen_expr(i))));
    index_cnt ++;

    while ((i->exp_tail != NULL) && (i->exp_tail->is_array_index == true))
    {
        i = i->exp_tail;
        // offset = offset + offset[i] * gen_expr(i)
        gen_assign(offset, gen_add(offset, gen_mul(new Var(head->get_arr_offset(index_cnt)), gen_expr(i))));
        index_cnt ++;
    }
    return offset;
}


Var *GenIR::gen_binop_expr(Exp *exp)
{
    Var *lvar = gen_expr(exp->exp1);
    Var *rvar = gen_expr(exp->exp2);
    switch (exp->tag)
    {
    case LOR:
        return gen_lor(lvar, rvar);
        break;
    case LAND:
        return gen_land(lvar, rvar);
        break;
    case BOR:
        return gen_bor(lvar, rvar);
        break;
    case BXOR:
        return gen_bxor(lvar, rvar);
        break;
    case BAND:
        return gen_band(lvar, rvar);
        break;
    case LTH:
        return gen_lth(lvar, rvar);
        break;
    case LEQ:
        return gen_leq(lvar, rvar);
        break;
    case GTH:
        return gen_gth(lvar, rvar);
        break;
    case GEQ:
        return gen_geq(lvar, rvar);
        break;
    case EQU:
        return gen_equ(lvar, rvar);
        break;
    case NEQ:
        return gen_neq(lvar, rvar);
        break;
    case LMO:
        return gen_lmo(lvar, rvar);
        break;
    case RMO:
        return gen_rmo(lvar, rvar);
        break;
    case ADD:
        return gen_add(lvar, rvar);
        break;
    case SUB:
        return gen_sub(lvar, rvar);
        break;
    case MUL:
        return gen_mul(lvar, rvar);
        break;
    case DIV:
        return gen_div(lvar, rvar);
        break;
    case MOD:
        return gen_mod(lvar, rvar);
        break;
    default:
        break;
    }
}

Var *GenIR::gen_unop_expr(Exp *exp)
{
    Var *var = gen_expr(exp->exp1);
    switch (exp->tag)
    {
    case NOT:
        return gen_not(var);
        break;
    case SUB:
        return gen_minus(var);
        break;
    case BNEG:
        return gen_bneg(var);
        break;
    case INC:
        return gen_linc(var);
        break;
    case DEC:
        return gen_ldec(var);
        break;
    case MUL:
        return gen_ptr(var);
        break;
    default:
        break;
    }
    return NULL;
}

Var *GenIR::gen_tri_expr(Exp *exp)
{
    Var *temp1 = gen_expr(exp->exp2);
    Var *temp2 = gen_expr(exp->exp3);
    Var *ret = new Var(temp1, true);
    gen_tempdecl(ret);
    Var *cond = gen_expr(exp->exp1);
    InterInst *_else, *_exit;
    gen_ifhead(cond, _else);
    gen_assign(ret, temp1);

    gen_elsehead(_else, _exit);
    gen_assign(ret, temp2);
    gen_elsetail(_exit);
    return ret;
}

Var *GenIR::gen_func_expr(Exp *exp)
{
    Func *function;
    if (function = symtab.get_func(exp->vid->name))
    {
        vector<Var *> args;

        Exp *e = exp;
        while (e->exp_tail)
        {
            e = e->exp_tail;
            args.push_back(gen_expr(e));
        }

        // 类型检查

        //
        for (int i = args.size() - 1; i >= 0; i--)
        { //逆向传递实际参数
            gen_para(args[i]);
        }
        Var *ret = new Var(function->get_ret()->get_basetype(), true);
        //中间代码call
        symtab.add_inst(new InterInst(IROP_CALL, function, ret));
        symtab.add_var(ret); //将返回值声明延迟到函数调用之后！！！
        return ret;
    }
    else
    {
        SEMERROR(Error::FUN_UN_DEC, exp->vid->name);
    }
}

Var *GenIR::gen_alloc_expr(Exp *exp)
{
}

Var *GenIR::gen_allocarray_expr(Exp *exp)
{
}

//双目

Var *GenIR::gen_lor(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_LOR, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_land(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_LAND, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_bor(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_BOR, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_bxor(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_BXOR, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_band(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_BAND, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_lth(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_LTH, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_leq(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_LEQ, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_gth(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_GTH, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_geq(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_GEQ, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_equ(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_EQU, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_neq(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_NEQ, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_lmo(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_LMO, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_rmo(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_RMO, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_add(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_ADD, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_sub(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_SUB, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_mul(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_MUL, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_div(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_DIV, tmp, lvar, rvar));
    return tmp;
}

Var *GenIR::gen_mod(Var *lvar, Var *rvar)
{
    Var *tmp = new Var(lvar, true); //基本类型
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_MOD, tmp, lvar, rvar));
    return tmp;
}

//单目
Var *GenIR::gen_not(Var *var)
{
    Var *tmp = new Var(var); //生成整数
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_NOT, tmp, var)); //中间代码tmp=!val
    return tmp;
}

Var *GenIR::gen_minus(Var *var)
{
    Var *tmp = new Var(var, true); //生成整数
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_NEG, tmp, var)); //中间代码tmp=-val
    return tmp;
}

Var *GenIR::gen_bneg(Var *var)
{
    Var *tmp = new Var(var, true); //生成整数
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_BNEG, tmp, var)); //中间代码tmp=-val
    return tmp;
}
//取址语句

Var *GenIR::gen_lea(Var *var)
{
    Var *tmp = new Var(var, true); //生成整数
    gen_tempdecl(tmp);
    symtab.add_inst(new InterInst(IROP_LEA, tmp, var)); //中间代码tmp=&val
    return tmp;
}

//指针
Var *GenIR::gen_ptr(Var *var)
{
/*     Var *tmp = new Var(var);
    tmp->setLeft(true);   //指针运算结果为左值
    tmp->setPointer(var); //设置指针变量
    symtab.addVar(tmp);   //产生表达式需要根据使用者判断，推迟！
    return tmp;
     */
}

Var *GenIR::gen_linc(Var *var)
{
    Var *tmp = gen_assign(var);                                              //拷贝
    symtab.add_inst(new InterInst(IROP_ADD, var, var, var->get_basevar())); //中间代码val++
    return tmp;
}

Var *GenIR::gen_ldec(Var *var)
{
    Var *tmp = gen_assign(var);                                              //拷贝
    symtab.add_inst(new InterInst(IROP_SUB, var, var, var->get_basevar())); //中间代码val++
    return tmp;
}

Var *GenIR::gen_rinc(Var *var)
{
    Var *tmp = gen_assign(var);                                              //拷贝
    symtab.add_inst(new InterInst(IROP_ADD, var, var, var->get_basevar())); //中间代码val++
    return tmp;
}

Var *GenIR::gen_rdec(Var *var)
{
    Var *tmp = gen_assign(var);                                              //拷贝
    symtab.add_inst(new InterInst(IROP_SUB, var, var, var->get_basevar())); //中间代码val++
    return tmp;
}

Var *GenIR::gen_get(Var *var)
{
    Var *temp = new Var(var, true);
    temp->sub_pointer();
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_GET, temp, var));
}

void GenIR::gen_para(Var*arg)//参数传递语句
{
    	/*
		注释部分为删除代码，参数入栈不能通过拷贝，只能push！
	*/
	//if(arg->isRef())arg=genAssign(arg);
	//无条件复制参数！！！传值，不传引用！！！
	//Var*newVar=new Var(symtab.getScopePath(),arg);//创建参数变量
	//symtab.addVar(newVar);//添加无效变量，占领栈帧！！
	InterInst*argInst=new InterInst(IROP_ARG, arg);//push arg!!!
	//argInst->offset=newVar->getOffset();//将变量的地址与arg指令地址共享！！！没有优化地址也能用
	//argInst->path=symtab.getScopePath();//记录路径！！！为了寄存器分配时计算地址
	symtab.add_inst(argInst);  
}

GenIR::GenIR(InterCode &ir, Prog &ast, Symtab &symtab) : ir(ir), ast(ast), symtab(symtab)
{
}

GenIR::~GenIR()
{
}

void GenIR::gen_ir()
{
    for (int i = 0; i < ast.gdefn_list.size(); i++)
    {
        Gdefn *gdefn = ast.gdefn_list[i];
        if (gdefn->type == Gdefn::FUNCDEFN)
        {
            Paralist *pl = gdefn->pl;

            Var *ret = new Var(gdefn->tp, gdefn->vid->name);
            Scope *scope = new Scope(symtab.new_scope_id());
            vector<Var *> *para_vars = new vector<Var *>;
            int pl_tp_list_size = pl->tp_list.size();
            for (int i = 0; i < pl_tp_list_size; i++)
            {
                Var *para = new Var(pl->tp_list[i], pl->vid_list[i]->name);
                para_vars->push_back(para);
                scope->add_var(para);
            }

            Func *func = new Func(ret, gdefn->vid->name, symtab.new_func_id(), scope, *para_vars);
            symtab.enter_func(func);
            symtab.enter_scope(scope);
            func_head(func);
            gen_body(gdefn->body, scope);
            func_tail(func);
            symtab.leave_scope();
            symtab.leave_func();
        }
    }
    return;
}

void GenIR::func_head(Func *func)
{
    symtab.add_inst(new InterInst(IROP_ENTRY));
    symtab.add_inst(func->lfb);
}

void GenIR::func_tail(Func *func)
{
    symtab.add_inst(func->lfe);
    symtab.add_inst(new InterInst(IROP_EXIT));
}

void GenIR::gen_body(Body *body, Scope *scope)
{
    bool scope_changed = false;
    Scope *newscope = scope;
    if(scope == NULL)
    {
        newscope = new Scope(symtab.new_scope_id());
        symtab.cur_scope->add_scope(newscope);
        symtab.enter_scope(newscope);
        scope = newscope;
        scope_changed = true;
    }
    int decl_list_size = body->decl_list.size();
    Decl *decl;
    for (int i = 0; i < decl_list_size; i++)
    {
        decl = body->decl_list[i];
        Var *var = new Var(decl->tp, decl->vid->name);
        symtab.add_var(var);
        symtab.add_inst(new InterInst(IROP_DEC, var));
    }

    int stmt_list_size = body->stmt_list.size();
    Stmt *stmt;
    for (int i = 0; i < stmt_list_size; i++)
    {
        stmt = body->stmt_list[i];
        gen_stmt(stmt, scope);
    }
    if (scope_changed)
        symtab.leave_scope();
    return;
}

void GenIR::gen_stmt(Stmt *stmt, Scope *parent)
{
    switch (stmt->type)
    {
    case Stmt::BODY:
        Scope *scope;
        gen_body(stmt->body);
        break;
    case Stmt::IF:
        gen_if(stmt, parent);
        break;
    case Stmt::WHILE:
        gen_while(stmt, parent);
        break;
    case Stmt::FOR:
        gen_for(stmt, parent);
        break;
    case Stmt::CONTINUE:
        gen_continue(stmt, parent);
        break;
    case Stmt::BREAK:
        gen_break(stmt, parent);
        break;
    case Stmt::RETURN:
        gen_return(stmt, parent);
        break;
    case Stmt::ASSERT:
        gen_assert(stmt, parent);
        break;
    case Stmt::EMPTY:
        gen_empty(stmt, parent);
        break;
    case Stmt::SIMPLE:
        gen_simple(stmt->simple1, parent);
        break;
    default:
        break;
    }
}

void GenIR::gen_if(Stmt *stmt, Scope *parent)
{
    InterInst *_else, *_exit;
    Var *cond = gen_expr(stmt->exp1);
    gen_ifhead(cond, _else);
    gen_stmt(stmt->stmt1, parent);
    gen_elsehead(_else, _exit);
    if (stmt->stmt2 != NULL)
    {
        gen_stmt(stmt->stmt2, parent);
    }
    gen_elsetail(_exit);
    return;
}

void GenIR::gen_while(Stmt *stmt, Scope *parent)
{
    InterInst *_while, *_exit; //标签
    gen_whilehead(_while, _exit);
    Var *cond = gen_expr(stmt->exp1);
    gen_whilecond(cond, _exit);
    gen_stmt(stmt->stmt1, parent);
    gen_whiletail(_while, _exit);
    return;
}

void GenIR::gen_for(Stmt *stmt, Scope *parent)
{
    InterInst *_for, *_exit, *_step, *_stmt; //标签
    gen_simple(stmt->simple1, parent);
    gen_forhead(_for, _exit);
    Var *cond = gen_expr(stmt->exp1);
    gen_forcondbegin(cond, _step, _stmt, _exit);
    gen_simple(stmt->simple2, parent);
    gen_forcondend(_for, _stmt);
    gen_stmt(stmt->stmt1, parent);
    gen_fortail(_step, _exit);
    return;
}

void GenIR::gen_continue(Stmt *stmt, Scope *parent)
{
    InterInst *head = loop_heads.back();
    if (head)
        symtab.add_inst(new InterInst(IROP_JMP, head));
    else
        SEMERROR(Error::CONTINUE_ERR, "");
    return;
}

void GenIR::gen_break(Stmt *stmt, Scope *parent)
{
    InterInst *tail = loop_tails.back();
    if (tail)
        symtab.add_inst(new InterInst(IROP_JMP, tail));
    else
        SEMERROR(Error::BREAK_ERR, "");
    return;
}

void GenIR::gen_return(Stmt *stmt, Scope *parent)
{
    Func *func = symtab.cur_func;
    InterInst *lfe = func->lfe;
    if (stmt->exp1 == NULL)
    {
        symtab.add_inst(new InterInst(IROP_RET, lfe));
    }
    else
    {
        Var *ret = gen_expr(stmt->exp1);
        symtab.add_inst(new InterInst(IROP_RETV, lfe, ret));
    }
    return;
}

void GenIR::gen_tempdecl(Var *var)
{
    symtab.add_inst(new InterInst(IROP_DEC, var)); //添加变量声明指令
    symtab.add_var(var);
}

void GenIR::gen_assert(Stmt *stmt, Scope *parent)
{
    //
    return;
}

void GenIR::gen_empty(Stmt *stmt, Scope *parent)
{
    symtab.add_inst(new InterInst(IROP_NOP));
    return;
}

void GenIR::gen_whilehead(InterInst *&_while, InterInst *&_exit)
{

    // InterInst* _blank=new InterInst();//_blank标签
    // symtab.add_inst(new InterInst(OP_JMP,_blank));//goto _blank

    _while = new NEWLABEL;   //产生while标签
    symtab.add_inst(_while); //添加while标签

    // symtab.add_inst(_blank);//添加_blank标签

    _exit = new InterInst(".L" + to_string(symtab.new_label_id())); //产生exit标签
    loop_push(_while, _exit);                                       //进入while
}

void GenIR::gen_whilecond(Var *cond, InterInst *_exit)
{
    if (cond)
    {
        /*
        if (cond->isVoid())
            cond = Var::getTrue(); //处理空表达式
        else if (cond->isRef())
            cond = genAssign(cond); //while(*p),while(a[0])
        */
        symtab.add_inst(new InterInst(IROP_JF, _exit, cond));
    }
}

void GenIR::gen_whiletail(InterInst *&_while, InterInst *&_exit)
{
    symtab.add_inst(new InterInst(IROP_JMP, _while)); //添加jmp指令
    symtab.add_inst(_exit);                           //添加exit标签
    loop_pop();                                       //离开while
}

void GenIR::gen_forhead(InterInst *&_for, InterInst *&_exit)
{
    _for = new NEWLABEL;  //产生for标签
    _exit = new NEWLABEL; //产生exit标签
    symtab.add_inst(_for);
}

void GenIR::gen_forcondbegin(Var *cond, InterInst *&_step, InterInst *&_stmt, InterInst *_exit)
{
    _stmt = new NEWLABEL; //产生block标签
    _step = new NEWLABEL; //产生循环动作标签
    if (cond)
    {
        /*
        if (cond->isVoid())
            cond = Var::getTrue(); //处理空表达式
        else if (cond->isRef())
            cond = genAssign(cond); //for(*p),for(a[0])
        */
        symtab.add_inst(new InterInst(IROP_JF, _exit, cond));
        symtab.add_inst(new InterInst(IROP_JMP, _stmt)); //执行循环体
    }
    symtab.add_inst(_step);  //添加循环动作标签
    loop_push(_step, _exit); //进入for
}

void GenIR::gen_forcondend(InterInst *_for, InterInst *_stmt)
{
    symtab.add_inst(new InterInst(IROP_JMP, _for)); //继续循环
    symtab.add_inst(_stmt);                         //添加循环体标签
}

void GenIR::gen_fortail(InterInst *&_step, InterInst *&_exit)
{
    symtab.add_inst(new InterInst(IROP_JMP, _step)); //跳转到循环动作
    symtab.add_inst(_exit);                          //添加_exit标签
    loop_pop();                                      //离开for
}

void GenIR::gen_ifhead(Var *cond, InterInst *&_else)
{
    _else = new NEWLABEL; //产生else标签
    if (cond)
    {
        /*
        if (cond->isRef())
            cond = genAssign(cond); //if(*p),if(a[0])
        */
        symtab.add_inst(new InterInst(IROP_JF, _else, cond));
    }
}

void GenIR::gen_iftail(InterInst *&_else)
{
    symtab.add_inst(_else);
}

void GenIR::gen_elsehead(InterInst *_else, InterInst *&_exit)
{
    _exit = new NEWLABEL; //产生exit标签
    symtab.add_inst(new InterInst(IROP_JMP, _exit));
    symtab.add_inst(_else);
}

void GenIR::gen_elsetail(InterInst *&_exit)
{
    symtab.add_inst(_exit);
}

void GenIR::gen_simple(Simple *simple, Scope *parent)
{
    Var *lvar = gen_lv(simple->lv);
    switch (simple->type)
    {
    case Simple::ASN:
        switch (simple->asnop->type)
        {
        case Asnop::ASN :
            gen_assign(lvar, gen_expr(simple->exp));
            break;
        case Asnop::ASNADD :
            gen_assign(lvar, gen_add(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNSUB :
            gen_assign(lvar, gen_sub(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNMUL :
            gen_assign(lvar, gen_mul(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNDIV :
            gen_assign(lvar, gen_div(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNMOD :
            gen_assign(lvar, gen_mod(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNLMO :
            gen_assign(lvar, gen_lmo(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNRMO :
            gen_assign(lvar, gen_rmo(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNAND :
            gen_assign(lvar, gen_band(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNXOR :
            gen_assign(lvar, gen_bxor(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNOR :
            gen_assign(lvar, gen_bor(lvar, gen_expr(simple->exp)));
            break;
        default:
            break;
        }
        
        break;
    case Simple::ADDADD:
        gen_assign(lvar, gen_add(lvar, lvar->get_basevar()));
        break;
    case Simple::SUBSUB:
        gen_assign(lvar, gen_sub(lvar, lvar->get_basevar()));
        break;
    case Simple::EXP:
        gen_expr(simple->exp);
        break;
    default:
        break;
    }
    return;
}

Var *GenIR::gen_assign(Var *val) //变量拷贝赋值，用于指针左值引用和变量复制
{
    Var *tmp = new Var(val); //拷贝变量信息
    gen_tempdecl(tmp);
    /*
    if (val->isRef())
    {
        //中间代码tmp=*(val->ptr)
        symtab.addInst(new InterInst(OP_GET, tmp, val->getPointer()));
    }
    */
    symtab.add_inst(new InterInst(IROP_ASN, tmp, val)); //中间代码tmp=val
    return tmp;
}

Var *GenIR::gen_assign(Var *lvar, Var *rvar)
{
    	//被赋值对象必须是左值
    /*
	if(!lvar->is_left()){
		SEMERROR(Error::EXPR_NOT_LEFT_VAL);//左值错误
		return rvar;
	}
    */
	//类型检查
	if(!Var::check_type(lvar,rvar)){
		SEMERROR(Error::ASSIGN_TYPE_ERR, rvar->get_name());//赋值类型不匹配
		return rvar;
	}
    /*
	//考虑右值(*p)
	if(rvar->isRef()){
		if(!lvar->isRef()){
			//中间代码lvar=*(rvar->ptr)
			symtab.addInst(new InterInst(OP_GET,lvar,rvar->getPointer()));
			return lvar;
		}
		else{
			//中间代码*(lvar->ptr)=*(rvar->ptr),先处理右值
			rvar=genAssign(rvar);
		}
	}
    //赋值运算
	if(lvar->isRef()){
		//中间代码*(lvar->ptr)=rvar
		symtab.addInst(new InterInst(OP_SET,rvar,lvar->getPointer()));
	}
	else{
		//中间代码lvar=rvar
		
	}
    */
	
    symtab.add_inst(new InterInst(IROP_ASN,lvar,rvar));
	return lvar;
}
