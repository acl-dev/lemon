#define _CRT_SECURE_NO_WARNINGS
#include "acl_cpp/lib_acl.hpp"
#include "lemon.h"


int main()
{
    lemon lm;

    if(lm.parse_cpp_header("hello.h"))
        lm.parse_template("hello.lm");

    return 0;
}