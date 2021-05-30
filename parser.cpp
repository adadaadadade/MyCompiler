#include "parser.h"

#define _(T) ||look->tag==T
#define F(C) look->tag==C

#define FUNOP F(NOT)_(BNEG)_(SUB)_(MUL)
#define FBINOP F(ADD)_(SUB)_(MUL)_(DIV)_(MOD)_(LMO)_(RMO)_(LTH)_(GTH)_(EQU)_(NEQ)_(AND)_(XOR)_(OR )_(LAND)_(LOR)
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

void Parser::output_error()
{

}

void Parser::move()
{
    look = lexer.nextToken();
    if(look->tag == ERR || look->tag == COMMIT || look->tag == DEF)
        move();
    else
    //if(Args::showToken)
        printf("%s\n",look->toString().c_str());//输出词法记号——测试
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
    //printf("prog\n");
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
        d->exp = exp(); d->exp->set_parent(d);
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
        i->lv_tail = temp;
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
        i->tp_tail = temp;
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

/*     if(FUNOP)
    {
        e->type = e->UNOP;

        e->unop = unop(); e->unop->set_parent(e);
        e->exp1 = exp(); e->exp1->set_parent(e);
    }
    else
    { */
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
        e->exp1 = exp();
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
        e->exp1 = exp();
        match(RPAREN);
        break;
    
    case MUL:
        e->type = e->POINTER;
        match(MUL);
        e->exp1 = exp();
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
        op->type = op->SUB;
        match(SUB);
        break;
    
    case MUL:
        op->type = op->MUL;
        match(MUL);
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