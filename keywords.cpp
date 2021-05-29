#include "keywords.h"
#include "token.h"
map<string,TokenType> namemap;

KeyWords::KeyWords()
{
    TokenType kwt;
    for(kwt = KW_INT; kwt < KW_INT + KWNUM; kwt =  TokenType(kwt + 1))
    {
        namemap[tokenName[kwt]] = kwt;
    }
}

KeyWords::~KeyWords()
{
    
}

int KeyWords::getKWType(string s)
{
    if(namemap.count(s))
    {
        return namemap[s];
    }
    else
        return -1;
}
