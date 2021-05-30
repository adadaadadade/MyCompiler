#  C0 Grammar, without annotations

<prog> ::= <typedef_def> <prog> | <struct_base> <prog> | <Gfn> <prog>
    | $

<typedef_def> ::= KW_TYPEDEF <tp> <aid> SEMICON


<struct_base> ::= KW_STRUCT <sid> <struct_decl_ordefn_orfn>

<struct_decl_ordefn_orfn> ::= SEMICON 
    | LBRACE <structdatalist> RBRACE SEMICON
    | <tp_tail> <vid> LPAREN <paralist> RPAREN <fn_decl_ordefn>

<struct_datalist> ::= <tp> <fid> SEMICON <struct_datalist>
    | $


<Gfn> ::= <tp_without_struct> <vid> LPAREN <paralist> RPAREN <fn_decl_ordefn>

<fn_decl_ordefn> ::= SEMICON | LBRACE <body> RBRACE

<paralist> ::= <tp> <vid> <para>
    | $

<para> ::= COMMA <tp> <vid> <para>
    | $


<body> ::= <decllist> <stmtlist>

<decllist> ::= <decl> <decllist>
    | $

<decl> ::= <tp> <vid> <init> SEMICON

<init> ::= ASN <exp>
    | $


<stmtlist> ::= <stmt> <stmtlist>
    | $

<stmt> ::= <simple> SEMICON
    | KW_IF LPAREN <exp> RPAREN <stmt> <else>
    | KW_WHILE LPAREN <exp> RPAREN <stmt>
    | KW_FOR LPAREN <simple_ornot> SEMICON <exp> SEMICON <simple_ornot> RPAREN <stmt>
    | KW_CONTINUE SEMICON
    | KW_BREAK SEMICON
    | KW_RETURN <exp_ornot> SEMICON
    | LBRACE <body> RBRACE
    | KW_ASSERT LPAREN <exp> COMMA <exp> RPAREN SEMICON
    | SEMICON

<else> ::= KW_ELSE <stmt> 
    | $

<simple_ornot> ::= <simple> 
    | $

<exp_ornot> ::= <exp> 
    | $

<simple> ::= <lv> <asnop> <exp>
    | <lv> ADD ADD
    | <lv> SUB SUB
    | <exp>


<lv> ::= <lv_base> <lv_tail> 

<lv_base> ::= <vid> | MUL <lv>

<lv_tail> ::= DOT <fid> <lv_tail> | SUB GTH <fid> <lv_tail>
    | LBRACK <exp> RBRACK <lv_tail> 
    | $


<tp> ::= <tp_base> <tp_tail>

<tp_base> ::= KW_INT | KW_BOOL | KW_STRING | KW_CHAR
    | KW_STRUCT <sid> | <aid>

<tp_without_struct> ::= <tp_without_struct_base> <tp_tail>

<tp_without_struct_base>::= KW_INT | KW_BOOL | KW_STRING | KW_CHAR
    | <aid>

<tp_tail> ::= MUL <tp_tail> | LBRACK RBRACK <tp_tail>
    | $


<exp> ::= <exp_base> <exp_tail>

<exp_base> ::= LPAREN <exp> RPAREN
    | NUMLIT | STRLIT | CHRLIT | KW_TRUE | KW_FALSE | KW_NULL
    | KW_ALLOC LPAREN <tp> RPAREN | KW_ALLOC_ARRAY LPAREN <tp> COMMA <exp> RPAREN
    | MUL <exp>
    | <unop> <exp>
    | <vid> LPAREN <call_ornot> RPAREN
    | <vid> LBRACK <exp> RBRACK

<call_ornot> ::=  <call_paralist>  
    | $

<call_paralist> ::= <exp> <call_para> 
    | $

<call_para> ::= COMMA <exp> 
    | $

<exp_tail> ::= <binop> <exp> <exp_tail>
    | QUE <exp> COLON <exp> <exp_tail>
    | DOT <fid> | <exp> SUB GTH <fid> <exp_tail>
    | LBRACK <exp> RBRACK <exp_tail>
    | $

<vid> ::= ID

<sid> ::= ID

<fid> ::= ID

<aid> ::= ID

<unop> ::= NOT | BNEG | SUB | MUL

<binop> ::= ADD | SUB | MUL | DIV | MOD |LMO | RMO | LTH | GTH | EQU | NEQ | AND | XOR | OR  | LAND | LOR
	
<asnop> ::= ASN | ASNADD | ASNSUB | ASNMUL | ASNDIV | ASNMOD | ASNLMO | ASNRMO | ASNAND | ASNXOR | ASNOR