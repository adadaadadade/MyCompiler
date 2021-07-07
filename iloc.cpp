#include "iloc.h"
#include "symbol.h"
#include "platform.h"
#include <sstream>

Arm32::Arm32(string op, string rs, string a1, string a2, string add)
    : opcode(op), result(rs), arg1(a1), arg2(a2), addition(add), dead(false)
{
}

void Arm32::replace(string op, string rs, string a1, string a2, string add)
{
    opcode = op;
    result = rs;
    arg1 = a1;
    arg2 = a2;
    addition = add;
}

void Arm32::set_dead()
{
    dead = true;
}

string Arm32::output()
{
    if (dead)
        return "";
    if (opcode == "")
        return ""; //占位指令
    string ret;
    //为跳转标签
    if (opcode[0] == '.')
        ret = opcode + result;
    else
        ret = "\t" + opcode + " " + result;
    if (arg1 != "")
        ret += "," + arg1;
    if (arg2 != "")
        ret += "," + arg2;
    if (addition != "")
        ret += "," + addition;
    return ret;
}

#define emit(args...) code.push_back(new Arm32(args))

string ILoc::to_string(int num, int flag)
{
    string ret = "";
    if (flag)
        ret = "#";
    stringstream ss;
    ss << num;
    ret += ss.str(); //#123 =123
    return ret;
}

void ILoc::label(string name)
{
    emit(name, ":"); //.L1:
}

void ILoc::comment(string str)
{
    emit("#", str); //#hello world
}

void ILoc::inst(string op, string rs)
{
    emit(op, rs);
}

void ILoc::inst(string op, string rs, string arg1)
{
    emit(op, rs, arg1);
}

void ILoc::inst(string op, string rs, string arg1, string arg2)
{
    emit(op, rs, arg1, arg2);
}

void ILoc::ldr_imm(string rs_reg, int num)
{
    if (Arm32Plat::const_expr(num))
        emit("mov", rs_reg, to_string(num)); //mov r8,#12
    else
        ldr_lb(rs_reg, to_string(num, false)); //ldr r8,=0xfff0
}

void ILoc::ldr_lb(string rs_reg, string name)
{
    emit("ldr", rs_reg, "=" + name); //ldr r8,=.L1
}

void ILoc::ldr_base(string rs_reg, string base, int disp, bool is_half)
{
    if (Arm32Plat::is_disp(disp))
    { //有效的偏移常量
        if (disp)
            base += "," + to_string(disp); //[fp,#-16] [fp]
    }
    else
    {
        ldr_imm(rs_reg, disp); //ldr r8,=-4096
        base += "," + rs_reg;  //fp,r8
    }
    base = "[" + base + "]";                      //内存寻址
    emit(is_half ? "ldrb" : "ldr", rs_reg, base); //ldr r8,[fp,#-16] ldr r8,[fp,r8]
}

void ILoc::str_base(string src_reg, string base, int disp, string tmp_reg, bool is_half)
{
    if (Arm32Plat::is_disp(disp))
    { //有效的偏移常量
        if (disp)
            base += "," + to_string(disp); //[fp,#-16] [fp]
    }
    else
    {
        ldr_imm(tmp_reg, disp); //ldr r9,=-4096
        base += "," + tmp_reg;  //fp,r9
    }
    base = "[" + base + "]";                       //内存寻址
    emit(is_half ? "strb" : "str", src_reg, base); //str r8,[fp,#-16] str r8,[fp,r9]
}

void ILoc::init_var(Var *var, string init_reg, string tmp_reg)
{
    /*
    if (!var->unInit())
    {                                           //
        if (var->is_base())                      //基本类型初始化 100 'a'
            ldr_imm(init_reg, var->getVal());   //ldr r8,#100
        else                                    //字符指针初始化
            ldr_lb(init_reg, var->getPtrVal()); //ldr r8,=.L1, .L1 byte "1234567"
        //初始值已经加载完成！一般在r8
        str_var(init_reg, var, tmp_reg); //将初始值保存到变量内存
    }
    */
}

