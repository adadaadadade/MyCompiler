#pragma once

#include "common.h"
#include"scanner.h"
#include"lexer.h"

#define isspace(ch) (ch==' '||ch=='\n'||ch=='\t')
#define isdigitN0(ch) ((ch >= '1') && (ch <= '9'))
#define isdigit(ch) (isdigitN0(ch) || (ch == '0'))
#define ishexnum(ch) (isdigit(ch) || ((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')))
#define isalpha(ch) ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
#define isalphaA_(ch) (isalpha(ch) || (ch == '_'))
#define isXx(ch) (ch == 'X' || ch == 'x')

#define isschar(ch) isnchar(ch)
#define iscchar(ch) (isnchar(ch) || ch == '"' || ch =='\0')
#define isnchar(ch) (ch != '"')
#define isescchar(ch) ((ch == 'n') || (ch == 't') || (ch == 'v') || (ch == 'b') || (ch == 'r') || (ch == 'f') || (ch == 'a') || (ch == '\\') || (ch == '?') || (ch == '\'') || (ch == '"')) 


bool esc2chr(string& str,char ch);

void outputScanner(Scanner scanner);
void outputTokens(Lexer lexer);