#include "selector.h"
#include "intercode.h"
#include "symbol.h"

Selector::Selector(vector<InterInst *> &irCode, ILoc &ilocCode)
    : ir(irCode), iloc(ilocCode)
{
}

/*
	指令选择
*/
void Selector::select()
{
    for (unsigned int i = 0; i < ir.size(); i++)
    {
        translate(ir[i]);
    }
}

/*
	翻译
*/
void Selector::translate(InterInst *inst)
{
    string label = inst->get_label();
    if (label != "")
    { //标签打印
        iloc.label(label);
        return;
    }
    IROP op = inst->get_op(); //操作符
    if (op == IROP_ENTRY)
    {                              //函数入口
        Func *func = inst->get_func(); //函数
        cur_func = inst->get_func();
        iloc.comment("函数入口");
        iloc.inst("mov", "ip", "sp");
        if (func->is_relocated())
            iloc.inst("stmfd", "sp!", "{r0-r7,fp,ip,lr,pc}"); //保护现场
        else
            iloc.inst("stmfd", "sp!", "{fp,ip,lr,pc}"); //不用保护现场
        iloc.inst("sub", "fp", "ip", "#4");
        iloc.comment("开辟栈帧");
        iloc.alloc_stack(func, "r8"); //为fun分配栈帧
        iloc.comment("加载参数变量到寄存器");
        iloc.ldr_args(func); //加载参数到对应的寄存器
        iloc.comment("函数内代码");
    }
    else if (op == IROP_EXIT)
    { //函数出口
        iloc.comment("函数出口");
        if (inst->get_func()->is_relocated())
            iloc.inst("ldmea", "fp", "{r0-r7,fp,sp,pc}"); //恢复现场
        else
            iloc.inst("ldmea", "fp", "{fp,sp,pc}"); //不用恢复现场
    }
    else if (op == IROP_DEC)
    {                                               //处理声明的初始化
        iloc.init_var(inst->get_arg1(), "r8", "r9"); //初始值在r8，可能用到r9
    }
    else if (op == IROP_LEA)
    {                                                //取地址
        iloc.lea_var("r8", inst->get_arg1());         //&arg1 -> r8
        iloc.str_var("r8", inst->get_result(), "r9"); //r8 -> rs 可能用到r9
    }
    else if (op == IROP_SET)
    {                                          //设置指针值
        iloc.ldr_var("r8", inst->get_result()); //rs -> r8
        iloc.ldr_var("r9", inst->get_arg1());   //arg1 -> r9
        
        iloc.str_base("r8", "r9", 0, "r9", inst->get_arg1()->is_string()); //rs -> *arg1
        iloc.nop();                                                       //占位
    }
    else if (op == IROP_JMP || op == IROP_JT || op == IROP_JF || op == IROP_JNE || op == IROP_RET || op == IROP_RETV)
    {                                               //跳转,函数返回
        string des = inst->get_target()->get_label(); //目标地址
        if(cur_func->get_name() == "main" && op == IROP_RETV)
            iloc.ldr_var("r0", inst->get_arg1());        //arg1 -> r0
        else
            iloc.ldr_var("r8", inst->get_arg1());        //arg1 -> r8
        iloc.ldr_var("r9", inst->get_arg2());        //arg2 -> r9 JNE需要
        switch (op)
        {
        case IROP_JMP:
            iloc.inst("b", des);
            break;
        case IROP_JT:
            iloc.inst("cmp", "r8", "#0");
            iloc.inst("bne", des);
            break;
        case IROP_JF:
            iloc.inst("cmp", "r8", "#0");
            iloc.inst("beq", des);
            break;
        case IROP_RET:
            iloc.inst("b", des);
            break;
        case IROP_RETV:
            iloc.inst("b", des);
            break;
        case IROP_JNE:
            iloc.cmp("ne", "eq", "r8", "r8", "r9"); //r8!=r9 -> r8
            iloc.inst("cmp", "r8", "#0");           //r8!=0?
            iloc.inst("bne", des);
            break;
        }
    }
    else if (op == IROP_ARG)
    {                                        //参数入栈,直接push arg1 !
        iloc.ldr_var("r8", inst->get_arg1()); //arg1 -> r8
        iloc.inst("stmfd", "sp!", "{r8}");   //将r8压栈
                                             //iloc.str_base("r8","fp",inst->offset,"r9");//r8 -> [fp,#offset]
    }
    else if (op == IROP_CALL)
    {                                                //调用返回值函数
        iloc.call_func(inst->get_func(), "r9");         //fun() -> r8,恢复栈帧可能用到r9
        iloc.str_var("r8", inst->get_result(), "r9"); //r8 -> rs 可能用到r9
    }
    else if (op == IROP_PROC)
    {                                        //调用无返回值函数
        iloc.call_func(inst->get_func(), "r9"); //fun()
    }
    else
    { //数值值表达式运算
        Var *rs = inst->get_result();
        Var *arg1 = inst->get_arg1();
        Var *arg2 = inst->get_arg2();
        iloc.ldr_var("r8", arg1); //arg1 -> r8
        iloc.ldr_var("r9", arg2); //arg2 -> r9
        switch (op)
        {
        //赋值
        case IROP_ASN:
            break;
        //算数
        case IROP_ADD:
            iloc.inst("add", "r8", "r8", "r9");
            break;
        case IROP_SUB:
            iloc.inst("sub", "r8", "r8", "r9");
            break;
        case IROP_MUL:
            iloc.inst("mul", "r10", "r8", "r9");
            iloc.inst("mov", "r8", "r10");
            break;
        case IROP_DIV:
            iloc.call_lib("__divsi3", "r8", "r8", "r9");
            break;
        case IROP_MOD:
            iloc.call_lib("__modsi3", "r8", "r8", "r9");
            break;
        case IROP_NEG:
            iloc.inst("rsb", "r8", "r8", "#0");
            break; //逆向减法r8=0-r8
        //位逻辑运算
        case IROP_BAND:
            iloc.inst("and", "r8", "r8", "r9");
            break;
        case IROP_BOR:
            iloc.inst("orr", "r8", "r8", "r9");
            break;
        case IROP_BXOR:
            iloc.inst("eor", "r8", "r8", "r9");
            break;
        case IROP_BNEG:
            iloc.inst("mvn", "r8", "r8");
            //取反后mov
            break;
        //移位运算
        case IROP_LMO:
            iloc.inst("lsl", "r8", "r8", "r9");
            break;
        case IROP_RMO:
            iloc.inst("lsr", "r8", "r8", "r9");
            break;
        //比较
        case IROP_GTH:
            iloc.cmp("gt", "le", "r8", "r8", "r9");
            break;
        case IROP_GEQ:
            iloc.cmp("ge", "lt", "r8", "r8", "r9");
            break;
        case IROP_LTH:
            iloc.cmp("lt", "ge", "r8", "r8", "r9");
            break;
        case IROP_LEQ:
            iloc.cmp("le", "gt", "r8", "r8", "r9");
            break;
        case IROP_EQU:
            iloc.cmp("eq", "ne", "r8", "r8", "r9");
            break;
        case IROP_NEQ:
            iloc.cmp("ne", "eq", "r8", "r8", "r9");
            break;
        //逻辑
        case IROP_LAND:
            iloc.logic_and("r8", "r8", "r9");
            break;
        case IROP_LOR:
            iloc.logic_or("r8", "r8", "r9");
            break;
        case IROP_NOT:
            iloc.logic_not("r8", "r8");
            break;
        //指针,此处需要将类型转换考虑进去
        case IROP_GET:
            iloc.ldr_base("r8", "r8", 0, rs->is_half());
            break; //a=*p
        }
        iloc.str_var("r8", rs, "r9"); //r8 -> rs 可能用到r9
    }
}