#include "genir.h"
#include "intercode.h"
#include "ast.h"
#include "symbol.h"
#include "symtab.h"

GenIR::GenIR(InterCode &ir, Prog &ast, Symtab &symtab) : ir(ir), ast(ast), symtab(symtab)
{
}

GenIR::~GenIR()
{
    
}

void GenIR::func_ir(Gdefn *gdefn)
{
    Func *func = new Func();
    //ir->add_inst(InterInst());
    Paralist* pl = gdefn->pl;
    Body* body = gdefn->body;
   // ir->add_inst(InterInst());
    //symtab.add_func(func)
}

void GenIR::make_ir()
{
    int gdefn_size = ast.gdefn_list.size();
    for(int i = 0; i < gdefn_size; i ++)
    {
        Gdefn *gdefn = ast.gdefn_list[i];
        if(gdefn->type == Gdefn::FUNCDEFN)
        {
            func_ir(gdefn);
        }
    }
}