#pragma once

#include <stdio.h>
#include <vector>
#include <list>
#include <string>
#include <ctype.h>
#include <iostream>
using namespace std;

enum TokenType
{
	ERR, END,
	//ops
	//BINOPS1
	NOT, BNEG, INC, DEC,	// ! ~ 
	ADD, SUB, MUL, DIV, MOD,	// | + | - | * | / | % 
	LMO, RMO,				// | << | >>
	LTH, GTH, LEQ, GEQ, 				// | < | > | <= | >=
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


class Args;
class Token;
class Scanner;
class Error;
class Lexer;
class Parser;

class Prog;
class Gdefn;

class Var;
class Scope;
class Func;
class Struct;
class Typedef;

class Symtab;

class InterCode;
class InterInst;

class GenIR;

class Args;