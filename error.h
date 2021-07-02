#pragma once
#include "common.h"

#define FATAL   "<fatal> : "
#define ERROR   "<error> : "
#define WARN	"<warn> : "


class Error
{
    static Scanner *scanner;
public:

    /*
	词法错误码
    */
    enum LexError
    {
        STR_NO_R_QUTION,		//字符串没有右引号
        NUM_BIN_TYPE,				//2进制数没有实体数据
        NUM_HEX_TYPE,				//16进制数没有实体数据
        CHAR_NO_R_QUTION,		//字符没有右引号
        CHAR_NO_DATA,				//字符没有数据
        OR_NO_PAIR,					//||只有一个|
        COMMENT_NO_END,			//多行注释没有正常结束
        TOKEN_NO_EXIST			//不存在的词法记号
    };

    /*
        语法错误码
    */
    enum SynError
    {
        TYPE_LOST,					//类型
        TYPE_WRONG,					
        ID_LOST,						//标志符
        ID_WRONG,						
        NUM_LOST,						//数组长度
        NUM_WRONG,
        LITERAL_LOST,				//常量
        LITERAL_WRONG,
        COMMA_LOST,					//逗号
        COMMA_WRONG,
        SEMICON_LOST,				//分号
        SEMICON_WRONG,
        ASSIGN_LOST,				//=
        ASSIGN_WRONG,
        COLON_LOST,					//冒号
        COLON_WRONG,
        WHILE_LOST,					//while
        WHILE_WRONG,
        LPAREN_LOST,				//(
        LPAREN_WRONG,
        RPAREN_LOST,				//)
        RPAREN_WRONG,
        LBRACK_LOST,				//[
        LBRACK_WRONG,
        RBRACK_LOST,				//]
        RBRACK_WRONG,
        LBRACE_LOST,				//{
        LBRACE_WRONG,
        RBRACE_LOST,				//}
        RBRACE_WRONG
    };

    /*
        语义错误码
    */
    enum SemError
    {
        STRUCT_UN_DEF,
        VAR_RE_DEF,					//变量重定义
        FUN_RE_DEF,					//函数重定义
        STRUCT_RE_DEF,
        VAR_UN_DEC,					//变量未声明
        FUN_UN_DEC,					//函数未声明
        FUN_DEC_ERR,				//函数声明与定义不匹配
        FUN_CALL_ERR,				//行参实参不匹配
        DEC_INIT_DENY,			//声明不允许初始化
        ARRAY_LEN_INVALID,	//数组长度无效
        VAR_INIT_ERR,				//变量初始化类型错误
        GLB_INIT_ERR,				//全局变量初始化值不是常量
        VOID_VAR,						//void变量
        EXPR_NOT_LEFT_VAL,	//无效的左值表达式
        ASSIGN_TYPE_ERR,		//赋值类型不匹配
        EXPR_IS_BASE,				//表达式不能是基本类型
        EXPR_NOT_BASE,			//表达式不是基本类型
        ARR_TYPE_ERR,				//数组运算类型错误
        EXPR_IS_VOID,				//表达式不能是VOID类型
        BREAK_ERR,					//break不在循环或switch-case中
        CONTINUE_ERR,				//continue不在循环中
        RETURN_ERR					//return语句和函数返回值类型不匹配
    };

    /*
        语义警告码
    */
    enum SemWarn
    {
        FUN_DEC_CONFLICT,		//函数参数列表类型冲突
        FUN_RET_CONFLICT		//函数返回值类型冲突
    };

    Error(Scanner *sc);
    ~Error();
    
    static int error_num;//错误个数
	static int warn_num;	//警告个数

	//外界接口
	static int get_error_num();
	static int get_warn_num();
	
	//错误接口
	static void lex_error(LexError code);//打印词法错误
	static void syn_error(SynError code,Token*t);//打印语法错误
	static void sem_error(SemError code,string name="");//打印语义错误
    static void fatal();
	static void sem_Warn(SemWarn code,string name="");//打印语义警告
};
