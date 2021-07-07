#include "lexer.h"
#include "util.h"

Lexer::Lexer(Scanner& sc) : scanner(sc)
{
    token = NULL;
    ch = ' ';
}


Lexer::~Lexer()
{
    if(!token)
    {
        delete token;
    }
}


bool Lexer::scan(char need)
{
	ch = scanner.nextChar();//扫描出字符
	if(need){
		if(ch != need)//与预期不吻合
			return false;
		ch = scanner.nextChar();//与预期吻合，扫描下一个
		return true;
	}
	return true;
}

Token* Lexer::nextToken()
{
    while(ch != EOF)
    {
        Token *t = NULL;
        while(isspace(ch))//忽略空白
            scan();
        
        if(isalphaA_(ch))   //标识符，关键字
        {
            string str = "";
            TokenType tag;
            do
            {
                str.push_back(ch);
                scan();
            } while (isalphaA_(ch) || isdigit(ch));
            if((tag = TokenType(KeyWords::getKWType(str))) != -1)   //判断是关键词
                t = new Token(tag);
            else
                t = new Id(str);
        }
        else if(isdigit(ch))    //数字
        {
            string str = "";
            str.push_back(ch);
            scan();
            if(str[0] == '0' && isXx(ch))   //是0x 0X
            {
                scan();
                str.clear();
                do
                {
                    str.push_back(ch);
                    scan();
                } while (ishexnum(ch));
                t = new Numlit(stoi(str, nullptr, 16), HEXNUM);
            }
            else    //是十进制
            {
                if(str[0] != '0' && isdigit(ch)) //判断非0
                {
                    do
                    {
                        str.push_back(ch);
                        scan();
                    } while (isdigit(ch));
                }
                t = new Numlit(stoi(str, nullptr, 10), DECNUM);
            }
        }
        else if(ch == '"') //字符串/////////////////////////????
        {
            string str = "";
            while(!scan('"'))
            {
                if(ch == '\\')
                {
                    scan();
                    if(!esc2chr(str,ch))
                    {
                        ;//error
                        t = new Token(ERR);
                        break;
                    } 
                }
                else if(ch == '\n' || ch == EOF)
                {
                    ;//error
                    t = new Token(ERR);
                    break;
                }
                else
                {
                    str.push_back(ch);
                }
            }
            if(!t)
                t = new Strlit(str);
        }
        else if(ch == '\'')//单字符
        {
            string str = "";
            scan();
            if(ch == '\\')
            {
                if(!esc2chr(str,ch))
                {
                    ;//error
                    t = new Token(ERR);
                }
            }
            else if(!iscchar(ch))
            {
                ;//error
                t = new Token(ERR);
            }
            else
                str[0] = ch;
            if(!scan('\''))
                t = new Token(ERR);
            else if(!t)
            {
                t = new Chrlit(str[0]);
            }   
        }
        else    //op
        {
            switch (ch)
            {
            case '#':   //宏定义
                while(ch != '\n' && ch != EOF)
                    scan();
                t = new Token(DEF);
                break;
            case '!':   // ! !=
                t = new Token(scan('=') ? NEQ : NOT);
                break;
            case '~':   // ~
                t = new Token(BNEG);
                scan();
                break;
            case '+':   // + += ++
                scan();
                if (ch == '=')
                {
                    scan();
                    t = new Token(ASNADD);
                }
                else if (ch == '+')
                {
                    scan();
                    t = new Token(INC);
                }
                else
                    t = new Token(ADD);
                break;
            case '-':   // - -= -- ->
                scan();
                if (ch == '=')
                {
                    scan();
                    t = new Token(ASNSUB);
                }
                else if (ch == '-')
                {
                    scan();
                    t = new Token(DEC);
                }
                else if (ch == '>')
                {
                    scan();
                    t = new Token(ARROW);
                }
                else
                    t = new Token(SUB);
                break;
            case '*':   // * *=
                t = new Token(scan('=') ? ASNMUL : MUL);
                break;
            case '/':   // / /= // /* */
                scan();
                if(ch == '/')
                {
                    while(ch != '\n' && ch != EOF)
                        scan();
                    t = new Token(COMMIT);
                }
                else if(ch == '*')
                {
                    do
                    {
                        scan();
                        if(ch == EOF)
                        {
                            t = new Token(ERR);
                            break;
                        }
                    } while (ch == '*' && scan('/'));
                    if(!t)
                        t = new Token(COMMIT);
                }
                else if(ch == '=')
                {
                    t = new Token(ASNDIV);
                    scan();
                }
                else
                    t = new Token(DIV);
                break;

            case '%':   // % %=
                t = new Token(scan('=') ? ASNMOD : MOD);
                break;
            case '<':   // < << <= <<=
                scan();
                if(ch == '<')
                {
                    if(scan('='))
                        t = new Token(ASNLMO);
                    else
                        t = new Token(LMO);
                }
                else if(ch == '=')
                {
                    scan();
                    t = new Token(LEQ);
                }
                else
                    t = new Token(LTH);
                break;

            case '>':   // > >> >>=
                scan();
                if(ch == '>')
                {
                    if(scan('='))
                        t = new Token(ASNRMO);
                    else
                        t = new Token(RMO);
                }
                else if(ch == '=')
                {
                    scan();
                    t = new Token(GEQ);
                }
                else
                    t = new Token(GTH);
                break;

            case '=':   // = ==
                t = new Token(scan('=') ? EQU : ASN);
                break;
            case '&':   // & && &=
                scan();
                if(ch =='&')
                {
                    t = new Token(LAND);
                    scan();
                } 
                else if(ch == '=')
                {
                    t = new Token(ASNAND);
                    scan();
                }
                else
                    t = new Token(BAND);
                break;

            case '^':   // ^ ^=
                t = new Token(scan('=') ? ASNXOR : BXOR);
                break;
            case '|':   // | || |=
                scan();
                if(ch =='|')
                {
                    t = new Token(LOR);
                    scan();
                } 
                else if(ch == '=')
                {
                    t = new Token(ASNOR);
                    scan();
                }
                else
                    t = new Token(BOR);
                break;

            case '(':
                t = new Token(LPAREN);scan();
                break;
            case ')':
                t = new Token(RPAREN);scan();
                break;
            case '[':
                t = new Token(LBRACK);scan();
                break;
            case ']':
                t = new Token(RBRACK);scan();
                break;
            case '{':
                t = new Token(LBRACE);scan();
                break;
            case '}':
                t = new Token(RBRACE);scan();
                break;
            case ',':
                t = new Token(COMMA);scan();
                break;
            case ':':
                t = new Token(COLON);scan();
                break;
            case ';':
                t = new Token(SEMICON);scan();
                break;
            case '?':
                t = new Token(QUE);scan();
                break;
            case '.':
                t = new Token(DOT);scan();
                break;
            case EOF:
                t = new Token(END);
                break;

            default:
                t = new Token(ERR);
                scan();
                break;
            }
        }
        if(token)
            delete token;
        token = t;
        return t;
    }
    if(token) 
        delete token;
    return token = new Token(END);
}