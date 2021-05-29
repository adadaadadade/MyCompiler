#include "parser.h"

#define _(T) ||look->tag==T
#define F(C) look->tag==C

#define FUNOP F(NOT)_(BNEG)_(SUB)_(MUL)
#define FTP F(KW_INT)_(KW_BOOL)_(KW_STRING)_(KW_CHAR)_(ID)
#define FEXP (FUNOP)_(LPAREN)_(NUMLIT)_(STRLIT)_(KW_TRUE)_(KW_FALSE)_(KW_NULL)_(KW_ALLOC)_(KW_ALLOC_ARRAY)_(MUL)_(ID)
#define FSIMPLE (FEXP)_(ID)_(MUL)
#define FSTMT (FSIMPLE)_(KW_IF)_(KW_WHILE)_(KW_FOR)_(KW_CONTINUE)_(KW_BREAK)_(KW_RETURN)_(LBRACE)_(KW_ASSERT)_(SEMICON)
#define FLVTAIL F(DOT)_(SUB)_(LBRACK)

Parser::Parser(Lexer &x) : lexer(x)
{
}

Parser::~Parser()
{
}

void Parser::move()
{
    look = lexer.nextToken();
    //if(Args::showToken)printf("%s\n",look->toString().c_str());//输出词法记号——测试
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

    return ast;
}

Prog* Parser::prog()
{
    Prog* prog = new Prog();
    prog->deep = 0;
    prog->parent = NULL;
    while(!F(END))
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
    Tp* t = new Tp();
    Vid* v = new Vid();
    Paralist* pl = new Paralist();

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
        gdecl->type = gdecl->FUNCDECL;
        gdecl->tp = tp; tp->set_parent(gdecl);
        gdecl->vid = vid; vid->set_parent(gdecl);
        gdecl->pl = pl; pl->set_parent(gdecl);

        p->gdecl_list.push_back(gdecl); gdecl->set_parent(p);
    }
    else if (match(LBRACE))
    {
        Gdefn* gdefn = new Gdefn();
        
        gdefn->type = gdefn->FUNCDEFN;
        gdefn->tp = tp; tp->set_parent(gdefn);
        gdefn->vid = vid; vid->set_parent(gdefn);
        gdefn->pl = pl; pl->set_parent(gdefn);

        match(LBRACE);
        gdefn->body = body(); gdefn->body->set_parent(gdefn);
        match(RBRACE);

        p->gdefn_list.push_back(gdefn); gdefn->set_parent(p);
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
        d->exp = exp(); d->exp->set_parent(d);
    }
    else
        d->exp = NULL;
    
    return d;
}


Node* Parser::stmtlist(Body* body)
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
        s->exp1 = exp(); s->exp1->set_parent(s);
        match(RPAREN); 
        s->stmt1 = stmt(); s->stmt1->set_parent(s);
        if(match(KW_ELSE))
        {
            s->stmt2 = stmt(); s->exp2->set_parent(s);
        }
        else
            s->stmt2 = NULL;
        break;
    
    case KW_WHILE:
        s->type = s->WHILE;

        match(KW_WHILE);
        match(LPAREN);
        s->exp1 = exp(); s->exp1->set_parent(s);
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
            s->simple2 = simple(); s->simple1->set_parent(s);
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
            s->exp1 = exp(); s->exp1->set_parent(s);
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
        s->exp1 = exp(); s->exp1->set_parent(s);
        match(COMMA);
        s->exp2 = exp(); s->exp2->set_parent(s);
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
    if(FEXP)
    {
        si->type = si->EXP;

        si->exp = exp();
    }
    else
    {
        si->lv = lv();
        if(match(ADD))
        {
            si->type = si->ADDADD;

            match(ADD);
        }
        else if(match(SUB))
        {
            si->type = si->SUBSUB;

            match(SUB);
        }
        else
        {
            si->type = si->ASN;

            si->asnop = asnop(); si->asnop->set_parent(si);
            si->exp = exp(); si->exp->set_parent(si);
        }
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
    l->vid = v;
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
        i->lv = temp;
        i = temp;
        if(match(DOT))
        {
            i->type = i->LVDOTVID;
            
            i->fid = fid(); i->fid->set_parent(i);
        }
        else if(match(SUB))
        {
            i->type = i->LVTOVID;

            match(GTH);
            i->fid = fid(); i->fid->set_parent(i);

        }
        else if(match(LBRACK))
        {
            i->type = i->LVARRAY;

            i->exp = exp(); i->exp->set_parent(i);
            match(RBRACE);
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
    else
    {
        tp->type = tp->AID;
        tp->aid = aid();
    }
}


void Parser::tp_tail(Tp* tp)
{
    Tp* i;
    i = tp;
    while(F(MUL)_(LBRACK))
    {
        Tp* temp = new Tp();
        i->tp = temp;
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
    else
    {
        tp->type = tp->AID;
        tp->aid = aid();
    }
}



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
    }
    return(e);
}


void Parser::exp_base(Exp* e)
{
    switch (look->tag)
    {
    case LPAREN:
        match(CHRLIT);
        break;
    
    case NUMLIT:
        e->numlit = num(); e->numlit->set_parent(e);

        break;
    
    case STRLIT:
        e->strlit = str(); e->strlit->set_parent(e);
        break;
    
    case CHRLIT:
        e->chrlit = chr(); e->chrlit->set_parent(e);
        break;
    
    case KW_TRUE:
        match(KW_TRUE);
        break;
    
    case KW_FALSE:
        match(KW_FALSE);
        break;
    
    case KW_NULL:
        match(KW_NULL);
        break;
    
    case KW_ALLOC:
        match(KW_ALLOC);
        break;
    
    case KW_ALLOC_ARRAY:
        match(KW_ALLOC_ARRAY);
        break;
    
    case MUL:
        match(MUL);
        break;
    
    case ID:
        match(ID);
        break;
    //////
    default:

        break;
    }
}


void Parser::call_ornot(Exp* exp)
{

}


void Parser::call_paralist(Exp* exp)
{

}


void Parser::call_para(Exp* exp)
{

}


void Parser::exp_tail(Exp* exp)
{

}



Vid* Parser::vid()
{

}


Sid* Parser::sid()
{

}


Fid* Parser::fid()
{

}


Aid* Parser::aid()
{

}


Unop* Parser::unop()
{

}


Binop* Parser::binop()
{

}


Asnop* Parser::asnop()
{

}