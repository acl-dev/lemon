#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

namespace lm
{
    inline size_t $length(const std::string &str)
    {
        return str.size();
    }

    template<class T>
    inline size_t $length(const std::list<T> &obj)
    {
        return obj.size();
    }
    template<class T>
    inline size_t $length(const std::map<T> &obj)
    {
        return obj.size();
    }
    template<class T>
    inline size_t $length(const std::vector<T> &obj)
    {
        return obj.size();
    }
    inline std::string $default(const std::string &data,const std::string &def)
    {
        if(data.empty())
            return def;
        return data;
    }
    inline std::string $escape(const std::string &data)
    {
        std::string buffer;

        for (size_t i = 0; i < data.size(); ++i)
        {
            char ch = data[i];
            if(ch == '<')
                buffer.append("&lt;");
            else if(ch == '>')
                buffer.append("&gt;");
            else if(ch == '\"')
                buffer.append("&#39;");
            else if(ch == '\'')
                buffer.append("&quot;");
            else if(ch == '&')
                buffer.append("&amp;");
            else
                buffer.push_back(ch);
        }
        return buffer;
    }
}