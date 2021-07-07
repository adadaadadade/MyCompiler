#include <iostream>
#include "util.h"
#include "scanner.h"
#include "lexer.h"

using namespace std;

// \n | \t | \v | \b | \r | \f | \a| \\ | \? | \’ | \"
bool esc2chr(string& str,char ch)
{
    if(ch == 'n') str.push_back('\n');
    else if(ch == 't') str.push_back('\t');
    else if(ch == 'v') str.push_back('\v');
    else if(ch == 'b') str.push_back('\b');
    else if(ch == 'r') str.push_back('\r');
    else if(ch == 'f') str.push_back('\f');
    else if(ch == 'a') str.push_back('\a');
    else if(ch == '\\') str.push_back('\\');
    else if(ch == '?') str.push_back('\?');
    else if(ch == '\'') str.push_back('\'');
    else if(ch == '"') str.push_back('\"');
    else return false;

    return true;
}

void outputScanner(Scanner scanner){
    char ch;
    while((ch = scanner.nextChar()) != EOF)
    {
        printf("<'%c',%d ,%d>", ch, scanner.get_line(), scanner.get_col());
        printf("\n");
    }
    //KeyWords();
    //printf("%d\n",(KeyWords::getKWType("int")));
    //printf("%d\n",(KeyWords::getKWType("NULL")));
    //printf("%d\n",(KeyWords::getKWType("alloc_array")));
    //printf("%d\n",(KeyWords::getKWType("sdasdas")));
}

void outputTokens(Lexer lexer){
    Token* token = lexer.nextToken();
    while(token)
    {
        cout << token->to_string() << " ";
        if(token->tag == END)
            break;
        token = lexer.nextToken();
    }
    cout << endl;
}

