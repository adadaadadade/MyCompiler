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
    Lv *i = lv;
    /*     while ((lv->lv_tail != NULL) && (lv->lv_tail->type == Lv::LVPOINTER))
    {
        lv = lv->lv_tail;
        pointer_cnt ++;
    } */

    if (lv->type == Lv::LVPOINTER)
    {
    }
    else
    {
        if (var = symtab.get_var(lv->vid->name))
        {
            if (lv->lv_tail == NULL)
            {
                return var;
            }
            else
            {
                return gen_tail_lv(var, lv->lv_tail);
            }
        }
        else
        {
            SEMERROR(Error::VAR_UN_DEC, lv->vid->name);
        }
    }
}

Var *GenIR::gen_tail_lv(Var *head, Lv *tail)
{
    switch (tail->type)
    {
    case Lv::LVDOTVID:
        return gen_dotfid_lv(head, tail);
        break;
    case Lv::LVTOVID:
        return gen_tofid_lv(head, tail);
        break;
    case Lv::LVARRAY:
        return gen_array_lv(head, tail);
        break;
    default:
        break;
    }
    Error::fatal();
    return NULL;
}

Var *GenIR::gen_dotfid_lv(Var *head, Lv *tail)
{
    Struct *stru;
    if (stru = head->get_struct())
    {
        Var *meta;
        if (meta = stru->get_var(tail->fid->name))
        {
            Var *fvar = new Var(meta, true);
            gen_tempdecl(fvar, false);
            fvar->set_f_offset(head->get_s_offset() + stru->get_offset(meta->get_name()));
            if (tail->lv_tail != NULL)
                return gen_tail_lv(fvar, tail->lv_tail);
            else
                return fvar;
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
    return NULL;
}

Var *GenIR::gen_tofid_lv(Var *head, Lv *tail)
{
}

Var *GenIR::gen_array_lv(Var *head, Lv *tail)
{
    int index_cnt = 0;
    Var *offset = new Var(head, true);
    ////////////////////////////////////////???????????????????????????
    Lv *i = tail;
    offset->array_sizes_pop_back();
    while ((i->lv_tail != NULL) && (i->lv_tail->type = Lv::LVARRAY))
    {
        i = i->lv_tail;
        offset->array_sizes_pop_back();
    }
        
    i = tail;
    //????????????
    
    gen_tempdecl(offset);
    
    //symtab.add_inst(new InterInst(IROP_LEA, ));
    gen_assign(offset, head);

    // offset = offset + offset[i] * gen_expr(i)
    gen_assign(offset, gen_add(offset, gen_mul(new Var(head->get_arr_offset(index_cnt)), gen_expr(i->exp))));
    index_cnt++;

    while ((i->lv_tail != NULL) && (i->lv_tail->type = Lv::LVARRAY))
    {
        i = i->lv_tail;
        // offset = offset + offset[i] * gen_expr(i)
        gen_assign(offset, gen_add(offset, gen_mul(new Var(head->get_arr_offset(index_cnt)), gen_expr(i->exp))));
        index_cnt++;
    }
    offset->set_pointer_deep(offset->get_pointer_deep() + 1);
    //????????????????????????
    if (i->lv_tail != NULL)
        return gen_tail_lv(offset, i->lv_tail);
    else
        return offset;
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
        // 32???
        return new Var(0);
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
        //???????????????
        if (exp->exp_tail == NULL)
        {
            return var;
        }
        //????????????  ???????????? ???????????????a.fid a->fid
        else
        {
            return gen_tail_expr(var, exp->exp_tail);
        }
    }
    //????????? ?????????
    else
    {
        SEMERROR(Error::VAR_UN_DEC, exp->vid->name)
        return NULL;
    }
}

Var *GenIR::gen_tail_expr(Var *head, Exp *tail)
{
    if(tail->type == Exp::DOTFID)
    {
        return gen_dotfid_expr(head, tail);
    }
    else if(tail->type == Exp::TOFID)
    {
        return gen_tofid_expr(head, tail);
    }
    else if(tail->is_array_index)
    {
        return gen_array_expr(head, tail);
    }
    Error::fatal();
    return NULL;
}

Var *GenIR::gen_dotfid_expr(Var *head, Exp *tail)
{
    Struct *stru;
    if (stru = head->get_struct())
    {
        Var *meta;
        if (meta = stru->get_var(tail->fid->name))
        {
            Var *fvar = new Var(meta, true);
            gen_tempdecl(fvar, false);
            fvar->set_f_offset(head->get_s_offset() + stru->get_offset(meta->get_name()));
            if (tail->exp_tail != NULL)
                return gen_tail_expr(fvar, tail->exp_tail);
            else
                return fvar;
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
    return NULL;
}

Var *GenIR::gen_tofid_expr(Var *head, Exp *tail)
{
    Struct *stru;
    if (stru = head->get_struct())
    {
        Var *meta;
        if (meta = stru->get_var(tail->fid->name))
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
    return NULL;
}

//????????????????????????
Var *GenIR::gen_array_expr(Var *head, Exp *tail)
{
    int index_cnt = 0;
    Var *offset = new Var(head, true);
    Var *ret;
    Exp *i = tail;
    offset->array_sizes_pop_back();
    while ((i->exp_tail != NULL) && (i->exp_tail->is_array_index == true))
    {
        i = i->exp_tail;
        offset->array_sizes_pop_back();
    }
    i = tail;
    
    gen_tempdecl(offset);
    
    gen_assign(offset, head);

    // offset = offset + offset[i] * gen_expr(i)
    gen_assign(offset, gen_add(offset, gen_mul(new Var(head->get_arr_offset(index_cnt)), gen_expr(i->exp1))));
    index_cnt++;

    while ((i->exp_tail != NULL) && (i->exp_tail->is_array_index == true))
    {
        i = i->exp_tail;
        // offset = offset + offset[i] * gen_expr(i)
        gen_assign(offset, gen_add(offset, gen_mul(new Var(head->get_arr_offset(index_cnt)), gen_expr(i->exp1))));
        index_cnt++;
    }
    offset->set_pointer_deep(offset->get_pointer_deep() + 1);
    ret = gen_get(offset);
    return ret;
    if (i->exp_tail != NULL)
        return gen_tail_expr(offset, i->exp_tail);
    else
        return ret;
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
            args.push_back(gen_expr(e->exp1));
        }

        // ????????????

        //
        for (int i = args.size() - 1; i >= 0; i--)
        { //????????????????????????
            gen_para(args[i]);
        }
        Var *ret = new Var(function->get_ret()->get_basetype(), true);
        //????????????call
        symtab.add_inst(new InterInst(IROP_CALL, function, ret));
        symtab.add_var(ret); 
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

//??????

Var *GenIR::gen_lor(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_LOR, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_land(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_LAND, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_bor(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_BOR, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_bxor(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_BXOR, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_band(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_BAND, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_lth(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_LTH, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_leq(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_LEQ, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_gth(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_GTH, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_geq(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_GEQ, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_equ(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_EQU, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_neq(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_NEQ, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_lmo(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_LMO, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_rmo(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_RMO, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_add(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_ADD, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_sub(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_SUB, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_mul(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_MUL, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_div(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_DIV, temp, lvar, rvar));
    return temp;
}

Var *GenIR::gen_mod(Var *lvar, Var *rvar)
{
    Var *temp = new Var(lvar, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_MOD, temp, lvar, rvar));
    return temp;
}

//??????
Var *GenIR::gen_not(Var *var)
{
    Var *temp = new Var(var, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_NOT, temp, var)); //????????????tmp=!val
    return temp;
}

Var *GenIR::gen_minus(Var *var)
{
    Var *temp = new Var(var, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_NEG, temp, var)); //????????????tmp=-val
    return temp;
}

Var *GenIR::gen_bneg(Var *var)
{
    Var *temp = new Var(var, true); //????????????
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_BNEG, temp, var)); //????????????tmp=-val
    return temp;
}
//????????????

Var *GenIR::gen_lea(Var *var)
{
    Var *temp = new Var(Var::POINTER, true); //????????????
    //temp->set_pointer_deep(temp->get_pointer_deep() + 1);
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_LEA, temp, var)); //????????????tmp=&val
    return temp;
}

//??????
Var *GenIR::gen_ptr(Var *var)
{
    /*     Var *temp = new Var(var);
    temp->setLeft(true);   //???????????????????????????
    temp->setPointer(var); //??????????????????
    symtab.addVar(temp);   //??????????????????????????????????????????????????????
    return temp;
     */
}

Var *GenIR::gen_linc(Var *var)
{
    Var *temp = gen_assign(var);                                         //??????
    symtab.add_inst(new InterInst(IROP_ADD, var, var, var->get_step())); //????????????val++
    return temp;
}

Var *GenIR::gen_ldec(Var *var)
{
    Var *temp = gen_assign(var);                                         //??????
    symtab.add_inst(new InterInst(IROP_SUB, var, var, var->get_step())); //????????????val++
    return temp;
}

Var *GenIR::gen_rinc(Var *var)
{
    Var *temp = gen_assign(var);                                         //??????
    symtab.add_inst(new InterInst(IROP_ADD, var, var, var->get_step())); //????????????val++
    return temp;
}

Var *GenIR::gen_rdec(Var *var)
{
    Var *temp = gen_assign(var);                                         //??????
    symtab.add_inst(new InterInst(IROP_SUB, var, var, var->get_step())); //????????????val++
    return temp;
}

Var *GenIR::gen_get(Var *var)
{
    Var *temp = new Var(Var::INT, true);
    //temp->set_pointer_deep(temp->get_pointer_deep() - 1);
    gen_tempdecl(temp);
    symtab.add_inst(new InterInst(IROP_GET, temp, var));
    return temp;
}

void GenIR::gen_set(Var *result, Var *var)
{
    symtab.add_inst(new InterInst(IROP_SET, var, result));
}

void GenIR::gen_para(Var *arg) //??????????????????
{
    InterInst *argInst = new InterInst(IROP_ARG, arg); //push arg!!!
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
            Scope *scope = new Scope(symtab.new_scope_id(), true);
            vector<Var *> *para_vars = new vector<Var *>;
            int pl_tp_list_size = pl->tp_list.size();
            for (int i = 0; i < pl_tp_list_size; i++)
            {
                Var *para = new Var(pl->tp_list[i], pl->vid_list[i]->name);
                para_vars->push_back(para);
                scope->add_arg(para);
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
    symtab.add_inst(new InterInst(IROP_ENTRY, func));
    symtab.add_inst(func->lfb);
}

void GenIR::func_tail(Func *func)
{
    symtab.add_inst(func->lfe);
    symtab.add_inst(new InterInst(IROP_EXIT, func));
}

void GenIR::gen_body(Body *body, Scope *scope)
{
    bool scope_changed = false;
    Scope *newscope = scope;
    if (scope == NULL)
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
        if (decl->exp != NULL)
        {
            Var *initval = gen_expr(decl->exp);
            gen_assign(var, initval);
        }
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
    InterInst *_while, *_exit; //??????
    gen_whilehead(_while, _exit);
    Var *cond = gen_expr(stmt->exp1);
    gen_whilecond(cond, _exit);
    gen_stmt(stmt->stmt1, parent);
    gen_whiletail(_while, _exit);
    return;
}

void GenIR::gen_for(Stmt *stmt, Scope *parent)
{
    InterInst *_for, *_exit, *_step, *_stmt; //??????
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

void GenIR::gen_tempdecl(Var *var, bool add_to_scope)
{
    symtab.add_inst(new InterInst(IROP_DEC, var)); //????????????????????????
    if (add_to_scope)
        symtab.add_var(var);
}

//????????????-1??? ???????????????
void GenIR::gen_assert(Stmt *stmt, Scope *parent)
{
    Var *var1 = gen_expr(stmt->exp1);
    Var *var2 = gen_expr(stmt->exp2);

    InterInst *_exit;
    Var *cond = gen_expr(stmt->exp1);
    gen_ifhead(gen_neq(var1, var2), _exit);
    symtab.add_inst(new InterInst(IROP_RETV, symtab.cur_func->lfe, new Var(-1)));
    gen_elsetail(_exit);
    return;
}

void GenIR::gen_empty(Stmt *stmt, Scope *parent)
{
    symtab.add_inst(new InterInst(IROP_NOP));
    return;
}

void GenIR::gen_whilehead(InterInst *&_while, InterInst *&_exit)
{

    // InterInst* _blank=new InterInst();//_blank??????
    // symtab.add_inst(new InterInst(OP_JMP,_blank));//goto _blank

    _while = new NEWLABEL;   //??????while??????
    symtab.add_inst(_while); //??????while??????

    // symtab.add_inst(_blank);//??????_blank??????

    _exit = new InterInst(".L" + to_string(symtab.new_label_id())); //??????exit??????
    loop_push(_while, _exit);                                       //??????while
}

void GenIR::gen_whilecond(Var *cond, InterInst *_exit)
{
    if (cond)
    {
        /*
        if (cond->isVoid())
            cond = Var::getTrue(); //??????????????????
        else if (cond->is_ref())
            cond = genAssign(cond); //while(*p),while(a[0])
        */
        symtab.add_inst(new InterInst(IROP_JF, _exit, cond));
    }
}

void GenIR::gen_whiletail(InterInst *&_while, InterInst *&_exit)
{
    symtab.add_inst(new InterInst(IROP_JMP, _while)); //??????jmp??????
    symtab.add_inst(_exit);                           //??????exit??????
    loop_pop();                                       //??????while
}

void GenIR::gen_forhead(InterInst *&_for, InterInst *&_exit)
{
    _for = new NEWLABEL;  //??????for??????
    _exit = new NEWLABEL; //??????exit??????
    symtab.add_inst(_for);
}

void GenIR::gen_forcondbegin(Var *cond, InterInst *&_step, InterInst *&_stmt, InterInst *_exit)
{
    _stmt = new NEWLABEL; //??????block??????
    _step = new NEWLABEL; //????????????????????????
    if (cond)
    {
        
        symtab.add_inst(new InterInst(IROP_JF, _exit, cond));
        symtab.add_inst(new InterInst(IROP_JMP, _stmt)); //???????????????
    }
    symtab.add_inst(_step);  //????????????????????????
    loop_push(_step, _exit); //??????for
}

void GenIR::gen_forcondend(InterInst *_for, InterInst *_stmt)
{
    symtab.add_inst(new InterInst(IROP_JMP, _for)); //????????????
    symtab.add_inst(_stmt);                         //?????????????????????
}

void GenIR::gen_fortail(InterInst *&_step, InterInst *&_exit)
{
    symtab.add_inst(new InterInst(IROP_JMP, _step)); //?????????????????????
    symtab.add_inst(_exit);                          //??????_exit??????
    loop_pop();                                      //??????for
}

void GenIR::gen_ifhead(Var *cond, InterInst *&_else)
{
    _else = new NEWLABEL; //??????else??????
    if (cond)
    {
        /*
        if (cond->is_ref())
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
    _exit = new NEWLABEL; //??????exit??????
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
        case Asnop::ASN:
            gen_assign(lvar, gen_expr(simple->exp));
            break;
        case Asnop::ASNADD:
            gen_assign(lvar, gen_add(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNSUB:
            gen_assign(lvar, gen_sub(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNMUL:
            gen_assign(lvar, gen_mul(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNDIV:
            gen_assign(lvar, gen_div(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNMOD:
            gen_assign(lvar, gen_mod(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNLMO:
            gen_assign(lvar, gen_lmo(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNRMO:
            gen_assign(lvar, gen_rmo(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNAND:
            gen_assign(lvar, gen_band(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNXOR:
            gen_assign(lvar, gen_bxor(lvar, gen_expr(simple->exp)));
            break;
        case Asnop::ASNOR:
            gen_assign(lvar, gen_bor(lvar, gen_expr(simple->exp)));
            break;
        default:
            break;
        }

        break;
    case Simple::ADDADD:
        gen_assign(lvar, gen_add(lvar, lvar->get_step()));
        break;
    case Simple::SUBSUB:
        gen_assign(lvar, gen_sub(lvar, lvar->get_step()));
        break;
    case Simple::EXP:
        gen_expr(simple->exp);
        break;
    default:
        break;
    }
    return;
}

Var *GenIR::gen_assign(Var *val) //????????????????????????????????????????????????????????????
{
    Var *temp = new Var(val); //??????????????????
    gen_tempdecl(temp);
    /*
    if (val->is_ref())
    {
        //????????????tmp=*(val->ptr)
        symtab.addInst(new InterInst(OP_GET, temp, val->getPointer()));
    }
    */
    symtab.add_inst(new InterInst(IROP_ASN, temp, val)); //????????????tmp=val
    return temp;
}

Var *GenIR::gen_assign(Var *lvar, Var *rvar)
{
    Var *rtemp = rvar;
    if(lvar->is_pointer())
    {
        if(lvar->get_pointer_deep() == rvar->get_pointer_deep())
            symtab.add_inst(new InterInst(IROP_ASN, lvar, rtemp));
        else
        {
            gen_set(lvar, rtemp);
        }
    }
    else
    {
        if(rvar->is_ref())
        {
            rtemp = gen_lea(rvar);
        }
        if(rvar->is_pointer())
        {
            int deep = rvar->get_pointer_deep();
            for(int i = 0; i < deep; i ++)
            {
                rtemp = gen_get(rtemp);
            }
        }
        symtab.add_inst(new InterInst(IROP_ASN, lvar, rtemp));
    }
    
    return lvar;
}
