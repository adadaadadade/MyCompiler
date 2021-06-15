#include "parser.h"
#include "error.h"
#include "symbol.h"
#include "symtab.h"
#include "args.h"

#define _(T) ||look->tag==T
#define F(C) look->tag==C

#define ISAID F(ID)&&(symtab.get_typedef(((Id*)look)->name)!=NULL)

#define FUNOP F(NOT)_(BNEG)_(SUB)_(MUL)_(INC)_(DEC)
#define FBINOP F(ADD)_(SUB)_(MUL)_(DIV)_(MOD)_(LMO)_(RMO)_(LTH)_(GTH)_(EQU)_(NEQ)_(BAND)_(BXOR)_(BOR)_(LAND)_(LOR)
#define FTP F(KW_INT)_(KW_BOOL)_(KW_STRING)_(KW_CHAR)||(ISAID)
#define FEXP (FUNOP)_(LPAREN)_(NUMLIT)_(STRLIT)_(KW_TRUE)_(KW_FALSE)_(KW_NULL)_(KW_ALLOC)_(KW_ALLOC_ARRAY)_(MUL)_(ID)
#define FSIMPLE (FEXP)_(ID)_(MUL)
#define FSTMT (FSIMPLE)_(KW_IF)_(KW_WHILE)_(KW_FOR)_(KW_CONTINUE)_(KW_BREAK)_(KW_RETURN)_(LBRACE)_(KW_ASSERT)_(SEMICON)
#define FLVTAIL F(DOT)_(ARROW)_(LBRACK)
#define FASNOP F(ASN)_(ASNADD)_(ASNSUB)_(ASNMUL)_(ASNDIV)_(ASNLMO)_(ASNRMO)_(ASNAND)_(ASNXOR)_(ASNOR)

Parser::Parser(Lexer &lexer, Symtab &symtab) : lexer(lexer), symtab(symtab)
{
}

Parser::~Parser()
{
}

void Parser::output_error()
{

}

void Parser::move()
{
    look = lexer.nextToken();
    if(Args::show_token)
    {
        printf("%s",look->toString().c_str());//输出词法记号——测试
        if(look->tag == END)
            printf("\n");
    }
    if(look->tag == ERR || look->tag == COMMIT || look->tag == DEF)
        move();
}

bool Parser::match(TokenType t)
{
    if(look->tag == t)
    {
        move();
        return true;
    }
    else
        return false;
}



Prog* Parser::makeAST()
{
    Prog* ast;
    move();
    ast = prog();
    if(Args::show_ast)
        ast->print();
    return ast;
}

Prog* Parser::prog()
{
    Prog* prog = new Prog();
    prog->deep = 0;
    prog->parent = NULL;
    //printf("look.tag = %d\n", look->tag);
    while(!(F(END)))
    {
        if(F(KW_TYPEDEF))
            typedef_def(prog);
        else if(F(KW_STRUCT))
            struct_base(prog);
        else
            gfn(prog);
    }

    return prog;
}

void Parser::typedef_def(Prog* p)
{
    Gdefn* gdefn = new Gdefn();

    match(KW_TYPEDEF);
    gdefn->tp = tp(); gdefn->tp->set_parent(gdefn);
    gdefn->aid =  aid(); gdefn->aid->set_parent(gdefn);
    match(SEMICON);

    Var* v = new Var(gdefn->tp, gdefn->aid->name);
    symtab.add_typedef(v);

    gdefn->type = Gdefn::TYPEDEF;
    p->gdefn_list.push_back(gdefn); gdefn->set_parent(p);
    return;
}

void Parser::struct_base(Prog* p)
{
    Sid* s;

    match(KW_STRUCT);
    s = sid();

    struct_decl_ordefn_orfn(p, s);
}



