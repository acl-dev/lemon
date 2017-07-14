#define _CRT_SECURE_NO_WARNINGS
#include "acl_cpp/lib_acl.hpp"
#include "lemon.h"


int main()
{
    lemon lm;

    lm.parse_template("hello.lm");

    return 0;
}