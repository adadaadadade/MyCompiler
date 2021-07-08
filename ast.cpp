#include "ast.h"
#include "symtab.h"

#define P(x) if(x)x->print()

extern const string tokenName[];

string Node::to_string()
{
    return "Node";
}

void Node::print()
{
    cout << to_string() << endl;
}

void Node::set_parent(Node *par)
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

string Prog::to_string()
{
    return "Prog";
}

void Prog::print()
{
    for (int i = 0; i < deep; i++)
        printf("\t");

    cout << to_string() << endl;
    for (int i = 0; i < this->gdecl_gdefn_list.size(); i++)
        this->gdecl_gdefn_list[i]->print();

    //programs->print();
    //next->print();
}

string Prog::get_nodetype()
{
    return this->nodetype;
}

string Gdecl::TypeName[] = {
    "STRUCTDECL",
    "FUNCDECL"};

Gdecl::Gdecl()
{
    vid = NULL;
    tp = NULL;
    pl = NULL;
}

string Gdecl::to_string()
{
    return "Gdecl : " + TypeName[type];
}

void Gdecl::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
    switch (type)

    {
    case FUNCDECL:
        tp->print();
        vid->print();
        pl->print();
        break;

    case STRUCTDECL:
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
    "TYPEDEF"};

Gdefn::Gdefn()
{
    sid = NULL;
    tp = NULL;
    vid = NULL;
    body = NULL;
    aid = NULL;
}

string Gdefn::to_string()
{
    return "Gdefn : " + TypeName[type];
}

void Gdefn::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
    P(sid);
    for (int i = 0; i < tp_list.size(); i++)
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

string Paralist::to_string()
{
    return "Paralist";
}

void Paralist::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
    for (int i = 0; i < tp_list.size(); i++)
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

string Body::to_string()
{
    return "Body";
}

void Body::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
    for (int i = 0; i < decl_list.size(); i++)
        ((Decl *)decl_list[i])->print();
    for (int i = 0; i < stmt_list.size(); i++)
        ((Stmt *)stmt_list[i])->print();
}

string Body::get_nodetype()
{
    return this->nodetype;
}

Decl::Decl()
{
    tp = NULL;
    vid = NULL;
    exp = NULL;
}

string Decl::to_string()
{
    return "Decl";
}

void Decl::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;

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
    "EMPTY"};

Stmt::Stmt()
{
    simple1 = NULL;
    simple2 = NULL;
    exp1 = NULL;
    exp2 = NULL;
    stmt1 = NULL;
    stmt2 = NULL;
    body = NULL;
}

string Stmt::to_string()
{
    return "Stmt : " + TypeName[type];
}

void Stmt::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;

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
    "EXP"};

Simple::Simple()
{
    lv = NULL;
    asnop = NULL;
    exp = NULL;
}

string Simple::to_string()
{
    return "Simple : " + TypeName[type];
}

void Simple::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;

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
    "LVARRAY"};

Lv::Lv()
{
    vid = NULL;
    lv = NULL;
    lv_tail = NULL;
    fid = NULL;
    exp = NULL;
}

string Lv::to_string()
{
    return "Lv: " + TypeName[type];
}

void Lv::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;

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
    "AID"};

Tp::Tp()
{
    tp_tail = NULL;
    sid = NULL;
    aid = NULL;
    size = 0;
}

Tp::Tp(TpType type) : type(type)
{
    switch (type)
    {
    case Tp::INT:
        //size = 4;
        break;
    case Tp::CHAR:
        //size = 2;
        break;
    case Tp::BOOL:
        //size = 2;
        break;
    case Tp::STRING:

        break;
    case Tp::STRUCT:

        break;
    case Tp::ARRAY:

        break;
    default:
        break;
    }
}

Tp::Tp(Tp *tp)
{
    if (tp->sid != NULL)
    {
        sid = new Sid();
        sid->name = tp->sid->name;
    }
    if (tp->aid != NULL)
    {
        aid = new Aid();
        aid->name = tp->sid->name;
    }
    if (tp->tp_tail != NULL)
    {
        tp_tail = new Tp(tp->tp_tail);
        tp_tail->set_parent(this);
    }
    type = tp->type;
    size = tp->size;
}

void Tp::add_tail(Tp *tp)
{
    Tp *i = this;
    while (i->tp_tail != NULL)
        i = i->tp_tail;
    i->tp_tail = tp;
    if(tp)
        tp->set_parent(i);
}

string Tp::to_string()
{
    if (type == ARRAY)
        return "Tp : " + TypeName[type] + ", size : " + std::to_string(size);
    return "Tp : " + TypeName[type];
}