void Parser::struct_decl_ordefn_orfn(Prog* p, Sid* sid)
{
    if(match(SEMICON))
    {
        Gdecl* gdecl = new Gdecl();
        gdecl->type = gdecl->STRUCTDECL;

        Vid* vid = new Vid();
        vid->name = sid->name;
        gdecl->vid = vid; vid->set_parent(gdecl);

        p->gdecl_list.push_back(gdecl); gdecl->set_parent(p);
    }
    else if(match(LBRACE))
    {
        Gdefn* gdefn = new Gdefn();
        gdefn->type = gdefn->STRUCTDEFN;
        gdefn->sid = sid; sid->set_parent(gdefn);

        struct_datalist(gdefn);
        match(RBRACE);
        match(SEMICON);
        
        p->gdefn_list.push_back(gdefn); gdefn->set_parent(p);
    }
    else if(F(MUL)_(LBRACK))
    {
        Tp* t = new Tp();
        t->type = t->STRUCT;
        t->sid = sid; sid->set_parent(t);
        Vid* v = new Vid();
        Paralist* pl = new Paralist();

        tp_tail(t);
        v = vid();
        match(LPAREN);
        pl = paralist();
        match(RPAREN);
        fn_decl_ordefn(p, t, v, pl);

    }
}


void Parser::struct_datalist(Gdefn* gdefn)
{
    while(FTP)
    {
        gdefn->tp_list.push_back(tp()); gdefn->tp_list.back()->set_parent(gdefn);
        gdefn->fid_list.push_back(fid()); gdefn->fid_list.back()->set_parent(gdefn);
        match(SEMICON);
    }
}



void Parser::gfn(Prog* p)
{
    Tp* t;
    Vid* v;
    Paralist* pl;
    
    t = tp_without_struct();
    v = vid();
    match(LPAREN);
    pl = paralist();
    match(RPAREN);
    fn_decl_ordefn(p, t, v, pl);
}


void Parser::fn_decl_ordefn(Prog* p, Tp* tp, Vid* vid, Paralist* pl)
{
    if(match(SEMICON))
    {
        Gdecl* gdecl = new Gdecl();
        gdecl->set_parent(p);

        gdecl->type = gdecl->FUNCDECL;
        gdecl->tp = tp; tp->set_parent(gdecl);
        gdecl->vid = vid; vid->set_parent(gdecl);
        gdecl->pl = pl; pl->set_parent(gdecl);

        p->gdecl_list.push_back(gdecl);
    }
    else if (match(LBRACE))
    {
        Gdefn* gdefn = new Gdefn();
        gdefn->set_parent(p);
        
        gdefn->type = gdefn->FUNCDEFN;
        gdefn->tp = tp; tp->set_parent(gdefn);
        gdefn->vid = vid; vid->set_parent(gdefn);
        gdefn->pl = pl; pl->set_parent(gdefn);
        gdefn->body = body(); gdefn->body->set_parent(gdefn);
        
        match(RBRACE);

        p->gdefn_list.push_back(gdefn); 
    }
}


Paralist* Parser::paralist()
{
    Paralist* pl = new Paralist();
    if(FTP)
    {
        pl->tp_list.push_back(tp()); pl->tp_list.back()->set_parent(pl);
        pl->vid_list.push_back(vid()); pl->vid_list.back()->set_parent(pl);
        para(pl);
    }
    return pl;
}


void Parser::para(Paralist* paralist)
{
    while(match(COMMA))
    {
        paralist->tp_list.push_back(tp()); paralist->tp_list.back()->set_parent(paralist);
        paralist->vid_list.push_back(vid()); paralist->vid_list.back()->set_parent(paralist);
    }
}



Body* Parser::body()
{
    Body* b = new Body();
    decllist(b);
    stmtlist(b);
    return b;
}



void Parser::decllist(Body* body)
{
    while(FTP)
    {
        body->decl_list.push_back(decl()); body->decl_list.back()->set_parent(body);
    }
}


Decl* Parser::decl()
{
    Decl* d = new Decl();
    
    d->tp = tp(); d->tp->set_parent(d);
    d->vid = vid(); d->vid->set_parent(d);
    if(match(ASN))
    {
        d->exp = expr(); d->exp->set_parent(d);
    }
    else
        d->exp = NULL;
    match(SEMICON);
    return d;
}


void Parser::stmtlist(Body* body)
{
    while (FSTMT)
    {
        body->stmt_list.push_back(stmt()); body->stmt_list.back()->set_parent(body);
    }
}


