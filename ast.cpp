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
    //programs->print();
    //next->print();
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
}

Tp::Tp()
{
}

string Tp::toString()
{
    return "Tp: " + to_string(type);
}

void Tp::print()
{
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
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
    for(int i = 0; i < deep; i ++)
        printf("\t");
    cout << toString() << endl;
}