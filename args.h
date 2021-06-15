#pragma once
#include "common.h"
#include <vector>


class Args
{
private:

public:
    static char* srcfiles[100];//源文件
    static int srcfile_idx;
    static char* outputfile;
    static bool next_outputfile_name;

    static bool show_help;
    static bool show_char;
    static bool show_token;
    static bool show_ast;
    
    static void get_args(int argc, char *argv[]);
    Args();
    ~Args();
};
