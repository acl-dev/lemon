#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

namespace lemon
{
    inline size_t length(const std::string &str)
    {
        return str.size();
    }

    template<class T>
    inline size_t length(const std::list<T> &obj)
    {
        return obj.size();
    }
}