Stmt* Parser::stmt()
{
    Stmt* s = new Stmt();
    switch (look->tag)
    {
    case KW_IF:
        s->type = s->IF;
        match(KW_IF);
        match(LPAREN);
        s->exp1 = expr(); s->exp1->set_parent(s);
        match(RPAREN); 
        s->stmt1 = stmt(); s->stmt1->set_parent(s);
        if(match(KW_ELSE))
        {
            s->stmt2 = stmt(); s->stmt2->set_parent(s);
        }
        else
            s->stmt2 = NULL;
        break;
    
    case KW_WHILE:
        s->type = s->WHILE;

        match(KW_WHILE);
        match(LPAREN);
        s->exp1 = expr(); s->exp1->set_parent(s);
        match(RPAREN);
        s->stmt1 = stmt(); s->stmt1->set_parent(s);
        break;
    
    case KW_FOR:
        s->type = s->FOR;

        match(KW_FOR);
        match(LPAREN);
        if(FSIMPLE)
        {
            s->simple1 = simple(); s->simple1->set_parent(s);
        }
        match(SEMICON);
        if(FSIMPLE)
        {
            s->simple2 = simple(); s->simple2->set_parent(s);
        }
        match(RPAREN);
        s->stmt1 = stmt(); s->stmt1->set_parent(s);
        break;
    
    case KW_CONTINUE:
        s->type = s->CONTINUE;

        match(KW_CONTINUE);
        match(SEMICON);
        break;
    
    case KW_BREAK:
        s->type = s->BREAK;

        match(KW_BREAK);
        match(SEMICON);
        break;
    
    case KW_RETURN:
        s->type = s->RETURN;

        match(KW_RETURN);
        if(FEXP)
        {
            s->exp1 = expr(); s->exp1->set_parent(s);
        }
        match(SEMICON);
        break;
    
    case LBRACE:
        s->type = s->BODY;

        match(LBRACE);
        s->body = body(); s->body->set_parent(s);
        match(RBRACE);
        break;
    
    case KW_ASSERT:
        s->type = s->ASSERT;

        match(KW_ASSERT);
        match(LPAREN);
        s->exp1 = expr(); s->exp1->set_parent(s);
        match(COMMA);
        s->exp2 = expr(); s->exp2->set_parent(s);
        match(RPAREN);
        match(SEMICON);
        break;
    
    case SEMICON:
        s->type = s->EMPTY;

        match(SEMICON);
        break;
    
    default:
        s->type = s->SIMPLE;

        s->simple1 = simple(); s->simple1->set_parent(s);
        match(SEMICON);
        break;
    }

    return s;
}



Simple* Parser::simple()
{
    Simple* si = new Simple();
    if (F(ID)_(MUL))
    {
        si->lv = lv(); si->lv->set_parent(si);
        if(FASNOP)
        {
            si->type = si->ASN;
            si->asnop = asnop(); si->asnop->set_parent(si);
            si->exp = expr(); si->exp->set_parent(si);
        }
        else if(match(INC))
        {
            si->type = si->ADDADD;
        }
        else if(match(DEC))
        {
            si->type = si->SUBSUB;
        }
    }
    else if(FEXP)
    {
        si->type = si->EXP;
        si->exp = expr();
    }
    else
    {
        output_error();
    }

    return si;
}



Lv* Parser::lv()
{
    Lv* l = new Lv();

    if(match(MUL))
    {
        l->type = l->LVPOINTER;
        l->lv = lv(); l->lv->set_parent(l);
    }
    else
    {
        lv_base(l);
        lv_tail(l);
    }
    return l;
}


void Parser::lv_base(Lv* l)
{
    Vid* v = vid();
    l->vid = v; l->vid->set_parent(l);
}


void Parser::lv_tail(Lv* l)
{
    Lv* i;
    i = l;
    if(!FLVTAIL)
    {
        l->type = l->VID;
        l->lv = NULL;
    }
    while(FLVTAIL)
    {
        Lv* temp = new Lv();
        i->lv_tail = temp; i->lv_tail->set_parent(i);
        i = temp;
        if(match(DOT))
        {
            i->type = i->LVDOTVID;
            i->fid = fid(); i->fid->set_parent(i);
        }
        else if(match(ARROW))
        {
            i->type = i->LVTOVID;
            i->fid = fid(); i->fid->set_parent(i);
        }
        else if(match(LBRACK))
        {
            i->type = i->LVARRAY;
            i->exp = expr(); i->exp->set_parent(i);
            match(RBRACK);
        }
    }
}



Tp* Parser::tp()
{
    Tp* t = new Tp();

    tp_base(t);
    tp_tail(t);

    return t;
}


