#define _CRT_SECURE_NO_WARNINGS
#include <stdexcept>
#include <iostream>
#include "lib_acl.h"
#include "acl_cpp/lib_acl.hpp"
#include "lemon.h"

struct syntax_error:std::logic_error
{
    syntax_error(const std::string &error = "syntax_error")
        :logic_error(error)
    {

    }
};

inline void skip(std::string &line,
                 const std::string &delims)
{
    size_t offset = 0;

    if (line.empty())
        return;

    for (size_t i = 0; i < line.size(); i++)
    {
        char ch = line[i];
        if (delims.find(ch) != delims.npos)
        {
            offset++;
            continue;
        }
        break;
    }

    if (offset == line.size())
    {
        line.clear();
    }
    else if (offset)
    {
        line = line.substr(offset);
    }
}

lemon::lemon()
{
    iterators_ = 0;
}

bool lemon::parse_template(const std::string &file_path)
{
    if (!file_.open_read(file_path.c_str()))
        return false;
    try
    {
        parse_template();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return false;
    }
    return true;
}
std::string lemon::read_line()
{
    acl::string buffer;
    file_.gets(buffer, false);
    current_line_ = buffer;
    return current_line_;
}
void lemon::move_buffer(int offset)
{
    line_buffer_ = line_buffer_.substr(offset);
}
std::string lemon::get_string(const std::string &delimiters)
{
    std::string buffer;
    if (delimiters.empty())
        return line_buffer_;

    for (size_t i = 0; i < line_buffer_.size(); i++)
    {
        char ch = line_buffer_[i];
        if (delimiters[0] != ch)
        {
            buffer.push_back(ch);
        }
        else 
        {
            int k = 0;
            bool ok = true;
            for (size_t j = i; j < line_buffer_.size() &&
                 k < delimiters.size(); j++,k++)
            {
                if (line_buffer_[j] != delimiters[k])
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
                return buffer;
        }
    }
    return buffer;
}
std::string lemon::get_string(size_t len)
{
    if (len > line_buffer_.size())
        len = line_buffer_.size();

    return std::string (line_buffer_.c_str(),len);
}
std::string lemon::next_token(const std::string &delimiters)
{
    std::string buffer;

    if(line_buffer_.empty())
        line_buffer_ = read_line();

    if (line_buffer_.empty())
        return buffer;

    for (size_t i = 0; i < line_buffer_.size(); i++)
    {
        char ch = line_buffer_[i];
        if (delimiters.find(ch) == std::string::npos)
        {
            buffer.push_back(ch);
        }
        else
        {
            if(buffer.empty())
                buffer.push_back(ch);
            break;
        }
    }
    line_buffer_ = line_buffer_.substr(buffer.size());
    return buffer;
}
lemon::token_t lemon::get_next_token(const std::string &skip_str)
{

    static std::string delimiters = " <>{}()[]%&!?:,\\/.\r\t\n\"'`=-";

    token_t t;
    std::string str;
    if (tokens_.size())
    {
        t = tokens_.front();
        tokens_.pop_front();
        return t;
    }
    else
    {
        if (skip_str.size())
        {
            skip(line_buffer_, skip_str);
        }
        str = next_token(delimiters);
        t.str_ = str;
    }

    if (str == " ")
    {
        t.type_ = token_t::e_space;
    }
    else if (str == "\t")
    {
        t.type_ = token_t::e_$t;
    }
    else if (str == "\r")
    {
        t.type_ = token_t::e_$r;
    }
    else if (str == "\n")
    {
        t.type_ = token_t::e_$n;
    }
   
    else if (str == "")
    {
        t.type_ = token_t::e_eof;
    }
    else if (str == "<")
    {
        t.type_ = token_t::e_less;
        if (get_string(3) == "!--")
        {
            move_buffer(3);
            t.str_ = "<!--";
            t.type_ = token_t::e_comment_begin;
        }
    }
    else if (str == "-")
    {
        t.type_ = token_t::e_sub;
        if (get_string(2) == "->")
        {
            move_buffer(2);
            t.str_ = "-->";
            t.type_ = token_t::e_comment_end;
        }
    }
    else if (str == ">")
    {
        t.type_ = token_t::e_gt;
    }
    else if (str == "{")
    {
        t.type_ = token_t::e_open_brace;
        if (get_string(1) == "{")
        {
            t.type_ = token_t::e_double_open_brace;
            t.str_ = "{{";
            move_buffer(1);
        }
        else if (get_string(1) == "%")
        {
            t.type_ = token_t::e_code_begin;
            t.str_  = "{%";
            move_buffer(1);
        }
    }
    else if (str == "}")
    {
        t.type_ = token_t::e_close_brace;
        if (get_string(1) == "}")
        {
            t.type_ = token_t::e_double_close_brace;
            t.str_ = "}}";
            move_buffer(1);
        }
        else if (get_string(1) == "%")
        {
            t.type_ = token_t::e_code_end;
            t.str_ = "%}";
            move_buffer(1);
        }
    }
    else if (str == "/")
    {
        t.type_ = token_t::e_forward_slash;
    }
    else if (str == "%")
    {
        t.type_ = token_t::e_modulus;
        if (get_string(1) == "}")
        {
            move_buffer(1);
            t.type_ = token_t::e_code_end;
        }
    }
    else if (str == "&")
    {
        t.type_ = token_t::e_and;
    }
    else if (str == "if")
    {
        t.type_ = token_t::e_if;
    }
    else if (str == "else")
    {
        t.type_ = token_t::e_else;
    }
    else if (str == "elif")
    {
        t.type_ = token_t::e_elif;
    }
    else if (str == "endif")
    {
        t.type_ = token_t::e_endif;
    }
    else if (str == "const")
    {
        t.type_ = token_t::e_const;
    }
    else if (str == "for")
    {
        t.type_ = token_t::e_for;
    }
    else if (str == "in")
    {
        t.type_ = token_t::e_in;
    }
    else if (str == "endfor")
    {
        t.type_ = token_t::e_endfor;
    }
    else if (str == ":")
    {
        t.type_ = token_t::e_colon;
        if (get_string(1) == ":")
        {
            move_buffer(1);
            t.type_ = token_t::e_double_colon;
        }
    }
    else if (str == "std")
    {

        token_t t2 = get_next_token();
        if (t2.type_ == token_t::e_double_colon)
        {
            token_t t3 = get_next_token();
            if (t3.str_ == "string")
            {
                t.type_ = token_t::e_std_string;
                t.str_ = "std::string";
            }
            else if (t3.str_ == "vector")
            {
                t.type_ = token_t::e_std_vector;
                t.str_ = "std::vector";
            }
            else if (t3.str_ == "list")
            {
                t.type_ = token_t::e_std_list;
                t.str_ = "std::list";
            }
            else if (t3.str_ == "map")
            {
                t.type_ = token_t::e_std_map;
                t.str_ = "std::map";
            }
            else if (t3.str_ == "set")
            {
                t.type_ = token_t::e_std_set;
                t.str_ = "std::set";
            }
            else
            {
                tokens_.push_back(t2);
                tokens_.push_back(t3);
            }
        }
        else
        {
            tokens_.push_back(t2);
        }
    }
    else if (str == "acl")
    {
        token_t t2 = get_next_token();
        if (t2.type_ == token_t::e_double_colon)
        {
            token_t t3 = get_next_token();
            if (t3.str_ == "string")
            {
                t.type_ = token_t::e_acl_string;
                t.str_ = "acl::string";
            }
        }
    }
    else if (str == ",")
    {
        t.type_ = token_t::e_comma;
    }
    else if (str == ";")
    {
        t.type_ = token_t::e_semicolon;
    }
    else if (str == ":")
    {
        t = get_next_token();
        t.type_ = token_t::e_colon;
        if (line_buffer_.size())
        {
            if (line_buffer_[0] == ':')
            {
                next_token(delimiters);
                t.type_ = token_t::e_double_colon;
            }
        }
    }
    else if (str == "`")
    {
        t.type_ = token_t::e_backtick;
    }
    else if (str == "\"")
    {
        t.type_ = token_t::e_double_quote;
    }
    else if (str == "'")
    {
        t.type_ = token_t::e_quote;
    }
    else if (str == "(")
    {
        t.type_ = token_t::e_open_paren;
    }
    else if (str == ")")
    {
        t.type_ = token_t::e_close_paren;
    }
    else if (str == "=")
    {
        t.type_ = token_t::e_eq;
    }
    else
    {
        t.type_ = token_t::e_identifier;
    }
    t.line_ = line_;
    token_ = t;
    return t;
}
std::string lemon::get_container_str()
{
    std::string code;
   
    int count = 0;
    for (size_t i = 0; i < line_buffer_.size(); ++i)
    {
        char ch = line_buffer_[i];

        if (ch == '<')
        {
            code.push_back(ch);
            count++;
        }
        else if(ch == '>')
        {
            if (count == 0)
            {
                return code;
            }
            else
            {
                code.push_back(ch);
                count--;
                if (count == 0)
                    return code;
            }
        }
        else
        {
            code.push_back(ch);
        }
    }
    return  code;
}
static inline std::string get_param_str(const std::string &str)
{
    int count = 0;
    for (size_t i = 0; i <str.size() ; ++i)
    {
        char ch = str[i];
        if (ch == ',')
        {
            if(count == 0)
                return str.substr(0, i);
        }
        else if(ch == ')')
        {
            return str.substr(0, i);
        }
        else if(ch == '<')
        {
            count ++;
        }
        else if(ch == '>')
        {
            count --;
        }
    }
    return str;
}

static inline std::string get_type_str(const std::string &str)
{
    std::string buffer(str);
    std::string const_str("const ");
    skip(buffer," \r\n\t");
    if (buffer.size() > const_str.size())
    {
        bool match = true;
        for (size_t i = 0; i < const_str.size(); i++)
        {
            if (buffer[i] != const_str[i])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            buffer = buffer.substr(const_str.size());
            skip(buffer, " \r\n\t");
        }
    }
    for (int i = (int)buffer.size() -1; i >= 0; --i)
    {
        char ch = buffer[i];
        if(ch == ' '|| ch == '&')
        {
            return buffer.substr(0, (size_t)( i - 1 ));
        }
        else if(ch == '>')
        {
            return buffer.substr(0, (size_t)i);
        }
    }
    return buffer;
}
lemon::field lemon::parse_param()
{
    lemon::field f;

    f.str_ = get_param_str(line_buffer_);
    f.type_str_ = get_type_str(f.str_);

    token_t t = get_next_token();
    
    if(t.type_ == token_t::e_const)
    {
        t = get_next_token();
    }
    if (t.type_ == token_t::e_std_string)
    {
        f.type_ = field::e_std_string;
    }
    else if(t.type_ == token_t::e_std_vector ||
            t.type_ == token_t::e_std_list)
    {
        f.type_ = field::e_vector;
        if(t.type_ == token_t::e_std_list)
            f.type_ = field::e_list;

        token_t t = get_next_token();
        if (t.type_ != token_t::e_less)
            throw syntax_error("not find <");
        int count = 1;
        do
        {
            t = get_next_token();
            if (t.type_ == token_t::e_less)
                count++;
            else if (t.type_ == token_t::e_gt)
            {
                count--;
                if (count == 0)
                    break;
            }
        } while (true);
    }
    //get name
    t = get_next_token();
    if (t.type_ == token_t::e_and)
        t = get_next_token();
    f.name_ = t.str_;
    t = get_next_token();
    if (t.type_ != token_t::e_close_paren&&
        t.type_ != token_t::e_comma)
        throw syntax_error("no find `,` or `)` ");
    return f;
}

lemon::field lemon::parse_return()
{
    token_t t = get_next_token();
    if (t.type_ != token_t::e_std_string)
        throw syntax_error("return value not std::string .error");
    field f;
    f.type_ = field::e_std_string;
    return f;
}
void lemon::parse_interface()
{
    token_t t;

    t.type_ = token_t::e_acl_string;
    template_.interface_.return_ = parse_return();
    template_.interface_.name_ = get_next_token().str_;
    t = get_next_token();
    if (t.type_ != token_t::e_open_paren)
        throw syntax_error("not find (");

    while(true)
    {
        template_.interface_.params_.push_back(parse_param());
        t = token_;
        if (t.type_ == token_t::e_close_paren)
        {
            t = get_next_token();
            if (t.type_ != token_t::e_comment_end)
                throw syntax_error("not find -->");
            return;
        }
            
    }
}
std::string lemon::get_iterator()
{
    char buffer[32];
    sprintf(buffer,"%d",++iterators_);
    return buffer;
}
std::string lemon::get_type(const std::string &str)
{
    for (size_t i = 0; i < stack_.size(); i++)
    {
        if (stack_[i].name_ == str)
            return stack_[i].type_;
    }
    return "";
}
static inline std::string get_next_type(const std::string &str)
{
    std::string buffer;

    int count = 0;

    for (size_t i = 0; i < str.size(); i++)
    {
        char ch = str[i];
        if(ch == '<')
        {
            count++;
            if (count == 1)
            {
                buffer.clear();
            }
        }
        else if(ch == '>')
        {
            buffer.push_back(ch);
            count--;
            if (count == 0)
                return buffer;
        }
        else
        {
            buffer.push_back(ch);
        }
    }
    return buffer;
}
#define br std::string("\r\n");

std::string lemon::parse_if()
{
    std::string code;
    std::string item = get_next_token().str_;
    code += "if("+item+")"+br;
    code += "{";

    token_t t = get_next_token();
    if (t.type_ != token_t::e_code_end)
        throw syntax_error("not find %}");

    code += "\"";
    do
    {
        t = get_next_token("");

        if (t.type_ == token_t::e_double_open_brace)
        {
            std::string name = get_next_token().str_;
            std::string type = get_type(name);
            if (type.empty())
                throw syntax_error("not find " + name);
            code += "code += " + name + ";";
            if (get_next_token().type_ != token_t::e_double_open_brace)
                throw syntax_error("not find }}");
        }
        else if (t.type_ == token_t::e_code_begin)
        {
            t = get_next_token();
            if (t.type_ == token_t::e_for)
            {
                code += parse_for();
            }
            else if (t.type_ == token_t::e_endfor)
            {
                throw syntax_error("error syntax :endif");
            }
            else if (t.type_ == token_t::e_if)
            {
                code += parse_if();
            }
            else if (t.type_ == token_t::e_elif)
            {
                code += "}" + br;
                code +="else ";
                code += parse_if();
                return code;
            }
            else if (t.type_ == token_t::e_endif)
            {
                code += "}" + br;
                return code;
            }
        }
        else if (t.type_ == token_t::e_$r)
        {
            t = get_next_token("");
            if (t.type_ == token_t::e_$n)
            {
                code += "\";" + br;
                code += "code += \"";
            }
            else
            {
                code += "\";" + br;
                code += "code += \"";
                code += t.str_;
            }
        }
        else if (t.type_ == token_t::e_$t)
        {
            code += "\t";
        }
        else
        {
            code += t.str_;
        }

    } while (true);
    return code;
}
std::string lemon::parse_for()
{
    std::string code;

    std::string item = get_next_token().str_;
    if (get_next_token().type_ != token_t::e_in)
        throw syntax_error("not find in ");
    std::string items = get_next_token().str_;
    std::string items_type = get_type(items);
    std::string item_type = get_next_type(items_type);
    if (items_type.empty())
        throw syntax_error("not find " + items);
    std::string it = get_iterator();
    code += items_type + "::const_iterator " + it + "= " + items + ".begin();";
    code += "for (; " + it + " != " + items + ".end(); ++" + it + ")" + br;
    code += "{" + br;
    code += "\t const " + item_type + " &" + item + " = *" + it + ";"+br;
    stack s;
    s.name_ = item;
    s.type_ = item_type;
    stack_.push_back(s);

    if (get_next_token().type_ != token_t::e_code_end)
        throw syntax_error("not find %}");

    token_t t = get_next_token("");
    do
    {
        if (t.type_ == token_t::e_double_open_brace)
        {
            std::string name = get_next_token().str_;
            std::string type = get_type(name);
            if (type.empty())
                throw syntax_error("not find "+name);
            code += "code += " + name + ";" + br;
            if(get_next_token().type_ != token_t::e_double_open_brace)
                throw syntax_error("not find }}");
        }
        else if (t.type_ == token_t::e_code_begin)
        {
            t = get_next_token();
            if (t.type_ == token_t::e_for)
            {
                code += parse_for();
            }
            else if (t.type_ == token_t::e_endfor)
            {
                if (get_next_token().type_ != token_t::e_double_open_brace)
                    throw syntax_error("not find }}");
                return code;
            }
            else if (t.type_ == token_t::e_if)
            {
                code += parse_if();
            }
        }
        else if (t.type_ == token_t::e_$r)
        {
            t = get_next_token("");
            if (t.type_ == token_t::e_$n)
            {
                code += "\";" + br;
                code += "code += \"";
            }
            else
            {
                code += "\";" + br;
                code += "code += \"";
                code += t.str_;
            }
        }
        else if (t.type_ == token_t::e_$t)
        {
            code += "\t";
        }
        else
        {
            code += t.str_;
        }

    } while (true);
    return code;
}
void lemon::parse_html()
{
    std::string code;
    code += "std::string code;\r\n";
    code += "code +=\"";
    do
    {
        token_t t = get_next_token("");
        if (t.type_ == token_t::e_eof)
        {
            std::cout << code;
        }
        else if (t.type_ == token_t::e_double_open_brace)
        {
            std::string name = get_next_token().str_;
            std::string type = get_type(name);
            if (type.empty())
                throw syntax_error("not find " + name);
            code += "code += " + name + " ;";
            if (get_next_token().type_ != token_t::e_double_close_brace)
                throw syntax_error("not find }}");
        }
        else if (t.type_ == token_t::e_code_begin)
        {
            t = get_next_token();
            if (t.type_ == token_t::e_for)
            {
                code += parse_for();
            }
            else if (t.type_ == token_t::e_if)
            {
                code += parse_if();
            }
        }
        else if (t.type_ == token_t::e_$r)
        {
            t = get_next_token("");
            if (t.type_ == token_t::e_$n)
            {
                code += "\";\r\n";
                code += "code += \"";
            }
            else
            {
                code += "\";\r\n";
                code += "code += \"";
                code += t.str_;
            }
        }
        else if (t.type_ == token_t::e_$t)
        {
            code += "\t";
        }
        else
        {
            code += t.str_;
        }
        
    } while (true);
}
void lemon::parse_template()
{

    token_t t = get_next_token();
    if (t.type_ != token_t::e_comment_begin)
        throw syntax_error("error not find template interface.");
    template_.interface_.str_ = get_string("-->");
    parse_interface();
    for (size_t i = 0; i < template_.interface_.params_.size(); i++)
    {
        field f = template_.interface_.params_[i];

        stack s;
        s.name_ = f.name_;
        s.type_ = f.type_str_;
        stack_.push_back(s);
    }
    parse_html();
}