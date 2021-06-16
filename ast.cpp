#include "ast.h"

#define P(x) if(x)x->print()

string Node::toString()
{
    return "Node";
}

void Node::print()
{
    cout << toString() << endl;
}

void Node::set_parent(Node* par)
{
    parent = par;
    deep = par->deep + 1;
}

string Node::get_nodetype()
{
    return this->nodetype;
}

Prog::Prog()
{
}

string Prog::toString()
{
    return "Prog";
}

void Prog::print()
{
    for(int i = 0; i < deep; i ++)
        printf("\t");
    
    cout << toString() << endl;
    for(int i = 0; i < this->gdecl_list.size(); i ++)
        this->gdecl_list[i]->print();
    for(int i = 0; i < this->gdefn_list.size(); i ++)
        this->gdefn_list[i]->print();
    
    //programs->print();
    //next->print();
}

string Prog::get_nodetype()
{
    return this->nodetype;
}

string Gdecl::TypeName[] = {
    "STRUCTDECL",
    "FUNCDECL"
};

Gdecl::Gdecl()
{
}

string Gdecl::toString()
{
    return "Gdecl: " + TypeName[type];
}

void Gdecl::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
    switch (type)
    
    {
    case FUNCDECL :
        tp->print();
        vid->print();
        pl->print();
        break;
    
    case STRUCTDECL :
        vid->print();
        break;

    default:
        break;
    }
}

string Gdecl::get_nodetype()
{
    return this->nodetype;
}

string Gdefn::TypeName[] = {
    "STRUCTDEFN",
    "FUNCDEFN",
    "TYPEDEF"
};

Gdefn::Gdefn()
{
}

string Gdefn::toString()
{
    return "Gdefn: " + TypeName[type];
}

void Gdefn::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
    P(sid);
    for(int i = 0; i < tp_list.size(); i ++)
    {
        tp_list[i]->print();
        fid_list[i]->print();
    }
    P(pl);
    P(tp);
    P(vid);
    P(body);
    P(aid);
}

string Gdefn::get_nodetype()
{
    return this->nodetype;
}


Paralist::Paralist()
{
}

string Paralist::toString()
{
    return "Paralist";
}

void Paralist::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
    for(int i = 0; i < tp_list.size(); i ++)
    {
        tp_list[i]->print();
        vid_list[i]->print();
    }
}

string Paralist::get_nodetype()
{
    return this->nodetype;
}

/*
Call_Paralist::Call_Paralist()
{
}

string Call_Paralist::toString()
{
    return "Call_Paralist";
}

void Call_Paralist::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;


}

string Call_Paralist::get_nodetype()
{
    return this->nodetype;
}
*/

Body::Body()
{
}

string Body::toString()
{
    return "Body";
}

void Body::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
    for(int i = 0; i < decl_list.size(); i ++)
        ((Decl*)decl_list[i])->print();
    for(int i = 0; i < stmt_list.size(); i ++)
        ((Stmt*)stmt_list[i])->print();
}

string Body::get_nodetype()
{
    return this->nodetype;
}


Decl::Decl()
{
}

string Decl::toString()
{
    return "Decl";
}

void Decl::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
    
    P(tp);
    P(vid);
    P(exp);

}

string Decl::get_nodetype()
{
    return this->nodetype;
}

string Stmt::TypeName[] = {
    "SIMPLE",
    "IF",
    "WHILE",
    "FOR",
    "CONTINUE",
    "BREAK",
    "RETURN",
    "BODY",
    "ASSERT",
    "EMPTY"
};

Stmt::Stmt()
{
}

string Stmt::toString()
{
    return "Stmt: " + TypeName[type];
}

void Stmt::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;

    P(simple1);
    P(simple2);
    P(exp1);
    P(exp2);
    P(stmt1);
    P(stmt2);
    P(body);
}

string Stmt::get_nodetype()
{
    return this->nodetype;
}

string Simple::TypeName[] = {
    "ASN",
    "ADDADD",
    "SUBSUB",
    "EXP"
};

Simple::Simple()
{
}

string Simple::toString()
{
    return "Simple: " + TypeName[type];
}

void Simple::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;

    P(lv);
    P(asnop);
    P(exp);
}

string Simple::get_nodetype()
{
    return this->nodetype;
}


string Lv::TypeName[] = {
    "VID",
    "LVDOTVID",
    "LVTOVID",
    "LVPOINTER",
    "LVARRAY"
};

Lv::Lv()
{
}

string Lv::toString()
{
    return "Lv: " + TypeName[type];
}

void Lv::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;

    P(vid);
    P(lv);
    P(fid);
    P(exp);
    P(lv_tail);
}

string Lv::get_nodetype()
{
    return this->nodetype;
}

