#pragma once

#include "common.h"
#include <map>

#define KWNUM 19

extern const string KWName[];

//extern map<string,KeyWordsType> namemap;

class KeyWords
{
public:
    KeyWords();
    ~KeyWords();
    static int getKWType(string s);
};