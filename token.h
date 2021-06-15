#pragma once

#include "common.h"
#include "keywords.h"

#define MAXTOKENSIZE 128

enum TokenType
{
	ERR, END,
	//ops
	//BINOPS1
	NOT, BNEG, INC, DEC,	// ! ~ 
	ADD, SUB, MUL, DIV, MOD,	// | + | - | * | / | % 
	LMO, RMO,				// | << | >>
	LTH, GTH, LET, GET, 				// | < | > | <= | >=
	EQU, NEQ,						// | == | != 
	BAND, BXOR, BOR , LAND, LOR,						// | & | ^ | | | && | ||		
	ASN, ASNADD, ASNSUB, ASNMUL, ASNDIV, ASNMOD, ASNLMO, ASNRMO, ASNAND, ASNXOR, ASNOR,//  = | += | -= | *= | /= | %= | <<= | >>=| &= | ^= | |=
	QUE, DOT, ARROW,
	ID,			// [A-Za-z_][A-Za-z0-9_]*
	NUMLIT,		// <decnum> | <hexnum>
	STRLIT,		// "<schar>*"
	CHRLIT,		// ’<cchar>’

	LPAREN,RPAREN,																//()
	LBRACK,RBRACK,																//[]
	LBRACE,RBRACE,																//{}
	COMMA,COLON,SEMICON,													//逗号,冒号,分号
	//keywords
	KW_INT, KW_BOOL, KW_STRING, KW_CHAR ,KW_STRUCT,
	KW_TYPEDEF,
	KW_IF, KW_ELSE, KW_WHILE, KW_FOR, KW_CONTINUE, KW_BREAK, KW_RETURN,
	KW_ASSERT,
	KW_TRUE, KW_FALSE, KW_NULL,
	KW_ALLOC, KW_ALLOC_ARRAY,
	//COM
	COMMIT,DEF
	// DECNUM,		// 0 | [1-9][0-9]*
	// HEXNUM,		// 0[xX][0-9a-fA-F]+
	// SCHAR,		// <nchar> | <esc>
	// CCHAR,		// <nchar> | <esc> | " | \0
	// NCHAR,		// (normal printing character except ")
	// ESC,		// \n | \t | \v | \b | \r | \f | \a| \\ | \? | \’ | \"
};

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