#pragma once
#include "common.h"

/*
	中间代码操作符枚举类型
*/
enum IROP
{
	//占位指令,默认值
	IROP_NOP,
	//声明指令
	IROP_DEC, //eg: DEC arg1 => (int/char */[]) arg1
	//函数入口和出口
	IROP_ENTRY,IROP_EXIT,
	//赋值运算
	IROP_ASN,//赋值 eg: AS result,arg1 => result=arg1
	//算数运算
	IROP_ADD,IROP_SUB,IROP_MUL,IROP_DIV,IROP_MOD,//加减乘除模 eg: ADD result,arg1,arg2 => result=arg1 + arg2
	IROP_NEG,//负 eg: NEG result,arg1 => result = -arg1
	//比较运算
	IROP_GTH,IROP_GEQ,IROP_LTH,IROP_LEQ,IROP_EQU,IROP_NEQ,//大小等 eg: GT result,arg1,arg2 => result=arg1 > arg2
	//逻辑运算
	IROP_NOT,//非 eg: NOT result,arg1 => result=!arg1
	IROP_LAND,IROP_LOR,//与或 eg: AND result,arg1,arg2 => result=arg1 && arg2
    //位运算
    IROP_LMO, IROP_RMO,
    IROP_BAND, IROP_BOR, IROP_BXOR,
    IROP_BNEG,
	//数组运算
	//IROP_INDL,//索引作为左值 eg: INDL result,arg1,arg2 => arg1[arg2]=result
	//IROP_INDR,//索引作为右值 eg: INDR result,arg1,arg2 => result=arg1[arg2]
	//指针运算
	IROP_LEA,//取址 eg: LEA result,arg1 => result=&arg1
	IROP_SET,//设置左值 eg: SET result,arg1 => *arg1=result
	IROP_GET,//取右值 eg: GET result,arg1 => result=*arg1
	//跳转
	IROP_JMP,//无条件跳转 eg: JMP result => goto result
	IROP_JT,//真跳转	 eg: JT result,arg1 => if(arg1) goto result
	IROP_JF,//假跳转	 eg: JF result,arg1 => if(!arg1) goto result
	/*IROP_JG,IROP_JGE,IROP_JL,IROP_JLE,IROP_JE,*/IROP_JNE,//跳转 eg:JG result,arg1,arg2 => if(arg1 > arg2) goto result
	//函数调用
	IROP_ARG,//参数传递 eg: ARG arg1 => 传递参数arg1
	IROP_PROC,//调用过程 eg: PROC fun => 调用fun函数,fun()
	IROP_CALL,//调用函数 eg: CALL result,fun => 调用fun函数,返回值result=fun()
	IROP_RET,//直接返回 eg: RET => return
	IROP_RETV//带数据返回 eg:RET arg1 => return arg1
};
/*
IROP_LMO, IROP_RMO,
IROP_BAND, IROP_BOR, IROP_BXOR,
IROP_BNEG,
*/

/*
	四元式类，定义了中间代码的指令的形式
*/
class InterInst
{
private:
    string label; //标签
    IROP op;  //操作符
    //union{
    Var *result;       //运算结果
    InterInst *target; //跳转标号
    //};
    //union{
    Var *arg1; //参数1
    Func *func;  //函数
    //};
    Var *arg2; //参数2

    //bool first;  //是否是首指令
    void init(); //初始化

public:
    //构造
    //一般运算指令
    InterInst(IROP op, Var *rs, Var *arg1, Var *arg2 = NULL);
    //函数调用指令,ENTRY,EXIT             
    InterInst(IROP op, Func *func, Var *rs = NULL);
    //参数进栈指令,NOP                            
    InterInst(IROP op, Var *arg1 = NULL);
    //产生唯一标号                                  
    InterInst(string label);
    //条件跳转指令,return
    InterInst(IROP op, InterInst *tar, Var *arg1 = NULL, Var *arg2 = NULL);
    //替换表达式指令信息，用于常量表达式处理
    void replace(IROP op, Var *rs, Var *arg1, Var *arg2 = NULL);
    //替换跳转指令信息，条件跳转优化
    void replace(IROP op, InterInst *tar, Var *arg1 = NULL, Var *arg2 = NULL); 
    //清理常量内存
    ~InterInst();

    //外部调用接口

    bool is_jcond(); //是否条件转移指令JT,JF,Jcond
    bool is_jmp();   //是否直接转移指令JMP,return
    bool is_lb();    //是否是标签
    bool is_dec();   //是否是声明
    bool is_expr();  //是基本类型表达式运算,可以对指针取值
    bool unknown(); //不确定运算结果影响的运算(指针赋值，函数调用)

    IROP get_op();        //获取操作符
    void call_to_proc();       //替换操作符，用于将CALL转化为PROC
    InterInst *get_target();  //获取跳转指令的目标指令
    Var *get_result();        //获取返回值
    Var *get_arg1();          //获取第一个参数
    Var *get_arg2();          //获取第二个参数
    string get_label();       //获取标签
    Func *get_func();           //获取函数对象
    void set_arg1(Var *arg1); //设置第一个参数
    string to_string();         //输出指令
};

/*
	中间代码
*/
class InterCode
{
    vector<InterInst *> inter_codes;
public:
    //内存管理
    InterCode();
    ~InterCode(); //清除内存

    //管理操作
    void add_inst(InterInst *inst); //添加一条中间代码

    //外部调用接口
    void print();                //输出指令
    vector<InterInst *> &get_code(); //获取中间代码序列
};
