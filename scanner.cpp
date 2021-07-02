#include "scanner.h"
#include "error.h"
#include "args.h"

Scanner::Scanner(char* srcName)
{
    file = fopen(srcName, "r");
    if(!file){
        printf(ERROR"%s 文件打开失败", srcName);
        //Error::errorNum ++;
    }
    fileName = srcName;
    lineLen = 0;
    readPos = -1;
    lastch = 0;
    lineNum = 1;
    colNum = 1;
}


Scanner::~Scanner()
{
}


void Scanner::showChar(char ch)
{

}


char Scanner::nextChar()//基于缓冲区的字符扫描算法,文件扫描接受后自动关闭文件
{
    if(!file)
        return -1;
    if(readPos == lineLen-1)    //缓存区空
    {
        lineLen = fread(line, 1, BUFLEN, file);
        if(lineLen == 0)    //没有更多的数据， 返回一个字符-1
        {
            lineLen = 1;
            line[0] = -1;
        }
        readPos = -1;
    }
    readPos ++;
    char ch = line[readPos];

    if(ch == EOF)
    {
        fclose(file);
        file = NULL;
    }
    if(lastch == '\n')
    {
        lineNum ++;
        colNum = 1;
    }
    else if(ch != '\n')
        colNum ++;
    lastch = ch;
    
    if(Args::show_char)
        printf("%c", ch);
    
    return ch;
}


//外部接口
char* Scanner::get_file()//获取文件名
{
    return fileName;
}


int Scanner::get_line()//获取行号
{
    return lineNum;
}


int Scanner::get_col()//获取列号
{
    return colNum;
}