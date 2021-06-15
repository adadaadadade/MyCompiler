#include "compiler.h"
#include "error.h"
#include "scanner.h"
#include "lexer.h"
#include "parser.h"
#include "symtab.h"
#include "keywords.h"
#include "util.h"
#include "args.h"

int main(int argc, char *argv[])
{    
    Args args;
    Args::get_args(argc, argv);
    
    Scanner scanner(Args::srcfiles[0]);
    KeyWords();
    Lexer lexer(scanner);
    
    Symtab symtab;
    Parser parser(lexer, symtab);
    Prog* ast = parser.makeAST();
    
    //outputTokens(lexer);
}
