#include "symbol.h"


Var::Var(Tp *tp, string name):type(tp), name(name) //typedef aid
{
}


Var::~Var()
{
    
}

 //
string Var::get_name()
{
    return name;
}


void Var::set_name(string name)
{
    this->name = name;
}