void Parser::tp_base(Tp* tp)
{
    if(match(KW_INT))
        tp->type = tp->INT;
    else if(match(KW_BOOL))
        tp->type = tp->BOOL;
    else if(match(KW_STRING))
        tp->type = tp->STRING;
    else if(match(KW_CHAR))
        tp->type = tp->CHAR;
    else if(match(KW_STRUCT))
    {
        tp->type = tp->STRUCT;
        tp->sid = sid();
    }
    else if(ISAID)
    {
        tp->type = tp->AID;
        tp->aid = aid();tp->aid->set_parent(tp);
    }
}


void Parser::tp_tail(Tp* tp)
{
    Tp* i;
    i = tp;
    while(F(MUL)_(LBRACK))
    {
        Tp* temp = new Tp();
        i->tp_tail = temp; i->tp_tail->set_parent(i);
        i = temp;

        if(match(MUL))
        {
            i->type = i->POINTER;
        }
        else if(match(LBRACK))
        {
            i->type = i->ARRAY;
            match(RBRACK);
        }
    }
}


Tp* Parser::tp_without_struct()
{
    Tp* t = new Tp();

    tp_without_struct_base(t);
    tp_tail(t);

    return t;
}


void Parser::tp_without_struct_base(Tp* tp)
{
    if(match(KW_INT))
        tp->type = tp->INT;
    else if(match(KW_BOOL))
        tp->type = tp->BOOL;
    else if(match(KW_STRING))
        tp->type = tp->STRING;
    else if(match(KW_CHAR))
        tp->type = tp->CHAR;
    else if(ISAID)
    {
        tp->type = tp->AID;
        tp->aid = aid(); tp->aid->set_parent(tp);
    }
}


/*
Exp* Parser::exp()
{
    Exp* e = new Exp();

     if(FUNOP)
    {
        e->type = e->UNOP;

        e->unop = unop(); e->unop->set_parent(e);
        e->exp1 = exp(); e->exp1->set_parent(e);
    }
    else
    { 
    exp_base(e);
    exp_tail(e);
    //}
    return(e);
}


void Parser::exp_base(Exp* e)
{
    switch (look->tag)
    {
    case LPAREN:
        match(LPAREN);
        e->exp1 = exp(); e->exp1->set_parent(e);
        match(RPAREN);

        break;
    
    case NUMLIT:
        e->type = e->NUMLIT;
        e->numlit = num(); e->numlit->set_parent(e);
        break;
    
    case STRLIT:
        e->type = e->STRLIT;
        e->strlit = str(); e->strlit->set_parent(e);
        break;
    
    case CHRLIT:
        e->type = e->CHRLIT;
        e->chrlit = chr(); e->chrlit->set_parent(e);
        break;
    
    case KW_TRUE:
        e->type = e->BOOL;
        e->boollit = boo(); e->boollit->set_parent(e);
        break;
    
    case KW_FALSE:
        e->type = e->BOOL;
        e->boollit = boo(); e->boollit->set_parent(e);
        break;
    
    case KW_NULL:
        e->type = e->NULLTYPE;
        match(KW_NULL);
        break;
    
    case KW_ALLOC:
        e->type = e->ALLOC;
        match(KW_ALLOC);
        match(LPAREN);
        e->tp = tp(); e->tp->set_parent(e);
        match(RPAREN);
        break;
    
    case KW_ALLOC_ARRAY:
        e->type = e->ALLOCARRAY;
        match(KW_ALLOC_ARRAY);
        match(LPAREN);
        e->tp = tp(); e->tp->set_parent(e);
        match(COMMA);
        e->exp1 = exp(); e->exp1->set_parent(e);
        match(RPAREN);
        break;
    
    case MUL:
        e->type = e->POINTER;
        match(MUL);
        e->exp1 = exp();  e->exp1->set_parent(e);
        break;
    
    case ID:
        e->type = e->VID;
        e->vid = vid(); e->vid->set_parent(e);
        match(LPAREN);
        e->cpl = call_paralist(); e->cpl->set_parent(e);
        match(RPAREN);

        break;
    default:
        if(FUNOP)
        {
            e->type = e->UNOP;
            e->unop = unop(); e->unop->set_parent(e);
            e->exp1 = exp(); e->exp1->set_parent(e);
        }
        break;
    }
}


void Parser::exp_tail(Exp* e)
{
    Exp* i;
    i = e;
    while((FBINOP)_(QUE)_(DOT)_(SUB)_(LBRACK))
    {
        Exp* temp = new Exp();
        i->exp_tail = temp;
        i = temp;
        switch (look->tag)
        {
        case QUE:
            i->type = i->TRI;
            match(QUE);
            i->exp1 = exp(); i->exp1->set_parent(i);
            match(COLON);
            i->exp2 = exp(); i->exp2->set_parent(i);
            break;

        case DOT:
            i->type = i->DOTFID;
            match(DOT);
            i->fid = fid(); i->fid->set_parent(i);
            break;

        case SUB:
            i->type = i->POINTER;
            match(SUB);
            match(GTH);
            i->fid = fid(); i->fid->set_parent(i);
        
        case LBRACK:
            i->type = i->ARRAY;
            match(LBRACK);
            i->exp1 = exp(); i->exp1->set_parent(i);
            match(RBRACK);
            break;
        
        default:
            i->binop = binop(); i->binop->set_parent(i);
            i->exp1 = exp(); i->exp1->set_parent(i);
            break;
        }
    }
}


Call_Paralist* Parser::call_paralist()
{
    Call_Paralist* cpl = new Call_Paralist();
    if(FEXP)
    {
        cpl->exp_list.push_back((Node*)exp()); cpl->exp_list.back()->set_parent(cpl);
        call_para(cpl);
    }
    return cpl;
}


void Parser::call_para(Call_Paralist* call_paralist)
{
    while(match(COMMA))
    {
        call_paralist->exp_list.push_back((Node*)exp()); call_paralist->exp_list.back()->set_parent(call_paralist);
    }
}
*/
Exp* Parser::expr()
{
    return triexpr();
}

