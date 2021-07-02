#include "intercode.h"
#include "symbol.h"
#define GETSTR(x) ((x != NULL) ? (x->get_str() + "\t") : "-\t")

InterInst::InterInst(IROP op, Var *rs, Var *arg1, Var *arg2)                  //一般运算指令
{
    this->op = op;
    this->result = rs;
    this->arg1 = arg1;
    this->arg2 = arg2;
}

InterInst::InterInst(IROP op, Func *fun, Var *rs)                              //函数调用指令,ENTRY,EXIT
{
    this->op = op;
    this->func = fun;
    this->result = rs;
}


InterInst::InterInst(IROP op, Var *arg1)                                      //参数进栈指令,NOP
{
    this->op = op;
    this->arg1 = arg1;
}


InterInst::InterInst(string label)                                                                   //产生唯一标号
{
    this->label = label;
}


InterInst::InterInst(IROP op, InterInst *tar, Var *arg1, Var *arg2)    //条件跳转指令,return
{
    this->op = op;
    this->target = tar;
    this->arg1 = arg1;
    this->arg2 = arg2;
}


void InterInst::replace(IROP op, Var *rs, Var *arg1, Var *arg2)               //替换表达式指令信息，用于常量表达式处理
{
    this->op = op;
    this->result = rs;
    this->arg1 = arg1;
    this->arg2 = arg2;
}


void InterInst::replace(IROP op, InterInst *tar, Var *arg1, Var *arg2) //替换跳转指令信息，条件跳转优化
{
    this->op = op;
    this->target = tar;
    this->arg1 = arg1;
    this->arg2 = arg2;
}


InterInst::~InterInst()                                                                  //清理常量内存
{

}


/*
	是否条件转移指令JT,JF,Jcond
*/
bool InterInst::is_jcond()
{
	return op>=IROP_JT&&op<=IROP_JNE;
}

/*
	是否直接转移指令JMP,return
*/
bool InterInst::is_jmp()
{
	return op==IROP_JMP||op==IROP_RET||op==IROP_RETV;
}

/*
	是否是标签
*/
bool InterInst::is_lb()
{
	return label!="";
}

/*
	是基本类型表达式运算,可以对指针取值
*/
bool InterInst::is_expr()
{
	return (op>=IROP_AS&&op<=IROP_OR||op==IROP_GET);//&&result->isBase();
}

/*
	不确定运算结果影响的运算(指针赋值，函数调用)
*/
bool InterInst::unknown()
{
	return op==IROP_SET||op==IROP_PROC||op==IROP_CALL;
}

IROP InterInst::get_op() //获取操作符
{
    return op;
}

void InterInst::callToProc() //替换操作符，用于将CALL转化为PROC
{
}

InterInst *InterInst::getTarget() //获取跳转指令的目标指令
{
}

Var *InterInst::get_result() //获取返回值
{
    return result;
}

Var *InterInst::get_arg1() //获取第一个参数
{
    return arg1;
}

Var *InterInst::get_arg2() //获取第二个参数
{
    return arg2;
}

string InterInst::get_label() //获取标签
{
    return label;
}

Func *InterInst::get_func() //获取函数对象
{
    return func;
}

void InterInst::set_arg1(Var *arg1) //设置第一个参数
{
    this->arg1 = arg1;
}

string InterInst::to_string() //输出指令
{
    string str;
    if (label != "")
    {
        str = label;
        return str;
    }
    switch (op)
    {
    //case IROP_NOP:printf("nop");break;
    case IROP_DEC:
        str = "DEC\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);
        break;
    case IROP_ENTRY:
        str = "ENTRY\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_EXIT:
        str = "EXIT\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_AS:
        str = "AS\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_ADD:
        str = "ADD\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_SUB:
        str = "SUB\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_MUL:
        str = "MUL\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_DIV:
        str = "DIV\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_MOD:
        str = "MOD\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_NEG:
        str = "NEG\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_GT:
        str = "GT\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_GE:
        str = "GE\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_LT:
        str = "LT\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_LE:
        str = "LE\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_EQU:
        str = "EQU\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_NE:
        str = "NE\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_NOT:
        str = "NOT\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_AND:
        str = "AND\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_OR:
        str = "OR\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_JMP:
        str = "JMP\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_JT:
        str = "JT\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_JF:
        str = "JF\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    // case IROP_JG:printf("if( ");arg1->value();printf(" > ");arg2->value();printf(" )goto %s",
    // 	target->label.c_str());break;
    // case IROP_JGE:printf("if( ");arg1->value();printf(" >= ");arg2->value();printf(" )goto %s",
    // 	target->label.c_str());break;
    // case IROP_JL:printf("if( ");arg1->value();printf(" < ");arg2->value();printf(" )goto %s",
    // 	target->label.c_str());break;
    // case IROP_JLE:printf("if( ");arg1->value();printf(" <= ");arg2->value();printf(" )goto %s",
    // 	target->label.c_str());break;
    // case IROP_JE:printf("if( ");arg1->value();printf(" == ");arg2->value();printf(" )goto %s",
    // 	target->label.c_str());break;
    case IROP_JNE:
        str = "JNE\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_ARG:
        str = "ARG\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_PROC:
        str = "PROC\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_CALL:
        str = "CALL\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_RET:
        str = "RET\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_RETV:
        str = "RETV\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_LEA:
        str = "LEA\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_SET:
        str = "SET\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_GET:
        str = "GET\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    }
    return str;
}

InterCode::InterCode()
{
}

InterCode::~InterCode() //清除内存
{
}

//管理操作
void InterCode::add_inst(InterInst *inst) //添加一条中间代码
{
    inter_codes.push_back(inst);
}

//外部调用接口
void InterCode::print() //输出指令
{
    for (int i = 0; i < inter_codes.size(); i++)
    {
        printf("%s\n", inter_codes[i]->to_string().c_str());
    }
}
