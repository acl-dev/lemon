#include <string>
#include <vector>
#include <map>
size_t length(const std::string &str)
{
    return str.size();
}
std::string hello(const std::string &hello,const std::string &name, std::vector<std::vector<std::string> > &table)
{
	std::string code;
	code += "<HTML>";
	code += "<HEAD>";
	code += "<METANAME=\"GENERATOR\"Content=\"MicrosoftVisualStudio\">";
	code += "<TITLE></TITLE>";
	code += "</HEAD>";
	code += "<BODY>";
	code += "hello";
	code += hello;
	if(length(name)>0)
	{
		code += "name:";
		code += name;
	}
	code += "<table>";
	std::vector<std::vector<std::string> >::const_iterator it1 = table.begin();
	for (; it1 != table.end(); ++it1)
	{
		const std::vector<std::string>  &items = *it1;
		code += "<tr>";
		std::vector<std::string> ::const_iterator it2 = items.begin();
		for (; it2 != items.end(); ++it2)
		{
			const std::string &item = *it2;
			code += "<td>";
			code += "hello:";
			code += hello;
			code += "name:";
			code += name;
			code += "item:";
			code += item;
			code += "</td>";
		}
		code += "</tr>";
	}
	code += "</table>";
	code += "</BODY>";
	code += "</HTML>";
	return code;
}
