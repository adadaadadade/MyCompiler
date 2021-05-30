#include <iostream>
#include <cstring>
#include "compiler.h"
#include "error.h"
#include "scanner.h"
#include "lexer.h"
#include "parser.h"
#include "keywords.h"
#include "util.h"


int main(int argc, char *argv[])
{
    vector<char *> srcfiles;//源文件
    
    if(argc > 1)
    {
        for(int i = 1; i < argc; i ++){//取最后一个参数之前的参数
			srcfiles.push_back(argv[i]);
		}
        Scanner scanner(srcfiles[0]);
        Lexer lexer(scanner);
        Parser parser(lexer);
        KeyWords();
        //outputScanner(scanner);
        //outputTokens(lexer);
    }
    else
    {

    }
    return 0;
}