void ILoc::ldr_var(string rs_reg, Var *var)
{
    if (!var)
        return;                    //无效变量
    bool is_half = var->is_half(); //是否是字符变量，需要类型转换
    if (!var->is_literal())
    {                                                     //非常量
        int id = var->reg_id;                             //-1表示非寄存器，其他表示寄存器的索引值
        if (id != -1)                                     //寄存器变量
            emit("mov", rs_reg, Arm32Plat::reg_name[id]); //mov r8,r2 | 这里有优化空间——消除r8
        else
        {                                  //所有定义的变量和数组
            int off = var->get_f_offset(); //栈帧偏移
            bool isGlb = !off;             //变量偏移，0表示全局，其他表示局部
            bool isVar = !var->is_array(); //纯变量，0表示数组，1表示变量
            if (isGlb)
            {                                    //全局符号
                ldr_lb(rs_reg, var->get_name()); //ldr r8,=glb
                if (isVar)
                    ldr_base(rs_reg, rs_reg, 0, is_half); //ldr r8,[r8]
            }
            else
            { //局部符号
                if (isVar)
                    ldr_base(rs_reg, "fp", off, is_half); //ldr r8,[fp,#-16]
                else
                    lea_stack(rs_reg, off); //add r8,fp,#-16
            }
        }
    }
    else
    {                                        //常量
        if (var->is_base())                  //是基本类型 100 'a'
            ldr_imm(rs_reg, var->get_val()); //ldr r8,#100
        else if (var->is_string())           //常量字符串
            ldr_lb(rs_reg, var->get_name()); //ldr r8,=.L1, .L1 byte "1234567"
    }
}

void ILoc::lea_var(string rs_reg, Var *var)
{
    //被加载的变量肯定不是常量！
    //被加载的变量肯定不是寄存器变量！
    int off = var->get_f_offset();       //栈帧偏移
    bool isGlb = !off;                   //变量偏移，0表示全局，其他表示局部
    if (isGlb)                           //全局符号
        ldr_lb(rs_reg, var->get_name()); //ldr r9,=glb 地址寄存器加载变量地址
    else
    {                           //局部符号
        lea_stack(rs_reg, off); //lea r8,[fp,#-16]
    }
}

void ILoc::str_var(string src_reg, Var *var, string tmp_reg)
{
    //被保存目标变量肯定不是常量！
    int id = var->reg_id;          //-1表示非寄存器，其他表示寄存器的索引值
    bool is_half = var->is_half(); //是否是字符变量，需要类型转换
    if (id != -1)
    {                                                  //寄存器变量
        emit("mov", Arm32Plat::reg_name[id], src_reg); //mov r2,r8 | 这里有优化空间——消除r8
    }
    else
    {                                  //所有定义的变量,不可能是数组！
        int off = var->get_f_offset(); //栈帧偏移
        bool isGlb = !off;             //变量偏移，0表示全局，其他表示局部
        if (isGlb)
        {                                                    //全局符号
            ldr_lb(tmp_reg, var->get_name());                //ldr r9,=glb 地址寄存器加载变量地址
            str_base(src_reg, tmp_reg, 0, tmp_reg, is_half); //str r8,[r9]
        }
        else
        {                                                   //局部符号
            str_base(src_reg, "fp", off, tmp_reg, is_half); //str r8,[fp,#-16]
        }
    }
    nop(); //分割优化
}

void ILoc::lea_stack(string rs_reg, int off)
{
    if (Arm32Plat::const_expr(off))
        emit("add", rs_reg, "fp", to_string(off)); //add r8,fp,#-16
    else
    {
        ldr_imm(rs_reg, off);              //ldr r8,=-257
        emit("add", rs_reg, "fp", rs_reg); //add r8,fp,r8
    }
}

