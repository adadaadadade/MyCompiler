#pragma once
#include "common.h"

struct Arm32
{
    bool dead;       //标识代码是否无效
    string opcode;   //操作码
    string result;   //结果
    string arg1;     //操作数1
    string arg2;     //操作数2
    string addition; //附加信息
    Arm32(string op, string rs = "", string a1 = "", string a2 = "", string add = "");
    void replace(string op, string rs = "", string a1 = "", string a2 = "", string add = "");
    void set_dead(); //设置无效
    string output(); //输出函数
};

/*
	底层汇编序列
*/
class ILoc
{
    list<Arm32 *> code; //代码序列
    
    string to_string(int num, int flag = true); //#123
    /*
		ldr str
	*/
    void ldr_imm(string rs_reg, int num);    //加载立即数 ldr r0,=#100
    void ldr_lb(string rs_reg, string name); //加载符号值 ldr r0,=g ldr r0,[r0]
    void lea_stack(string rs_reg, int off);  //加载栈内变量地址
public:
    //基址寻址 ldr r0,[fp,#100]
    void ldr_base(string rs_reg, string base, int disp, bool isChar);
    //基址寻址 str r0,[fp,#100]
    void str_base(string src_reg, string base, int disp, string tmp_reg, bool isChar);
    //label 注释 一般指令
    void label(string name);                                   //产生标签
    void comment(string str);                                  //产生注释
    void inst(string op, string rs);                           //0个操作数
    void inst(string op, string rs, string arg1);              //一个操作数
    void inst(string op, string rs, string arg1, string arg2); //两个操作数s
	//变量操作
    void init_var(Var *var, string init_reg, string tmp_reg); //变量的初始化
    void ldr_var(string rs_reg, Var *var);                    //加载变量到寄存器
    void lea_var(string rs_reg, Var *var);                    //加载变量地址到寄存器
    void str_var(string src_reg, Var *var, string addrReg);   //保存寄存器到变量
	//函数调用
    void call_lib(string fun, string rs_reg, string reg1, string reg2); //调用库函数
    void call_func(Func *fun, string tmp_reg);                          //调用函数funs
	//栈处理
    void alloc_stack(Func *fun, string tmp_reg); //分配栈帧
    void ldr_args(Func *fun);                    //加载函数的参数到寄存器
    //逻辑运算
    void logic_and(string rs_reg, string reg1, string reg2);                      //逻辑与
    void logic_or(string rs_reg, string reg1, string reg2);                       //逻辑或
    void logic_not(string rs_reg, string reg1);                                   //逻辑非
    void cmp(string cmp, string cmpnot, string rs_reg, string reg1, string reg2); //比较
	//占位指令
    void nop(); //占位

    void output(FILE *file);   //输出汇编
    list<Arm32 *> &get_code(); //获取代码
    ~ILoc();
};