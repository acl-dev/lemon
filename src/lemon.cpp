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

}

bool lemon::parse_template(const std::string &file_path)
{
    if (!file_.open_read(file_path.c_str()))
        return;
    try
    {
        parse_template();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
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
std::string lemon::get_string(int len)
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
lemon::token_t lemon::get_next_token()
{
    token_t t;
    std::string str;
    if (tokens_.size())
    {
        t = tokens_.front();
        tokens_.pop_front();
        str = t.str_;
    }
    else
    {
        std::string dilimiters = " <>{}()[]%!?:,\\/.\r\t\n\"'`=-";
        str = next_token(dilimiters);
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
        }
        else if (get_string(1) == "%")
        {
            t.type_ = token_t::e_code_begin;
            t.str_  = "{%";
        }
    }
    else if (str == "}")
    {
        t.type_ = token_t::e_close_brace;
        if (get_string(1) == "}")
        {
            t.type_ = token_t::e_double_close_quote;
            t.str_ = "}}";
        }
        else if (get_string(1) == "%")
        {
            t.type_ = token_t::e_code_end;
            t.str_ = "%}";
        }
    }
    else if (str == "/")
    {
        t.type_ = token_t::e_forward_slash;
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

lemon::field lemon::parse_param()
{
    lemon::field f;

    std::string str1 = get_string(",");
    std::string str2 = get_string(")");
    f.str_ = str1;
    if (str1.size() > str2.size())
        f.str_ = str2;

    token_t t = get_next_token();
    if(t.type_ == token_t::e_const)
        t = get_next_token();
    if (t.type_ == token_t::e_std_string)
        f.type_ = field::e_string;
    else if (t.type_ == token_t::e_std_list)
    {

    }
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

    if (get_next_token().type_ != token_t::e_close_paren)
        throw syntax_error("not find (");

    while(t.type_ != token_t::e_comment_end)
    {

    }
}
void lemon::parse_template()
{

    token_t t = get_next_token();
    if (t.type_ != token_t::e_comment_begin)
        throw syntax_error("error not find template interface.");
    std::string str = get_string("-->");

    do
    {
        

    } while (true);
}