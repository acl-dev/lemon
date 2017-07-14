#include <string>
#include <vector>
#include "hello.lm.h"
namespace
{
    std::string to_string()
    {

    }
}
std::string lm::hello(const std::string &hello,
                      const std::string &name,
                      const std::vector<std::vector<std::string> > &table)
{
    std::string code;
    code += "<HTML>";
    code += "<HEAD>";
    code += "<META NAME = \"GENERATOR\" Content = \"Microsoft Visual Studio\">";
    code += "<TITLE></TITLE>";
    code += "</HEAD>";
    code += "<BODY>";

    code += hello;
    code += name;

    code += "<table border='1'>";

    std::vector<std::vector<std::string> >::const_iterator it1 = table.begin();
    for (; it1 != table.end(); ++it1)
    {
        code += "<tr>";
        const std::vector<std::string> &items = *it1;
        std::vector<std::string>::const_iterator it2 = items.begin();
        for (; it2 != items.end(); it2++)
        {
            const std::string &item = *it2;
            code += "<td>";
            code += item;
            code += "</td>";
        }
        code += "</tr>";
    }
    code += "</table>";
    code += "</BODY>";
    code += "</HTML>";
}