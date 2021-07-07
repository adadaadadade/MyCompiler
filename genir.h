#pragma once

#include "common.h"
#include "ast.h"
#include "intercode.h"

class GenIR
{
    InterCode &ir;
    Prog &ast;
    Symtab &symtab;

    vector<InterInst *> loop_heads;
    vector<InterInst *> loop_tails;
    void loop_push(InterInst *head, InterInst *tail);
    void loop_pop();

    Var *gen_lv(Lv *lv);
    Var *gen_tail_lv(Var *head, Lv *tail);
    Var *gen_dotfid_lv(Var *head, Lv *tail);
    Var *gen_tofid_lv(Var *head, Lv *tail);
    Var *gen_array_lv(Var *head, Lv *tail);

    Var *gen_expr(Exp *exp);
    
    Var *gen_vid_expr(Exp *exp);
    Var *gen_tail_expr(Var *head, Exp *tail);
    Var *gen_dotfid_expr(Var *head, Exp *tail);
    Var *gen_tofid_expr(Var *head, Exp *tail);
    Var *gen_array_expr(Var *head, Exp *tail);

    Var *gen_binop_expr(Exp *exp);
    Var *gen_unop_expr(Exp *exp);
    Var *gen_tri_expr(Exp *exp);
    Var *gen_func_expr(Exp *exp);
    
    //Var *gen_pointer_expr(Exp *exp);
    Var *gen_alloc_expr(Exp *exp);
    Var *gen_allocarray_expr(Exp *exp);
    //双目
    Var *gen_lor(Var *lvar, Var *rvar);
    Var *gen_land(Var *lvar, Var *rvar);
    Var *gen_bor(Var *lvar, Var *rvar);
    Var *gen_bxor(Var *lvar, Var *rvar);
    Var *gen_band(Var *lvar, Var *rvar);
    Var *gen_lth(Var *lvar, Var *rvar);
    Var *gen_leq(Var *lvar, Var *rvar);
    Var *gen_gth(Var *lvar, Var *rvar);
    Var *gen_geq(Var *lvar, Var *rvar);
    Var *gen_equ(Var *lvar, Var *rvar);
    Var *gen_neq(Var *lvar, Var *rvar);
    Var *gen_lmo(Var *lvar, Var *rvar);
    Var *gen_rmo(Var *lvar, Var *rvar);
    Var *gen_add(Var *lvar, Var *rvar);
    Var *gen_sub(Var *lvar, Var *rvar);
    Var *gen_mul(Var *lvar, Var *rvar);
    Var *gen_div(Var *lvar, Var *rvar);
    Var *gen_mod(Var *lvar, Var *rvar);
    //单目
    Var *gen_not(Var *var);
    Var *gen_minus(Var *var);
    Var *gen_bneg(Var *var);
    Var *gen_lea(Var *var); //取址语句
    Var *gen_ptr(Var *var); //指针取值语句
    Var *gen_linc(Var *var);
    Var *gen_ldec(Var *var);
    Var *gen_rinc(Var *var);
    Var *gen_rdec(Var *var);

    Var *gen_get(Var *var);
    void gen_get(Var *result, Var *var);
    Var *gen_set(Var *var);
    void gen_set(Var *result, Var *var);

    void gen_para(Var*arg);//参数传递语句

public:
    GenIR(InterCode &ir, Prog &ast, Symtab &symtab);
    ~GenIR();

    void gen_ir();

    void func_head(Func *func);
    void func_tail(Func *func);

    void gen_body(Body *body, Scope *scope = NULL);
    void gen_stmt(Stmt *stmt, Scope *parent);

    void gen_if(Stmt *stmt, Scope *parent);
    void gen_while(Stmt *stmt, Scope *parent);
    void gen_for(Stmt *stmt, Scope *parent);
    void gen_continue(Stmt *stmt, Scope *parent);
    void gen_break(Stmt *stmt, Scope *parent);
    void gen_return(Stmt *stmt, Scope *parent);
    void gen_assert(Stmt *stmt, Scope *parent);
    void gen_empty(Stmt *stmt, Scope *parent);

    //产生复合语句
    void gen_whilehead(InterInst *&_while, InterInst *&_exit);                                //while循环头部
    void gen_whilecond(Var *cond, InterInst *_exit);                                          //while条件
    void gen_whiletail(InterInst *&_while, InterInst *&_exit);                                //while尾部
    void gen_forhead(InterInst *&_for, InterInst *&_exit);                                    //for循环头部
    void gen_forcondbegin(Var *cond, InterInst *&_step, InterInst *&_stmt, InterInst *_exit); //for条件开始
    void gen_forcondend(InterInst *_for, InterInst *_stmt);                                   //for循环条件结束部分
    void gen_fortail(InterInst *&_step, InterInst *&_exit);                                   //for循环尾部
    void gen_ifhead(Var *cond, InterInst *&_else);                                            //if头部
    void gen_iftail(InterInst *&_else);                                                       //if尾部
    void gen_elsehead(InterInst *_else, InterInst *&_exit);                                   //else头部
    void gen_elsetail(InterInst *&_exit);

    void gen_simple(Simple *simple, Scope *parent);

    Var *gen_assign(Var *val); //变量拷贝赋值，用于指针左值引用和变量复制
    //赋值
    Var *gen_assign(Var *lvar, Var *rvar);

    //产生符号和语句
    Var *gen_array(Var *array, Var *index);             //数组索引语句
    Var *gen_call(Func *function, vector<Var *> &args); //函数调用语句
    Var *gen_twoop(Var *lval, IROP opt, Var *rval);     //双目运算语句
    Var *gen_oneopleft(IROP opt, Var *val);             //左单目运算语句
    Var *gen_oneopright(Var *val, IROP opt);            //右单目运算语句

    void gen_tempdecl(Var *var, bool add_to_scope = true); //产生临时变量初始化语句
};