string Tp::TypeName[] = {
    "INT",
    "BOOL",
    "STRING",
    "CHAR",
    "POINTER",
    "ARRAY",
    "STRUCT",
    "AID"
};

Tp::Tp()
{
}

Tp::Tp(TpType type):type(type)
{
}

string Tp::toString()
{
    return "Tp: " + TypeName[type];
}

void Tp::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
    
    P(tp);
    P(sid);
    P(aid);
    P(tp_tail);

}

string Tp::get_nodetype()
{
    return this->nodetype;
}

string Exp::TypeName[] = {
    "PAREN",
    "NUMLIT",
    "STRLIT",
    "CHRLIT",
    "TRUELIT",
    "FALSELIT",
    "NULLLIT",
    "VID",
    "BINOP",
    "UNOP",
    "TRI",
    "FUNC",
    "DOTFID",
    "TOFID",
    "ARRAY",
    "POINTER",
    "ALLOC",
    "ALLOCARRAY"
};

Exp::Exp()
{
}

Exp* Exp::binop_exp(Exp* lval,TokenType tag, Exp* rval)
{
    Exp* result = new Exp();
    result->type = BINOP;
    result->exp1 = lval; result->exp1->set_parent(result);
    result->exp2 = rval; result->exp2->set_parent(result);
    result->tag = tag;

    return result;
}

Exp* Exp::unop_exp(TokenType tag, Exp* e)
{
    Exp* result = new Exp();
    result->type = UNOP;
    result->exp1 = e; result->exp1->set_parent(e);
    result->tag = tag;

    return result;
}

string Exp::toString()
{
    if(type == BINOP)
        return "Exp: " + TypeName[type] + ",val:" + tokenName[tag];
    if(type == NUMLIT)
        return "Exp: " + TypeName[type] + ",val:" + to_string(numlit);
    if(type == CHRLIT)
        return "Exp: " + TypeName[type] + ",val:" + to_string(chrlit);
    if(type == STRLIT)
        return "Exp: " + TypeName[type] + ",val:" + strlit;
    return "Exp: " + TypeName[type];
}

void Exp::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
    P(exp1);
    P(exp2);
    P(exp3);
    P(vid);
    P(fid);
    P(tp);
    P(exp_tail);
}

string Exp::get_nodetype()
{
    return this->nodetype;
}

/*
string Unop::TypeName[] = {
    "NOT",
    "BNEG",
    "SUB",
    "MUL",
    "INC",
    "DEC"
};


Unop::Unop()
{
}

string Unop::toString()
{
    return "Unop: " + TypeName[type];
}

void Unop::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Unop::get_nodetype()
{
    return this->nodetype;
}


Binop::Binop()
{
}

string Binop::toString()
{
    return "Binop: " + TypeName[type];
}

void Binop::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Binop::get_nodetype()
{
    return this->nodetype;
}
*/

string Asnop::TypeName[] = {
    "ASN",
    "ASNADD",
    "ASNSUB",
    "ASNMUL",
    "ASNDIV",
    "ASNMOD",
    "ASNLMO",
    "ASNRMO",
    "ASNAND",
    "ASNXOR",
    "ASNOR"
};

Asnop::Asnop()
{
}

string Asnop::toString()
{
    return "Asnop: " + TypeName[type];
}

void Asnop::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Asnop::get_nodetype()
{
    return this->nodetype;
}


Str::Str()
{
}

string Str::toString()
{
    return "Str: " + str;
}

void Str::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Str::get_nodetype()
{
    return this->nodetype;
}


Chr::Chr()
{
}

string Chr::toString()
{
    return "Chr: " + ch;
}

void Chr::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Chr::get_nodetype()
{
    return this->nodetype;
}


Num::Num()
{
}

string Num::toString()
{
    return "Num: " + to_string(value);
}

void Num::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Num::get_nodetype()
{
    return this->nodetype;
}


Bool::Bool()
{
}

string Bool::toString()
{
    if (boollit)
        return "Bool: true";
    else
        return "Bool: false";
}

void Bool::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Bool::get_nodetype()
{
    return this->nodetype;
}


Vid::Vid()
{
}

string Vid::toString()
{
    return "Vid: " + name;
}

void Vid::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Vid::get_nodetype()
{
    return this->nodetype;
}


Sid::Sid()
{
}

string Sid::toString()
{
    return "Sid: " + name;
}

void Sid::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Sid::get_nodetype()
{
    return this->nodetype;
}


Fid::Fid()
{
}

string Fid::toString()
{
    return "Fid: " + name;
}

void Fid::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Fid::get_nodetype()
{
    return this->nodetype;
}


Aid::Aid()
{
}

string Aid::toString()
{
    return "Aid: " + name;
}

void Aid::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Aid::get_nodetype()
{
    return this->nodetype;
}