Exp* Parser::triexpr()
{
    Exp* lval = lorexpr();
    return tritail(lval);
}


Exp* Parser::tritail(Exp* lval)
{
    if(match(QUE))
    {
        Exp* e = new Exp();
        e->type = e->TRI;

        e->exp1 = lval; e->exp1->set_parent(e);
        Exp* e2 = lorexpr();
        e->exp2 = tritail(e2); e->exp2->set_parent(e);
        match(COLON);
        Exp* e3 = lorexpr();
        e->exp3 = tritail(e3); e->exp3->set_parent(e);

        return e;
    }
    return lval;
}


Exp* Parser::lorexpr()
{
    Exp* lval = landexpr();
    return lortail(lval);
}


Exp* Parser::lortail(Exp* lval)
{
    if(match(LOR))
    {
        Exp* rval = landexpr();
        Exp* result = Exp::binop_exp(lval, LOR, rval);
        return lortail(result);
    }
    return lval;
}


Exp* Parser::landexpr()
{
    Exp* lval = borexpr();
    return landtail(lval);
}


Exp* Parser::landtail(Exp* lval)
{
    if(match(LAND))
    {
        Exp* rval = borexpr();
        Exp* result = Exp::binop_exp(lval, LAND, rval);
        return landtail(result);
    }
    return lval;
}


Exp* Parser::borexpr()
{
    Exp* lval = bxorexpr();
    return bortail(lval);
}


Exp* Parser::bortail(Exp* lval)
{
    if(match(BOR))
    {
        Exp* rval = bxorexpr();
        Exp* result = Exp::binop_exp(lval, BOR, rval);
        return bortail(result);
    }
    return lval;
}

Exp* Parser::bxorexpr()
{
    Exp* lval = bandexpr();
    return bxortail(lval);
}

Exp* Parser::bxortail(Exp* lval)
{
    if(match(BXOR))
    {
        Exp* rval = bandexpr();
        Exp* result = Exp::binop_exp(lval, BXOR, rval);
        return bxortail(result);
    }
    return lval;
}


Exp* Parser::bandexpr()
{
    Exp* lval = equexpr();
    return bandtail(lval);
}


Exp* Parser::bandtail(Exp* lval)
{
    if(match(BAND))
    {
        Exp* rval = equexpr();
        Exp* result = Exp::binop_exp(lval, BAND, rval);
        return bandtail(result);
    }
    return lval;
}


Exp* Parser::equexpr()
{
    Exp* lval = cmpexpr();
    return equtail(lval);
}


Exp* Parser::equtail(Exp* lval)
{
    if(match(EQU))
    {
        Exp* rval = cmpexpr();
        Exp* result = Exp::binop_exp(lval, EQU, rval);
        return equtail(result);
    }
    else if(match(NEQ))
    {
        Exp* rval = cmpexpr();
        Exp* result = Exp::binop_exp(lval, NEQ, rval);
        return equtail(result);
    }
    return lval;
}

