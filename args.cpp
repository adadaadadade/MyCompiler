#include "args.h"
#include <cstring>
/*
-o <outputfile_name>
-c --showchar   show_char
-t --showtoken  show_token
-a --showast    show_ast
-h --help       show_help
*/

char *Args::srcfiles[100] = {};
int Args::srcfile_idx = 0;
char *Args::outputfile = NULL;
bool Args::next_outputfile_name = false;
bool Args::show_help = false;
bool Args::show_char = false;
bool Args::show_token = false;
bool Args::show_ast = false;
bool Args::show_symtab = false;
bool Args::show_ir = false;

Args::Args()
{
}

Args::~Args()
{
}

void Args::get_args(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) //取第0个参数即目录之后的参数
    {
        if (argv[i][0] == '-')
        {
            if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
                Args::show_help = true;
            if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--showchar"))
                Args::show_char = true;
            if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--showtoken"))
                Args::show_token = true;
            if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "--showast"))
                Args::show_ast = true;
            if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--showsymtab"))
                Args::show_symtab = true;
            if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--showir"))
                Args::show_ir = true;
            if (!strcmp(argv[i], "-o"))
                Args::next_outputfile_name = true;
        }
        if (next_outputfile_name)
        {
            outputfile = argv[i];
            next_outputfile_name = false;
        }
        else
        {
            srcfiles[srcfile_idx] = argv[i];
            srcfile_idx++;
        }
    }
    if (show_help || !srcfiles[0])
    {
        cout << "-o <outputfile>\toutput file name\n"
                "-c --showchar\tshow char\n"
                "-t --showtoken\tshow token\n"
                "-a --showast\tshow ast\n"
                "-s --showsymtab\t show symtab\n"
                "-i --showir\t show ir\n"
                "-h --help\tshow help\n";
    }
    if (!srcfiles[0])
    {
        cout << "Please input src file name" << endl;
        exit(0);
    }
    if (!outputfile)
    {
        string new_name = srcfiles[0];
        int pos = new_name.rfind(".c");
        if (pos > 0 && pos == new_name.length() - 2)
        {
            new_name.replace(pos, 2, ".s");
        }
        else
            new_name = new_name + ".s";

        outputfile = new char[100];
        strcpy(outputfile, new_name.c_str());
    }
}