#pragma once

#include "common.h"
#include <map>

#define KWNUM 19

enum KeyWordsType
{
    KW_INT, KW_BOOL, KW_STRING, KW_CHAR ,KW_STRUCT,
	KW_TYPEDEF,
	KW_IF, KW_ELSE, KW_WHILE, KW_FOR, KW_CONTINUE, KW_BREAK, KW_RETURN,
	KW_ASSERT,
	KW_TRUE, KW_FALSE, KW_NULL,
	KW_ALLOC, KW_ALLOC_ARRAY
};
extern const string KWName[];

//extern map<string,KeyWordsType> namemap;

class KeyWords
{
public:
    KeyWords();
    ~KeyWords();
    static int getKWType(string s);
};