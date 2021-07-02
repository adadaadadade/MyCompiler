#pragma once

#include "common.h"
#include "keywords.h"

#define MAXTOKENSIZE 128

extern const string tokenName[];

enum NumType
{
	DECNUM,
	HEXNUM
};

class Token
{
public:
	TokenType tag;
	Token(TokenType t);
	virtual string toString();
	virtual ~Token ();
};

class Id : public Token
{
public:
	string name;
	Id (string n);
	virtual string toString();
};

class Numlit : public Token
{
public:
	NumType numType;
	int val;
	Numlit (int v, NumType nT);
	virtual string toString();
};


class Chrlit : public Token
{
public:
	char ch;
	Chrlit (char c);
	virtual string toString();
};

class Strlit : public Token
{
public:
	string str;
	Strlit (string s);
	virtual string toString();
};