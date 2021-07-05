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
    | <lv> INC
    | <lv> DEC
    | <exp>


<lv> ::= <lv_base> <lv_tail> 

<lv_base> ::= <vid> | MUL <lv>

<lv_tail> ::= DOT <fid> <lv_tail> | ARROW <fid> <lv_tail>
    | LBRACK <exp> RBRACK <lv_tail> 
    | $


<tp> ::= <tp_base> <tp_tail>

<tp_base> ::= KW_INT | KW_BOOL | KW_STRING | KW_CHAR
    | KW_STRUCT <sid> | <aid>

<tp_without_struct> ::= <tp_without_struct_base> <tp_tail>

<tp_without_struct_base>::= KW_INT | KW_BOOL | KW_STRING | KW_CHAR
    | <aid>

<tp_tail> ::= MUL <tp_tail> | LBRACK <literal> RBRACK <tp_tail>
    | $

<expr> ::= <triexpr>

<triexpr> ::= <lorexpr> <tritail> 

<tritail> ::= QUE <lorexpr> <tritail> COLON <lorexpr> <tritail>
    | $

<lorexpr> ::= <landexpr> <lortail>

<lortail> ::= LOR <landexpr> <lortail>
    |$

<landexpr> ::= <borexpr> <landtail>

<landtail> ::= LAND <borexpr> <landtail>
    |$

<borexpr> ::= <bxorexpr> <bortail>

<bortail> ::= BOR <bxorexpr> <bortail>
    |$

<bxorexpr> ::= <bandexpr> <bxortail>

<bxortail> ::= BXOR <bandexpr> <bxortail>

<bandexpr> ::= <equexpr> <bandtail>

<bandtail> ::= BAND <equexpr> <bandtail>
    |$

<equexpr> ::= <cmpexpr> <equtail>

<equtail> ::= <equ_binop> <cmpexpr> <equtail>
    |$

<equ_binop> ::= EUQ | NEQ

<cmpexpr> ::= <shiftexpr> <cmptail>

<cmptail> ::= <cmp_binop> <shiftexpr> <cmptail>
    |$

<cmp_binop> ::= LTH | GTH | LET | GET

<shiftexpr> ::= <aloexpr> <shifttail>

<shifttail> ::= <shift_binop> <aloexpr> <shifttail>
    |$

<shift_binop> ::= LMO | RMO

<aloexpr> ::= <mlusexpr> <alotail>

<alotail> ::= <alo_binop> <mlusexpr> <alotail>
    |$

<alo_binop> ::= ADD | SUB

<mlusexpr> ::= <unopexpr> <mulstail>

<mulstail> ::= <muls_binop> <unopexpr> <mulstail>
    |$

<muls_binop> ::= MUL | DIV | MOD

<unopexpr> ::= <unop> <unopexpr>
    | <elem>

<unop> ::= NOT | BNEG | SUB | MUL | INC | DEC

<elem> ::= LPAREN <expr> RPAREN
    | ID <idexpr>
    | <allocs>
    | <literal>

<alocs> ::= KW_ALLOC LPAREN <tp> RPAREN
    | KW_ALLOC_ARRAY LPAREN <tp> COMMA <expr> RPAREN

<idexpr> ::= LBRACK <expr> RBRACK
    | LPAREN <realarg> RPAREN
    |$

<realarg> ::= <arg> <arglist>
    |$

<arg> ::= <expr>

<arglist> ::= COMMA <arg> <arglist>
    |$

<literal> ::= NUMLIT | CHRLIT | STRLIT

<vid> ::= ID

<sid> ::= ID

<fid> ::= ID

<aid> ::= ID

<unop> ::= NOT | BNEG | SUB | MUL

<binop> ::= ADD | SUB | MUL | DIV | MOD |LMO | RMO | LTH | GTH | EQU | NEQ | AND | XOR | OR  | LAND | LOR
	
<asnop> ::= ASN | ASNADD | ASNSUB | ASNMUL | ASNDIV | ASNMOD | ASNLMO | ASNRMO | ASNAND | ASNXOR | ASNOR


dec .L9
.L9 = 0 * 4
dec .L8
.L8 = arr + .L9
dec .L10
*.L8 = 1
dec .L12
.L12 = 0 * 4
dec .L11
.L11 = arr + .L12
dec .L13
*.L11 = 2
dec .L15
.L15 = 1 * 4
dec .L14
.L14 = arr + .L15
dec .L16
*.L14 = 3
return 0 goto .L5
.L5:
exit


	mov r8,#0
	mov r9,#4
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-44]
	add r8,fp,#-32
	ldr r9,[fp,#-44]
	add r8,r8,r9
	str r8,[fp,#-48]
	mov r8,#1
	ldr r9,[fp,#-48]
	str r8,[r9]
	mov r8,#0
	mov r9,#4
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-56]
	add r8,fp,#-32
	ldr r9,[fp,#-56]
	add r8,r8,r9
	str r8,[fp,#-60]
	mov r8,#2
	ldr r9,[fp,#-60]
	str r8,[r9]
	mov r8,#1
	mov r9,#4
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-68]
	add r8,fp,#-32
	ldr r9,[fp,#-68]
	add r8,r8,r9
	str r8,[fp,#-72]
	mov r8,#3
	ldr r9,[fp,#-72]
	str r8,[r9]
