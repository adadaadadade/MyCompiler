#include "error.h"
#include "scanner.h"

int Error::error_num = 0;//错误个数
int Error::warn_num = 0;	//警告个数
Scanner* Error::scanner=NULL;

Error::Error(Scanner *sc)
{
    Error::scanner = sc;
}

Error::~Error()
{

}

int Error::get_error_num()
{
    return error_num;
}

int Error::get_warn_num()
{
    return warn_num;
}


//错误接口
void Error::lex_error(LexError code)//打印词法错误
{
    error_num ++;

    printf(ERROR);
    printf("LexError : %d\n", code);

    //exit(0);
}

void Error::syn_error(SynError code,Token *t)//打印语法错误
{
    error_num ++;

    printf(ERROR);
    printf("SynError : %d, File : %s, line : %d, col : %d\n", code, scanner->get_file(), scanner->get_line(), scanner->get_col());

    //exit(0);
}

void Error::sem_error(SemError code,string name)//打印语义错误
{
    error_num ++;

    printf(ERROR);
    printf("SemError : %d, name : %s\n", code, name.c_str());

    //exit(0);
}

void Error::fatal()
{
    printf(FATAL);
    exit(1);
}

void Error::sem_Warn(SemWarn code,string name)//打印语义警告
{
    warn_num ++;

    printf(WARN);
    printf("SemWarn : %d, name : %s\n", code, name.c_str());

}
