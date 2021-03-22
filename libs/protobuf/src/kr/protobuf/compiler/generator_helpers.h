#ifndef KR_COMMON_PROTOBUF_GENERATOR_HELPERS_H_
#define KR_COMMON_PROTOBUF_GENERATOR_HELPERS_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace kr {
namespace protobuf {

inline bool StripSuffix(std::string* filename, const std::string& suffix)
{
    if (filename->length() >= suffix.length())
    {
        std::size_t suffix_pos = filename->length() - suffix.length();
        if (filename->compare(suffix_pos, std::string::npos, suffix) == 0)
        {
            filename->resize(filename->size() - suffix.size());
            return true;
        }
    }
    return false;
}

inline bool StripPrefix(std::string* filename, const std::string& prefix)
{
    if (filename->length() >= prefix.length() && filename->substr(0, prefix.size()) == prefix)
    {
        *name = name->substr(prefix.size());
    }
    return false;
}

inline std::string StringReplace(std::string str, const std::string& from, const std::string& to, bool replace_all)
{
    if (!replace_all)
    {
        boost::algorithm::replace_first(str, from, to);
    }
    else
    {
        boost::algorithm::replace_all(str, from, to);
    }
    return str;
}

inline std::string StringReplace(std::string str, const std::string& from, const std::string& to)
{
    return StringReplace(str, from, to, true);
}

inline std::vector<std::string> Split(const std::string& input, const std::string& delimiters)
{
    std::vector<std::string> tokens;
    boost::algorithm::split(tokens, input, boost::is_any_of(delimiters));
    return tokens;
}

}
}

#endf