void ILoc::alloc_stack(Func *func, string tmp_reg)
{
    // int base=(fun->is_relocated())?(Arm32Plat::stackBase_protect):(Arm32Plat::stack_base);
    int base = Arm32Plat::stack_base;
    if (func->is_relocated())
        base = Arm32Plat::stackBase_protect;
    int off = func->get_scope()->get_size() - base; //计算栈帧大小
    if (Arm32Plat::const_expr(off))
        emit("sub", "sp", "sp", to_string(off)); //sub sp,sp,#16
    else
    {
        ldr_imm(tmp_reg, off);            //ldr r8,=257
        emit("sub", "sp", "sp", tmp_reg); //sub sp,sp,r8
    }
}

void ILoc::ldr_args(Func *fun)
{
    vector<Var *> args = fun->get_para_vars(); //获取参数
    for (unsigned int i = 0; i < args.size(); i++)
    {
        Var *arg = args[i];
        if (arg->reg_id != -1)
        {                                                                                 //参数在寄存器，需要加载,不用理会是否是字符，统一4字节加载
            ldr_base(Arm32Plat::reg_name[arg->reg_id], "fp", arg->get_f_offset(), false); //ldr r0,[fp,#4]
        }
    }
}

void ILoc::call_lib(string fun, string rs_reg, string reg1, string reg2)
{
    emit("stmfd", "sp!", "{r0-r7}"); //保护现场
    emit("mov", "r0", reg1);         //传递操作数
    emit("mov", "r1", reg2);
    emit("bl", fun);
    emit("mov", rs_reg, "r0");       //取返回结果
    emit("ldmfd", "sp!", "{r0-r7}"); //恢复现场
}

void ILoc::call_func(Func *fun, string tmp_reg)
{
    string func_name = fun->get_name();
    emit("bl", func_name);                 //函数返回值在r8,不需要保护
    int off = fun->get_para_vars().size(); //需要弹出参数的个数
    off *= 4;                              //参数都是4字节
    if (Arm32Plat::const_expr(off))
        emit("add", "sp", "sp", to_string(off)); //add sp,sp,#16
    else
    {
        ldr_imm(tmp_reg, off);            //ldr r8,=257
        emit("add", "sp", "sp", tmp_reg); //add sp,sp,r8
    }
}

void ILoc::logic_and(string rs_reg, string reg1, string reg2)
{
    emit("cmp", reg1, "#0");     //r8=0?
    emit("moveq", rs_reg, "#0"); //r8=0
    emit("movne", rs_reg, "#1"); //r8=1
    emit("cmpne", reg2, "#0");   //r9=0?
    emit("moveq", rs_reg, "#0"); //r8=0
}

void ILoc::logic_or(string rs_reg, string reg1, string reg2)
{
    emit("cmp", reg1, "#0");     //r8=0?
    emit("moveq", rs_reg, "#0"); //r8=0
    emit("movne", rs_reg, "#1"); //r8=1
    emit("cmpeq", reg2, "#0");   //r9=0?
    emit("movne", rs_reg, "#1"); //r8=0
}

void ILoc::logic_not(string rs_reg, string reg1)
{
    emit("cmp", reg1, "#0");     //r8=0
    emit("moveq", rs_reg, "#1"); //r8=1
    emit("movne", rs_reg, "#0"); //r8=0
}

void ILoc::cmp(string cmp, string cmpnot, string rs_reg, string reg1, string reg2)
{
    emit("cmp", reg1, reg2);            //r8 ? r9
    emit("mov" + cmp, rs_reg, "#1");    //r8=0
    emit("mov" + cmpnot, rs_reg, "#0"); //r8=1
}

void ILoc::nop()
{
    emit(""); //无操作符
}

ILoc::~ILoc()
{
    for (list<Arm32 *>::iterator it = code.begin(); it != code.end(); ++it)
    {
        delete (*it);
    }
}

void ILoc::output(FILE *file)
{
    for (list<Arm32 *>::iterator it = code.begin(); it != code.end(); ++it)
    {
        string s = (*it)->output();
        if (s != "")
            fprintf(file, "%s\n", s.c_str());
    }
}

list<Arm32 *> &ILoc::get_code()
{
    return code;
}
