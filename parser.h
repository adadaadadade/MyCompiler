#pragma once

#include "common.h"
#include "lexer.h"
#include "ast.h"

class Parser
{

    Lexer &lexer;
    Symtab &symtab;

    Prog* AST;
    Token* look;
    vector <Token*> token_buf;
    
    void move();
    bool match(TokenType t);

    void output_error();

    Prog* prog();
    void typedef_def(Prog* p);
    void struct_base(Prog* p);

    void struct_decl_ordefn_orfn(Prog* p, Sid* sid);
    void struct_datalist(Gdefn* gdefn);

    void gfn(Prog* p);
    void fn_decl_ordefn(Prog* p, Tp* tp, Vid* vid, Paralist* pl);
    Paralist* paralist();
    void para(Paralist* paralist);

    Body* body();

    void decllist(Body* body);
    Decl* decl();
    void init();

    void stmtlist(Body* body);
    Stmt* stmt();
    void else_tail(Stmt* stmt);
    void exp_ornot(Stmt* stmt);

    Simple* simple();

    Lv* lv();
    void lv_base(Lv* l);
    void lv_tail(Lv* l);

    Tp* tp();
    void tp_base(Tp* tp);
    void tp_tail(Tp* tp);
    Tp* tp_without_struct();
    void tp_without_struct_base(Tp* tp);

    Exp* expr();
    Exp* triexpr();
    Exp* tritail(Exp* lval);
    Exp* lorexpr();
    Exp* lortail(Exp* lval);
    Exp* landexpr();
    Exp* landtail(Exp* lval);
    Exp* borexpr();
    Exp* bortail(Exp* lval);
    Exp* bxorexpr();
    Exp* bxortail(Exp* lval);
    Exp* bandexpr();
    Exp* bandtail(Exp* lval);
    Exp* equexpr();
    Exp* equtail(Exp* lval);
    //Binop* equ_binop();
    Exp* cmpexpr();
    Exp* cmptail(Exp* lval);
    //Binop* cmp_binop();
    Exp* shiftexpr();
    Exp* shifttail(Exp* lval);
    //Binop* shift_binop();
    Exp* aloexpr();
    Exp* alotail(Exp* lval);
    Exp* alo_binop(Exp* lval);
    Exp* mlusexpr();
    Exp* mulstail(Exp* lval);
    //Binop* muls_binop();
    Exp* unopexpr();
    //Unop* unop();
    Exp* elem();
    Exp* idexpr(Exp* e);
    Exp* realarg(Exp* e);
    Exp* arg();
    Exp* arglist(Exp* e);
    Exp* allocs();
    Exp* literal();

    Vid* vid();
    Sid* sid();
    Fid* fid();
    Asnop* asnop();
    Aid* aid();
    Num* num();
    Str* str();
    Chr* chr();
    Bool* boo();

    //Aid* match_aid();
    bool next_is_aid();

public:

    Prog* makeAST();
    Prog* sema_analysis();

    Parser(Lexer &lexer, Symtab &symtab);
    ~Parser();
};