Exp* Parser::cmpexpr()
{
    Exp* lval = shiftexpr();
    return cmptail(lval);
}


Exp* Parser::cmptail(Exp* lval)
{
    if(match(LTH))
    {
        Exp* rval = shiftexpr();
        Exp* result = Exp::binop_exp(lval, LTH, rval);
        return cmptail(result);
    }
    else if(match(GTH))
    {
        Exp* rval = shiftexpr();
        Exp* result = Exp::binop_exp(lval, GTH, rval);
        return cmptail(result);
    }
    else if(match(LET))
    {
        Exp* rval = shiftexpr();
        Exp* result = Exp::binop_exp(lval, LET, rval);
        return cmptail(result);
    }
    else if(match(GET))
    {
        Exp* rval = shiftexpr();
        Exp* result = Exp::binop_exp(lval, GET, rval);
        return cmptail(result);
    }
    return lval;
}


Exp* Parser::shiftexpr()
{
    Exp* lval = aloexpr();
    return shifttail(lval);
}


Exp* Parser::shifttail(Exp* lval)
{
    if(match(LMO))
    {
        Exp* rval = aloexpr();
        Exp* result = Exp::binop_exp(lval, LMO, rval);
        return shifttail(result);
    }
    else if(match(RMO))
    {
        Exp* rval = aloexpr();
        Exp* result = Exp::binop_exp(lval, RMO, rval);
        return shifttail(result);
    }
    return lval;
}


Exp* Parser::aloexpr()
{
    Exp* lval = mlusexpr();
    return alotail(lval);
}


Exp* Parser::alotail(Exp* lval)
{
    if(match(ADD))
    {
        Exp* rval = mlusexpr();
        Exp* result = Exp::binop_exp(lval, ADD, rval);
        return alotail(result);
    }
    else if(match(SUB))
    {
        Exp* rval = mlusexpr();
        Exp* result = Exp::binop_exp(lval, SUB, rval);
        return alotail(result);
    }
    return lval;
}



Exp* Parser::mlusexpr()
{
    Exp* lval = unopexpr();
    return mulstail(lval);
}


Exp* Parser::mulstail(Exp* lval)
{
    if(match(MUL))
    {
        Exp* rval = unopexpr();
        Exp* result = Exp::binop_exp(lval, MUL, rval);
        return mulstail(result);
    }
    else if(match(DIV))
    {
        Exp* rval = unopexpr();
        Exp* result = Exp::binop_exp(lval, DIV, rval);
        return mulstail(result);
    }
    else if(match(MOD))
    {
        Exp* rval = unopexpr();
        Exp* result = Exp::binop_exp(lval, MOD, rval);
        return mulstail(result);
    }

    return lval;
}



Exp* Parser::unopexpr()
{
    if(match(NOT))
    {
        Exp* e = unopexpr();
        return Exp::unop_exp(NOT, e);
    }
    else if(match(BNEG))
    {   
        Exp* e = unopexpr();
        return Exp::unop_exp(BNEG, e);
    }
    else if(match(SUB))
    {
        Exp* e = unopexpr();
        return Exp::unop_exp(SUB, e);
    }
    else if(match(MUL))
    {
        Exp* e = unopexpr();
        return Exp::unop_exp(MUL, e);
    }
    else if(match(INC))
    {
        Exp* e = unopexpr();
        return Exp::unop_exp(INC, e);
    }
    else if(match(DEC))
    {
        Exp* e = unopexpr();
        return Exp::unop_exp(DEC, e);
    }
    else
    {
        return elem();
    }
}

/*
<elem> ::= LPAREN <expr> RPAREN
    | ID <idexpr>
    | <literal>
*/
Exp* Parser::elem()
{
    Exp* e = new Exp();
    if(match(LPAREN))
    {
        e->type = Exp::PAREN;
        e->exp1 = expr(); e->exp1->set_parent(e);
        match(RPAREN);
        return e;
    }
    else if(F(ID))
    {
        e->type = Exp::VID;
        e->vid = vid(); e->vid->set_parent(e);
        idexpr(e);
        return(e);
    }
    else if (F(KW_ALLOC)_(KW_ALLOC_ARRAY))
    {
        return allocs();
    }
    else
    {
        return literal();
    }
}


