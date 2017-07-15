#include <iostream>
#include <string>
#include <vector>
#include "hello.lm.h"

int main()
{
    std::vector<std::vector<std::string >> table;

    std::vector<std::string > colum;
    colum.push_back("hello 1");
    colum.push_back("hello 2");
    colum.push_back("hello 3");
    colum.push_back("hello 4");
    colum.push_back("hello 5");
    table.push_back(colum);
    colum.clear();

    colum.push_back("world 1");
    colum.push_back("world 2");
    colum.push_back("world 3");
    colum.push_back("world 4");
    colum.push_back("world 5");
    table.push_back(colum);

    std::cout << hello("hello world"," akzi",table) << std::endl;
}