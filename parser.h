#include "common.h"
#include "lexer.h"
#include "ast.h"
#include "token.h"

class Parser
{

    Lexer &lexer;
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

    Exp* exp();
    void exp_base(Exp* exp);
    void call_ornot(Exp* exp);
    Call_Paralist* call_paralist();
    void call_para(Call_Paralist* call_paralist);
    void exp_tail(Exp* exp);

    Vid* vid();
    Sid* sid();
    Fid* fid();
    Aid* aid();
    Unop* unop();
    Binop* binop();
    Asnop* asnop();
    Num* num();
    Str* str();
    Chr* chr();
    Bool* boo();

public:

    Prog* makeAST();

    Parser(Lexer &lexer);
    ~Parser();
};
