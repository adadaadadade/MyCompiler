#include "token.h"
#include <sstream>

const string tokenName[] = 
{
	"ERR", "END",

	"!", "~",
	"+", "-", "*", "/", "%", 
	"<<", ">>",
	"<", ">", "==", "!=",
	"&", "^", "|", "&&", "||",
	"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=",
	"ID", "NUMLIT", "STRLIT", "CHRLIT",
	"(",")",//()
	"[","]",//[]
	"{","}",//{}
	",",":",";",//逗号,冒号,分号
	"KW",
	"ONECOM","MULCOM","DEF"
};

Token::Token(TokenType t):tag(t)
{
}

string Token::toString()
{
	return string("<") + tokenName[tag] + string (">");
}

Token::~Token()
{
}


Id::Id(string n):Token(ID),name(n)
{
}

string Id::toString()
{
	return Token::toString();
}


Kw::Kw(KeyWordsType k):Token(KW),keyWordsType(k)
{
}

string Kw::toString()
{
	return string("<") + tokenName[tag] + string(":") + KWName[keyWordsType] + string (">");
}


Numlit::Numlit(int v, NumType nT):Token(NUMLIT), val(v), numType(nT)
{
}

string Numlit::toString()
{
    stringstream ss;
	ss << val;
	return string("<") + tokenName[tag] + string(":") + ss.str() + string (">");
}


Chrlit::Chrlit(char c):Token(CHRLIT), ch(c)
{
}

string Chrlit::toString()
{
    stringstream ss;
	ss << ch;
	return string("<") + tokenName[tag] + string(":") + ss.str() + string (">");
}

Strlit::Strlit(string s):Token(STRLIT), str(s)
{
}

string Strlit::toString()
{
	return string("<") + tokenName[tag] + string(":") + str + string (">");
}