void Tp::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;

    //P(tp);
    P(sid);
    P(aid);
    P(tp_tail);
    //printf("%p", tp_tail);
}

string Tp::get_nodetype()
{
    return this->nodetype;
}
/*
int Tp::get_base_size()
{
    switch (type)
    {
    case INT:
        base_size = int_size;
        break;
    case CHAR:
        base_size = char_size;
        break;
    case BOOL:
        base_size = bool_size;
        break;
    case POINTER:
        base_size = pointer_size;
        break;
    case STRING:
        base_size = string_size;
        break;
    case STRUCT:
        base_size = string_size;
        break;
    case AID:
        //base_size = Symtab::get_typedef(aid->name);
        break;
    case ARRAY:
        //base_size = string_size;
        break;
    default:
        break;
    }
    return base_size;
}

int Tp::get_size()
{

}
*/
string Exp::TypeName[] = {
    "PAREN",
    "NUMLIT",
    "STRLIT",
    "CHRLIT",
    "BOOLIT",
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
    "ALLOCARRAY"};

Exp::Exp()
{
    exp1 = NULL;
    exp2 = NULL;
    exp3 = NULL;
    exp_tail = NULL;
    vid = NULL;
    fid = NULL;
    tp = NULL;
    is_array_index = false;
    is_func_arg = false;
}

Exp *Exp::binop_exp(Exp *lval, TokenType tag, Exp *rval)
{
    Exp *result = new Exp();
    result->type = BINOP;
    result->exp1 = lval;
    result->exp1->set_parent(result);
    result->exp2 = rval;
    result->exp2->set_parent(result);
    result->tag = tag;

    return result;
}

Exp *Exp::unop_exp(TokenType tag, Exp *e)
{
    Exp *result = new Exp();
    result->type = UNOP;
    result->exp1 = e;
    result->exp1->set_parent(result);
    result->tag = tag;

    return result;
}

string Exp::to_string()
{
    string str = "Exp : ";
    if (is_array_index)
        str = "Array Index Exp : ";
    if (is_func_arg)
        str = "Func Arg Exp : ";
    if (type == BINOP)
        return str + TypeName[type] + ",val :" + tokenName[tag];
    if (type == UNOP)
        return str + TypeName[type] + ",val :" + tokenName[tag];
    if (type == NUMLIT)
        return str + TypeName[type] + ",val :" + std::to_string(numlit);
    if (type == CHRLIT)
        return str + TypeName[type] + ",val :" + std::to_string(chrlit);
    if (type == STRLIT)
        return str + TypeName[type] + ",val :" + strlit;

    return str + TypeName[type];
}

void Exp::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
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
    "ASNOR"};

Asnop::Asnop()
{
}

string Asnop::to_string()
{
    return "Asnop: " + TypeName[type];
}

void Asnop::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
}

string Asnop::get_nodetype()
{
    return this->nodetype;
}

Str::Str()
{
}

string Str::to_string()
{
    return "Str : " + str;
}

void Str::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
}

string Str::get_nodetype()
{
    return this->nodetype;
}

Chr::Chr()
{
}

string Chr::to_string()
{
    return "Chr : " + ch;
}

void Chr::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
}

string Chr::get_nodetype()
{
    return this->nodetype;
}

Num::Num()
{
}

string Num::to_string()
{
    return "Num : " + std::to_string(value);
}

void Num::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
}

string Num::get_nodetype()
{
    return this->nodetype;
}

Bool::Bool()
{
}

string Bool::to_string()
{
    if (boollit)
        return "Bool : true";
    else
        return "Bool : false";
}

void Bool::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
}

string Bool::get_nodetype()
{
    return this->nodetype;
}

Vid::Vid()
{
}

string Vid::to_string()
{
    return "Vid: " + name;
}

void Vid::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
}

string Vid::get_nodetype()
{
    return this->nodetype;
}

Sid::Sid()
{
}

string Sid::to_string()
{
    return "Sid : " + name;
}

void Sid::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
}

string Sid::get_nodetype()
{
    return this->nodetype;
}

Fid::Fid()
{
}

string Fid::to_string()
{
    return "Fid : " + name;
}

void Fid::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
}

string Fid::get_nodetype()
{
    return this->nodetype;
}

Aid::Aid()
{
}

string Aid::to_string()
{
    return "Aid : " + name;
}

void Aid::print()
{
    for (int i = 0; i < (deep = parent->deep + 1); i++)
        printf("\t");
    cout << to_string() << endl;
}

string Aid::get_nodetype()
{
    return this->nodetype;
}