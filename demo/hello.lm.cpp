#include "lemon.hpp"

std::string hello(const std::string &hello,const std::string &name, const std::vector<std::vector<std::string> > &table)
{
	std::string code;
	code += "<HTML>";
	code += "<HEAD>";
	code += "<META NAME=\"GENERATOR\" Content=\"Microsoft Visual Studio\">";
	code += "<TITLE></TITLE>";
	code += "</HEAD>";
	code += "<BODY>";
	code += "---- ";
	code += "hello ";
	code += lm::$default(lm::$escape(hello), "hello is empty!!!!");
	if(lm::$length(name)>0)
	{
		code += "name:";
		code += name;
	}
	code += "<p> - - - - - - - - - </p>";
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
			code += "name: ";
			code += lm::$escape(name);
			code += "item: ";
			code += item;
			code += "</td>";
		}
		if(!name.empty())
		{
			code += "<p>";
			code += hello;
			code += ",I am ";
			code += name;
			code += "other.lm</p>";
		}
		code += "</tr>";
	}
	code += "</table>";
	code += "<p>";
	code += lm::$escape(hello);
	code += lm::$escape(name);
	code += "</p>";
	code += "<p> - - - - - - - - - </p>";
	if(!name.empty())
	{
		code += "<p>";
		code += lm::$escape(hello);
		code += ",I am ";
		code += lm::$escape(name);
		code += "other.lm</p>";
	}
	code += "</BODY>";
	code += "</HTML>";
	return code;
}