Exp* Parser::idexpr(Exp* e)
{
    Exp* exp = new Exp();
    if(match(LBRACK))
    {
        exp = expr();
        e->exp1 = exp; e->exp1->set_parent(e);
        exp->type = Exp::ARRAY;
        match(RBRACK);
        return idexpr(exp);

    }
    else if(match(LPAREN))
    {
        exp = expr();
        e->exp1 = exp; e->exp1->set_parent(e);
        exp->type = Exp::FUNC;
        match(RPAREN);
        return idexpr(exp);
    }
    else if(match(DOT))
    {
        exp->fid = fid(); exp->fid->set_parent(e);
        e->exp1 = exp; e->exp1->set_parent(e);
        exp->type = Exp::DOTFID;
        return idexpr(exp);
    }
    else if(match(ARROW))
    {
        exp->fid = fid(); exp->fid->set_parent(e);
        e->exp1 = exp; e->exp1->set_parent(e);
        exp->type = Exp::TOFID;
        return idexpr(exp);
    }
    return e;
}


Exp* Parser::realarg(Exp* e)
{
    if(FEXP)
    {
        Exp* a = arg();
        return arglist(a);
    }
    return e;
}


Exp* Parser::arg()
{
    return expr();
}


Exp* Parser::arglist(Exp* e)
{
    if(match(COMMA))
    {
        Exp* next = arg();
        Exp* list = new Exp();
        list->exp_tail = next; list->exp_tail->set_parent(list);
        return arglist(list);
    }
    return e;
}


Exp* Parser::allocs()
{
    Exp* exp = new Exp();
    if(match(KW_ALLOC))
    {
        exp->type = Exp::ALLOC;

        match(LPAREN);
        exp->tp = tp(); exp->tp->set_parent(exp);
        match(RPAREN);
        return exp;
    }
    else if(match(KW_ALLOC_ARRAY))
    {
        exp->type = Exp::ALLOCARRAY;
        
        match(LPAREN);
        exp->tp = tp(); exp->tp->set_parent(exp);
        match(COMMA);
        exp->exp1 = expr(); exp->exp1->set_parent(exp);
        match(RPAREN);
        return exp;
    }
    else
    {
        output_error();
    }
}


Exp* Parser::literal()
{
    Exp* lit = new Exp();
    if(look->tag == NUMLIT)
    {
        lit->type = Exp::NUMLIT;
        lit->numlit = ((Numlit*)look)->val;
        match(NUMLIT);
        return lit;
    }
    else if(look->tag == CHRLIT)
    {
        lit->type = Exp::CHRLIT;
        lit->chrlit = ((Chrlit*)look)->ch;
        match(CHRLIT);
        return lit;
    }
    else if(look->tag == STRLIT)
    {
        lit->type = Exp::STRLIT;
        lit->strlit = ((Strlit*)look)->str;
        match(STRLIT);
        return lit;
    }
    else if(match(KW_TRUE))
    {
        lit->type = Exp::TRUELIT;
        return lit;
    }
    else if(match(KW_FALSE))
    {
        lit->type = Exp::FALSELIT;
        return lit;
    }
    else if(match(KW_NULL))
    {
        lit->type = Exp::NULLLIT;
        return lit;
    }
    else
    {
        output_error();
    }
}




Vid* Parser::vid()
{
    Vid* v = new Vid();
    if(F(ID))
    {
        v->name = ((Id*)look)->name;
        match(ID);
    }
    else
    {
        output_error();
    }
    return v;
}


Sid* Parser::sid()
{
    Sid* s = new Sid();
    if(F(ID))
    {
        s->name = ((Id*)look)->name;
        match(ID);
    }
    else
    {
        output_error();
    }
    return s;
}


Fid* Parser::fid()
{
    Fid* f = new Fid();
    if(F(ID))
    {
        f->name = ((Id*)look)->name;
        match(ID);
    }
    else
    {
        output_error();
    }
    return f;
}


Aid* Parser::aid()
{
    Aid* a = new Aid();
    if(F(ID))
    {
        a->name = ((Id*)look)->name;
        match(ID);
    }
    else
    {
        output_error();
    }
    return a;
}


