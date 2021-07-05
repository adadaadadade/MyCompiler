#pragma once

#include "common.h"

class Node;
class Asnop;
class Str;
class Chr;
class Num;
class Bool;
class Vid;
class Sid;
class Fid;
class Aid;
class Tp;
class Exp;
class Decl;
class Lv;
class Simple;
class Stmt;
class Gdecl;
class Gdefn;
class Prog;

class Node
{
    const string nodetype = "Node";
public:
    Node *parent;
    int deep;
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
    virtual void set_parent(Node* par);
};

class Asnop : public Node
{
    const string nodetype = "Asnop";
public:
    enum AsnopType
    {
        ASN,
        ASNADD,
        ASNSUB,
        ASNMUL,
        ASNDIV,
        ASNMOD,
        ASNLMO,
        ASNRMO,
        //位运算
        ASNAND,
        ASNXOR,
        ASNOR
    };
    static string TypeName[];
    AsnopType type;

    Asnop();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Str : public Node
{
    const string nodetype = "Str";
public:
    string str;

    Str();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Chr : public Node
{
    const string nodetype = "Chr";
public:
    char ch;
    string name;

    Chr();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Num : public Node
{
    const string nodetype = "Num";
public:
    long long int value;

    Num();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Bool : public Node
{
    const string nodetype = "Bool";
public:
    bool boollit;

    Bool();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};
// 变量或函数id
class Vid : public Node
{
    const string nodetype = "Vid";
public:
    string name;

    Vid();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

// 结构体id
class Sid : public Node
{
    const string nodetype = "Sid";
public:
    string name;

    Sid();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

// 属性id
class Fid : public Node
{
    const string nodetype = "Fid";
public:
    string name;

    Fid();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

// 自定义类型id
class Aid : public Node
{
    const string nodetype = "Aid";
public:
    string name;

    Aid();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Tp : public Node
{
    const string nodetype = "Tp";
public:
    enum TpType
    {
        INT,
        BOOL,
        STRING,
        CHAR,
        POINTER,
        ARRAY,
        STRUCT,
        AID
    };
    static string TypeName[];
    TpType type;

    //Tp *tp;
    Tp *tp_tail;
    Sid *sid;
    Aid *aid;
    int size; // 数组大小
    //int base_size;

    Tp(TpType type);
    //复制一个tp
    Tp(Tp *tp);
    Tp();

    void add_tail(Tp *tail);
    
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
    //static bool tp_equal(Tp* a, Tp* b);
    //int get_base_size(); //返回基本大小
    //int get_size();
};


class Paralist : public Node
{
    const string nodetype = "Paralist";
public:
    vector<Tp *> tp_list;
    vector<Vid *> vid_list;

    Paralist();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};
/*
class Call_Paralist : public Node
{
    const string nodetype = "Call_Paralist";
public:
    vector<Node *> exp_list;
    
    Call_Paralist();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};
*/

class Exp : public Node
{
    const string nodetype = "Exp";
public:
    enum ExpType
    {
        PAREN,
        NUMLIT,
        STRLIT,
        CHRLIT,
        BOOLLIT,
        NULLLIT,
        VID,
        BINOP,
        UNOP,
        TRI,
        FUNC,
        DOTFID,
        TOFID,
        ARRAY,
        //POINTER,
        ALLOC,
        ALLOCARRAY
    };
    static string TypeName[];
    ExpType type;
    TokenType tag;

    //Unop *unop;
    Exp *exp1;
    Exp *exp2;
    Exp *exp3;
    Exp *exp_tail;
    int numlit;
    char chrlit;
    string strlit;
    bool boollit;

    bool is_array_index;
    bool is_func_arg;

    Vid *vid;

    Fid *fid;
    Tp *tp;

    const static int a = 0;
    Exp();
    static Exp* binop_exp(Exp* lval,TokenType tag, Exp* rval);
    static Exp* unop_exp(TokenType tag, Exp* e);
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Decl : public Node
{
    const string nodetype = "Decl";
public:
    Tp *tp;
    Vid *vid;
    Exp *exp;

    Decl();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Body : public Node
{
    const string nodetype = "Body";
public:
    vector<Decl *> decl_list;
    vector<Stmt *> stmt_list;

    Body();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Lv : public Node
{
    const string nodetype = "Lv";
public:
    enum LvType
    {
        VID,
        LVDOTVID,
        LVTOVID,
        LVPOINTER,
        LVARRAY
    };
    static string TypeName[];
    LvType type;

    Vid *vid;
    Lv *lv;
    Lv *lv_tail;
    Fid *fid;
    Exp *exp;

    Lv();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Simple : public Node
{
    const string nodetype = "Simple";
public:
    enum SimpleType
    {
        ASN,
        ADDADD,
        SUBSUB,
        EXP
    };
    static string TypeName[];
    SimpleType type;

    Lv *lv;
    Asnop *asnop;
    Exp *exp;

    Simple();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Stmt : public Node
{
    const string nodetype = "Stmt";
public:
    enum StmtType
    {
        SIMPLE,
        IF,
        WHILE,
        FOR,
        CONTINUE,
        BREAK,
        RETURN,
        BODY,
        ASSERT,
        EMPTY
    };
    static string TypeName[];
    StmtType type;

    Simple *simple1;
    Simple *simple2;

    Exp *exp1;
    Exp *exp2;
    Stmt *stmt1;
    Stmt *stmt2;
    Body *body;

    Stmt();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};


class Gdecl : public Node
{
    const string nodetype = "Gdecl";
public:
    enum GdeclType
    {
        STRUCTDECL,
        FUNCDECL
    };
    static string TypeName[];
    GdeclType type;

    Vid *vid;

    Tp *tp;
    Paralist* pl;

    Gdecl();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Gdefn : public Node
{
    const string nodetype = "Gdefn";
public:
    enum GdefnType
    {
        STRUCTDEFN,
        FUNCDEFN,
        TYPEDEF
    };
    static string TypeName[];
    GdefnType type;

    Sid *sid;
    vector<Tp *> tp_list;
    vector<string> name_list;
    vector<Fid *> fid_list;
    Paralist* pl;

    Tp *tp;
    Vid *vid;

    Body *body;
    Aid *aid;

    Gdefn();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};

class Prog : public Node
{
    const string nodetype = "Prog";
public:
    vector<Node*> gdecl_gdefn_list;
    vector<Gdecl*> gdecl_list;
    vector<Gdefn*> gdefn_list;

    Prog();
    virtual string toString();
    virtual void print();
    virtual string get_nodetype();
};