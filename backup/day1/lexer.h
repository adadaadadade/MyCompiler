#pragma once

#include "common.h"
#include "scanner.h"
#include "keywords.h"
#include "token.h"

class Lexer
{
	static KeyWords keywords;//关键字列表
	Scanner &scanner;//扫描器
	char ch;//读入的字符
	char ch2;
	char ch3;

	Token *token;//记录扫描的词法记号
	
public:
	Lexer (Scanner& sc);
	~Lexer();

	bool scan(char need = 0);//扫描与匹配
	Token* nextToken();//有限自动机匹配，词法记号解析
};

