#include "common.h"
#include "compiler.h"
#include "error.h"
#include "scanner.h"
#include "lexer.h"
#include "parser.h"
#include "symtab.h"
#include "keywords.h"
#include "util.h"
#include "args.h"
#include "genir.h"
#include "intercode.h"

int main(int argc, char *argv[])
{    
    Args args;
    Args::get_args(argc, argv);
    
    Scanner scanner(Args::srcfiles[0]);
    Error error(&scanner);
    KeyWords();
    Lexer lexer(scanner);
    
    Symtab symtab;
    Symtab::set_symtab(&symtab);
    Parser parser(lexer, symtab);
    Prog* ast = parser.makeAST();
    if (Error::get_error_num() == 0)
        ast = parser.sema_analysis();
    InterCode ir;
    if (Error::get_error_num() == 0)
        GenIR genir(ir, *ast, symtab);

    //outputTokens(lexer);
}
