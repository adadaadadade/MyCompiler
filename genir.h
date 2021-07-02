#pragma once

#include "common.h"

class GenIR
{
    InterCode &ir;
    Prog &ast;
    Symtab &symtab;

    void func_ir(Gdefn *gdefn);
public:
    GenIR(InterCode &ir, Prog &ast, Symtab &symtab);
    ~GenIR();

    void make_ir();
};