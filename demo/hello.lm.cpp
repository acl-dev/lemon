#include <string>
#include <vector>
#include "hello.lm.h"

static inline std::string lemon1(const std::string &hello)
{
    return hello;
}
static inline std::string lemon21(const std::string &name)
{
    return name;
}
static inline std::string lemon2(const std::string &name)
{
    std::string code;
    //{%if name %} name: {{name}}{%endif%}
    if (name.size())
    {
        code += "name:";
        code += lemon21(name);
    }
    return code;
}
static inline std::string lemon311(const std::string &item)
{
    return item;
}
static inline std::string lemon31(const std::vector<std::string> &items)
{
    /*
    {%for item in items%}
    <td>
    {{item}}
    </td>
    {%endfor%}
    */
    std::string code;
    for (size_t i = 0; i < items.size(); i++)
    {
        code += "<td>";
        code += lemon311(items[i]);
        code += "</td>";
    }
    return code;
}

static inline std::string lemon3(const std::vector<std::vector<std::string>> &table)
{
    /*
    {%for items in table%}
    <tr>
    {%for item in items%}
    <td>
    {{item}}
    </td>
    {%endfor%}
    </tr>
    {%endfor%}
    */
    std::string code;
    for (size_t i = 0; i < table.size(); i++)
    {
        code += "<tr>";
        code += lemon31(table[i]);
        code += "</tr>";
    }
    return code;
}
std::string lm::hello(const std::string &hello, const std::string &name, const std::vector<std::vector<std::string>> &table)
{
    return
        "<HTML>"
        "<HEAD>"
        "<META NAME = \"GENERATOR\" Content = \"Microsoft Visual Studio\">"
        "<TITLE></TITLE>"
        "</HEAD>"
        "<BODY>" +
        lemon1(hello) +
        lemon2(name) +
        "<table border='1'>"+
        lemon3(table) + 
        "</table>"
        "</BODY>"
        "</HTML>";
}