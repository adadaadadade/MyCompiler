#include "keywords.h"

const string KWName[] =
{
    "int", "bool", "string", "char", "struct",
    "typedef",
    "if","else", "while", "for", "continue", "break", "return",
    "assert",
    "true", "false", "NULL",
    "alloc", "alloc_array"
};

map<string,KeyWordsType> namemap;

KeyWords::KeyWords()
{
    KeyWordsType kwt;
    for(kwt = KW_INT; kwt < KWNUM; kwt =  KeyWordsType(kwt + 1))
    {
        namemap[KWName[kwt]] = kwt;
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