Num* Parser::num()
{
    Num* n = new Num();
    if(look->tag == NUMLIT)
    {
        n->value = ((Numlit*)look)->val;
        match(NUMLIT);
    }
    else
        output_error();
    return n;
}
Str* Parser::str()
{
    Str* s = new Str();
    if(look->tag = STRLIT)
    {
        s->str = ((Strlit*)look)->str;
        match(STRLIT);
    }
    else
        output_error();
    return s;
}

Chr* Parser::chr()
{
    Chr* ch = new Chr();
    if(look->tag == CHRLIT)
    {
        ch->ch = ((Chrlit*)look)->ch;
        match(CHRLIT);
    }
    else
        output_error();
    return ch;
}

Bool* Parser::boo()
{
    Bool* b = new Bool();
    if(look->tag == KW_TRUE)
    {
        b->boollit = true;
        match(KW_TRUE);
    }
        
    else if(look->tag == KW_FALSE)
    {
        b->boollit = false;
        match(KW_FALSE);
    }
    else
        output_error();
    return b;
}

/*
Unop* Parser::unop()
{
    Unop* op = new Unop();
    switch (look->tag)
    {
    case NOT:
        op->type = op->NOT;
        match(NOT);
        break;
    
    case BNEG:
        op->type = op->BNEG;
        match(BNEG);
        break;
    
    case SUB:
        match(SUB);
        if(match(SUB))
        {
            op->type = op->DEC;
        }
        else
        {
            op->type = op->SUB;
        }
        break;
    
    case MUL:
        op->type = op->MUL;
        match(MUL);
        break;

    case ADD:
        op->type = op->INC;
        match(ADD);
        if(!match(ADD))
            output_error();
        break;

    default:
        output_error();
        break;
    }
    return op;
}


Binop* Parser::binop()
{
    Binop* op = new Binop();
    switch (look->tag)
    {
    case ADD:
        op->type = op->ADD;
        match(ADD);
        break;
    
    case SUB:
        op->type = op->SUB;
        match(SUB);
        break;
    
    case MUL:
        op->type = op->MUL;
        match(MUL);
        break;
    
    case DIV:
        op->type = op->DIV;
        match(DIV);
        break;
    
    case MOD:
        op->type = op->MOD;
        match(MOD);
        break;
    
    case LMO:
        op->type = op->LMO;
        match(LMO);
        break;
    
    case RMO:
        op->type = op->RMO;
        match(RMO);
        break;
    
    case LTH:
        op->type = op->LTH;
        match(LTH);
        break;
    
    case GTH:
        op->type = op->GTH;
        match(GTH);
        break;
    
    case EQU:
        op->type = op->EQU;
        match(EQU);
        break;
    
    case NEQ:
        op->type = op->NEQ;
        match(NEQ);
        break;
    
    case AND:
        op->type = op->AND;
        match(AND);
        break;
    
    case XOR:
        op->type = op->XOR;
        match(XOR);
        break;
    
    case OR:
        op->type = op->OR;
        match(OR);
        break;
    
    case LAND:
        op->type = op->LAND;
        match(LAND);
        break;
    
    case LOR:
        op->type = op->LOR;
        match(LOR);
        break;
    
    default:
        output_error();
        break;
    }
    return op;
}
*/

Asnop* Parser::asnop()
{
    Asnop* op = new Asnop();
    switch (look->tag)
    {
    case ASN:
        op->type = op->ASN;
        match(ASN);
        break;
    
    case ASNADD:
        op->type = op->ASNADD;
        match(ASNADD);
        break;
    
    case ASNSUB:
        op->type = op->ASNSUB;
        match(ASNSUB);
        break;
    
    case ASNMUL:
        op->type = op->ASNMUL;
        match(ASNMUL);
        break;
    
    case ASNDIV:
        op->type = op->ASNDIV;
        match(ASNDIV);
        break;
    
    case ASNMOD:
        op->type = op->ASNMOD;
        match(ASNMOD);
        break;
    
    case ASNLMO:
        op->type = op->ASNLMO;
        match(ASNLMO);
        break;
    
    case ASNRMO:
        op->type = op->ASNRMO;
        match(ASNRMO);
        break;
    
    case ASNAND:
        op->type = op->ASNAND;
        match(ASNAND);
        break;
    
    case ASNXOR:
        op->type = op->ASNXOR;
        match(ASNXOR);
        break;
    
    case ASNOR:
        op->type = op->ASNOR;
        match(ASNOR);
        break;
    
    default:
        output_error();
        break;
    }
    return op;
}
