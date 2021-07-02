#include "token.h"
#include <sstream>

const string tokenName[] =
	{
		"ERR", "END",
		"!", "~", "++", "--",
		"+", "-", "*", "/", "%",
		"<<", ">>",
		"<", ">", "<=", ">=",
		"==", "!=",
		"&", "^", "|", "&&", "||",
		"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=",
		"?", ".", "->",
		"ID", "NUMLIT", "STRLIT", "CHRLIT",
		"(", ")",	   //()
		"[", "]",	   //[]
		"{", "}",	   //{}
		",", ":", ";", //逗号,冒号,分号
		"int", "bool", "string", "char", "struct",
		"typedef",
		"if","else", "while", "for", "continue", "break", "return",
		"assert",
		"true", "false", "NULL",
		"alloc", "alloc_array",
		"COMMIT", "DEF"
	};

Token::Token(TokenType t) : tag(t)
{
}

string Token::toString()
{
	return string("<") + tokenName[tag] + string(">\n");
}

Token::~Token()
{
}

Id::Id(string n) : Token(ID), name(n)
{
}

string Id::toString()
{
	return string("<") + tokenName[tag] + string(":") + name + string(">\n");
}

Numlit::Numlit(int v, NumType nT) : Token(NUMLIT), val(v), numType(nT)
{
}

string Numlit::toString()
{
	stringstream ss;
	ss << val;
	return string("<") + tokenName[tag] + string(":") + ss.str() + string(">\n");
}

Chrlit::Chrlit(char c) : Token(CHRLIT), ch(c)
{
}

string Chrlit::toString()
{
	stringstream ss;
	ss << ch;
	return string("<") + tokenName[tag] + string(":") + ss.str() + string(">\n");
}

Strlit::Strlit(string s) : Token(STRLIT), str(s)
{
}

string Strlit::toString()
{
	return string("<") + tokenName[tag] + string(":") + str + string(">\n");
}