#include "ast.h"

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

Gdecl::Gdecl()
{
}

string Gdecl::toString()
{
    return "Gdecl: " + to_string(type);
}

void Gdecl::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << type << endl;
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


Gdefn::Gdefn()
{
}

string Gdefn::toString()
{
    return "Gdefn: " + to_string(type);
}

void Gdefn::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
    switch (type)
    {
    case STRUCTDEFN:
        /* code */
        break;
    
    case FUNCDEFN:
        tp->print();
        vid->print();
        pl->print();
        body->print();
        break;
    
    case TYPEDEF:
        /* code */
        break;
    
    default:
        break;
    }
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
        tp_list[i]->print();
    for(int i = 0; i < vid_list.size(); i ++)
        vid_list[i]->print();
}

string Paralist::get_nodetype()
{
    return this->nodetype;
}


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
    tp->print();
    vid->print();
    if(exp != NULL)
        exp->print();
}

string Decl::get_nodetype()
{
    return this->nodetype;
}


Stmt::Stmt()
{
}

string Stmt::toString()
{
    return "Stmt: " + to_string(type);
}

void Stmt::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Stmt::get_nodetype()
{
    return this->nodetype;
}


Simple::Simple()
{
}

string Simple::toString()
{
    return "Simple: " + to_string(type);
}

void Simple::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Simple::get_nodetype()
{
    return this->nodetype;
}


Lv::Lv()
{
}

string Lv::toString()
{
    return "Lv: " + to_string(type);
}

void Lv::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
}

string Lv::get_nodetype()
{
    return this->nodetype;
}


Tp::Tp()
{
}

Tp::Tp(TpType type):type(type)
{
}

string Tp::toString()
{
    return "Tp: " + to_string(type);
}

void Tp::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
    switch (type)
    {
    case INT:
        
        break;
    
    case BOOL:
        
        break;
    
    case STRING:
        
        break;
    
    case CHAR:
        
        break;
    
    case POINTER:
        tp_tail->print();
        break;
    
    case ARRAY:
        tp_tail->print();
        break;
    
    case STRUCT:
        sid->print();
        break;
    
    case AID:
        aid->print();
        break;
    
    default:
        break;
    }
}

string Tp::get_nodetype()
{
    return this->nodetype;
}


Exp::Exp()
{
}

string Exp::toString()
{
    return "Exp: " + to_string(type);
}

void Exp::print()
{
    for(int i = 0; i < (deep = parent->deep + 1); i ++)
        printf("\t");
    cout << toString() << endl;
    switch (type)
    {
    case NUMLIT:
        numlit->print();
        break;
    
    case STRLIT:
        numlit->print();
        break;
    
    case CHRLIT:
        numlit->print();
        break;
    
    case BOOL:
        numlit->print();
        break;
    
    case NULLTYPE:
        numlit->print();
        break;
    
    case VID:
        numlit->print();
        break;
    
    case BINOP:
        numlit->print();
        break;
    
    case UNOP:
        numlit->print();
        break;
    
    case TRI:
        numlit->print();
        break;
    
    case FUNC:
        numlit->print();
        break;
    
    case DOTFID:
        numlit->print();
        break;
    
    case TOFID:
        numlit->print();
        break;
    
    case ARRAY:
        
        break;
    
    case POINTER:
        numlit->print();
        break;
    
    case ALLOC:
        numlit->print();
        break;
    
    case ALLOCARRAY:
        numlit->print();
        break;
    
    default:
        break;
    }
    if(exp_tail != NULL)
        exp_tail->print();
}

string Exp::get_nodetype()
{
    return this->nodetype;
}


Unop::Unop()
{
}

string Unop::toString()
{
    return "Unop: " + to_string(type);
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
    return "Binop: " + to_string(type);
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


Asnop::Asnop()
{
}

string Asnop::toString()
{
    return "Asnop: " + to_string(type);
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