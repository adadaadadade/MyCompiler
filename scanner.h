#pragma once

#include "common.h"


class Scanner
{
	//文件指针
	char*fileName;//文件名
	FILE*file;//文件指针
	
	//内部状态
	static const int BUFLEN=4096;//扫描缓冲区长度
	char line[BUFLEN];
	int lineLen;//当前行的长度
	int readPos;//读取的位置
	char lastch;//上一个字符，主要用于判断换行位置	
	
	//读取状态
	int lineNum;//记录行号
	int colNum;//列号
	
	
public:

	//构造与初始化
	Scanner(char* srcName);
	~Scanner();

	//显示字符
	void showChar(char ch);
	
	//扫描
	char nextChar();//基于缓冲区的字符扫描算法,文件扫描接受后自动关闭文件
	
	//外部接口
	char* get_file();//获取文件名
	int get_line();//获取行号
	int get_col();//获取列号
};