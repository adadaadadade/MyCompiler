#include "intercode.h"
#include "symbol.h"
#define GETSTR(x) ((x != NULL) ? (x->to_string() + "\t") : "-\t")

void InterInst::init()
{
    label = "";
    op = IROP_NOP;
    result = NULL;
    target = NULL;
    arg1 = NULL;
    arg2 = NULL;
    func = NULL;
}

InterInst::InterInst(IROP op, Var *rs, Var *arg1, Var *arg2) //一般运算指令
{
    init();
    this->op = op;
    this->result = rs;
    this->arg1 = arg1;
    this->arg2 = arg2;
}

InterInst::InterInst(IROP op, Func *fun, Var *rs) //函数调用指令,ENTRY,EXIT
{
    init();
    this->op = op;
    this->func = fun;
    this->result = rs;
}

InterInst::InterInst(IROP op, Var *arg1) //参数进栈指令,NOP
{
    init();
    this->op = op;
    this->arg1 = arg1;
}

InterInst::InterInst(string label) //产生唯一标号
{
    init();
    this->label = label;
}

InterInst::InterInst(IROP op, InterInst *tar, Var *arg1, Var *arg2) //条件跳转指令,return
{
    init();
    this->op = op;
    this->target = tar;
    this->arg1 = arg1;
    this->arg2 = arg2;
}

void InterInst::replace(IROP op, Var *rs, Var *arg1, Var *arg2) //替换表达式指令信息，用于常量表达式处理
{
    init();
    this->op = op;
    this->result = rs;
    this->arg1 = arg1;
    this->arg2 = arg2;
}

void InterInst::replace(IROP op, InterInst *tar, Var *arg1, Var *arg2) //替换跳转指令信息，条件跳转优化
{
    init();
    this->op = op;
    this->target = tar;
    this->arg1 = arg1;
    this->arg2 = arg2;
}

InterInst::~InterInst() //清理常量内存
{
}

/*
	是否条件转移指令JT,JF,Jcond
*/
bool InterInst::is_jcond()
{
    return op >= IROP_JT && op <= IROP_JNE;
}

/*
	是否直接转移指令JMP,return
*/
bool InterInst::is_jmp()
{
    return op == IROP_JMP || op == IROP_RET || op == IROP_RETV;
}

/*
	是否是标签
*/
bool InterInst::is_lb()
{
    return label != "";
}

/*
	是基本类型表达式运算,可以对指针取值
*/
bool InterInst::is_expr()
{
    return (op >= IROP_ASN && op <= IROP_LOR || op == IROP_GET); //&&result->isBase();
}

/*
	不确定运算结果影响的运算(指针赋值，函数调用)
*/
bool InterInst::unknown()
{
    return op == IROP_SET || op == IROP_PROC || op == IROP_CALL;
}

IROP InterInst::get_op() //获取操作符
{
    return op;
}

void InterInst::call_to_proc() //替换操作符，用于将CALL转化为PROC
{
}

InterInst *InterInst::get_target() //获取跳转指令的目标指令
{
    return target;
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
        str = label + ":";
        return str;
    }
    switch (op)
    {
    //case IROP_NOP:printf("nop");break;
    case IROP_DEC:
        str = "\tDEC\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);
        break;
    case IROP_ENTRY:
        str = "\tENTRY\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_EXIT:
        str = "\tEXIT\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_ASN:
        str = "\tAS\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_ADD:
        str = "\tADD\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_SUB:
        str = "\tSUB\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_MUL:
        str = "\tMUL\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_DIV:
        str = "\tDIV\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_MOD:
        str = "\tMOD\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_BNEG:
        str = "\tNEG\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_GTH:
        str = "\tGTH\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_GEQ:
        str = "\tGEQ\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_LTH:
        str = "\tLTH\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_LEQ:
        str = "\tLEQ\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_EQU:
        str = "\tEQU\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_NEQ:
        str = "\tNE\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_NOT:
        str = "\tNOT\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_LAND:
        str = "\tAND\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_LOR:
        str = "\tOR\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_JMP:
        str = "\tJMP\t" + target->label + "\t" + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_JT:
        str = "\tJT\t" + target->label + "\t" + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_JF:
        str = "\tJF\t" + target->label + "\t" + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_JNE:
        str = "\tJNE\t" + target->label + "\t" + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_ARG:
        str = "\tARG\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_PROC:
        str = "\tPROC\t" + GETSTR(result) + func->get_name() + "\t" + GETSTR(arg2);

        break;
    case IROP_CALL:
        str = "\tCALL\t" + GETSTR(result) + func->get_name() + "\t" + GETSTR(arg2);

        break;
    case IROP_RET:
        str = "\tRET\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_RETV:
        str = "\tRETV\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_LEA:
        str = "\tLEA\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_SET:
        str = "\tSET\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    case IROP_GET:
        str = "\tGET\t" + GETSTR(result) + GETSTR(arg1) + GETSTR(arg2);

        break;
    }
    return str;
}

InterCode::InterCode()
{
}

InterCode::~InterCode()
{
}

void InterCode::add_inst(InterInst *inst)
{
    inter_codes.push_back(inst);
}

void InterCode::print()
{
    for (int i = 0; i < inter_codes.size(); i++)
    {
        printf("%s\n", inter_codes[i]->to_string().c_str());
    }
}

vector<InterInst *> &InterCode::get_code()
{
    return inter_codes;
}
