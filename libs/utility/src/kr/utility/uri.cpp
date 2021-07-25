//
// Created by kaiser on 2021/6/22.
//

#include <algorithm>
#include <regex>

#include <boost/lexical_cast.hpp>
#include <fmt/format.h>

#include <kr/core/core.h>
#include <kr/utility/uri.h>

namespace kr {
namespace utility {

namespace {

std::string submatch(const std::cmatch &m, int idx)
{
    const auto &sub = m[idx];
    return std::string(sub.first, sub.second);
}

} // namespace

uri::uri(std::string_view str)
    : has_authority_(false), port_(0)
{
    static const std::regex uri_regex(
        "([a-zA-Z][a-zA-Z0-9+.-]*):" // scheme:
        "([^?#]*)" // authority and path
        "(?:\\?([^#]*))?" // ?query
        "(?:#(.*))?"); // #fragment
    static const std::regex auth_path_regex("//([^/]*)(/.*)?");

    std::cmatch match;
    if (UNLIKELY(!std::regex_match(str.begin(), str.end(), match, uri_regex)))
    {
        throw std::invalid_argument(fmt::format("invaild uri {}", str));
    }

    scheme_ = submatch(match, 1);
    std::transform(scheme_.begin(), scheme_.end(), scheme_.begin(), ::tolower);

    std::string_view auth_path(match[2].first, match[2].second);
    std::cmatch auth_path_match;
    if (!std::regex_match(auth_path.begin(), auth_path.end(), auth_path_match, auth_path_regex))
    {
        has_authority_ = false;
        path_ = auth_path.data();
    }
    else
    {
        static const std::regex auth_regex(
            "(?:([^@:]*)(?::([^@]*))?@)?"
            "(\\[[^\\]]*\\]|[^\\[:]*)"
            "(?::(\\d*))?");

        const auto auth = auth_path_match[1];
        std::cmatch auth_match;
        if (!std::regex_match(
                auth.first,
                auth.second,
                auth_match,
                auth_regex))
        {
            throw std::invalid_argument(fmt::format("invalid uri authority {} {}", auth.first, auth.second));
        }

        std::string_view port(auth_match[4].first, auth_match[4].second);
        if (!port.empty())
        {
            try
            {
                port_ = boost::lexical_cast<uint16_t>(port);
            }
            catch (const boost::bad_lexical_cast &e)
            {
                throw std::invalid_argument(fmt::format("invalid uri port: ", e.what()));
            }
        }

        has_authority_ = true;
        username_ = submatch(auth_match, 1);
        password_ = submatch(auth_match, 2);
        host_ = submatch(auth_match, 3);
        path_ = submatch(auth_path_match, 2);
    }

    query_ = submatch(match, 3);
    fragment_ = submatch(match, 4);
}

std::string uri::authority() const
{
    std::string result;
    result.reserve(host().size() + username().size() + password().size() + 8);

    if (!username().empty() || !password().empty())
    {
        result.append(username());
        if (!password().empty())
        {
            result.push_back(':');
            result.append(password());
        }
        result.push_back('@');
    }
    result.append(host());

    if (port() != 0)
    {
        result.append(fmt::format(":{}", port()));
    }
    return result;
}

std::string uri::to_string() const
{
    std::string str;
    if (has_authority_)
    {
        str += fmt::format("{}://", scheme_);
        if (!password_.empty())
        {
            str += fmt::format("{}:{}@", username_, password_);
        }
        else if (!username_.empty())
        {
            str += fmt::format("{}@", username_);
        }
        str += host_;
        if (port_ != 0)
        {
            str += fmt::format(":{}", port_);
        }
    }
    else
    {
        str += fmt::format("{}:", scheme_);
    }
    str += path_;
    if (!query_.empty())
    {
        str += fmt::format("?{}", query_);
    }
    if (!fragment_.empty())
    {
        str += fmt::format("#{}", fragment_);
    }
    return str;
}

std::string uri::hostname() const
{
    if (!host_.empty() && host_[0] == '[')
    {
        return host_.substr(1, host_.size() - 2);
    }
    return host_;
}

const std::vector<std::pair<std::string, std::string>> &uri::query_params()
{
    if (!query_.empty() && query_params_.empty())
    {
        static const std::regex query_param_regex(
            "(^|&)"
            "([^=&]*)=?"
            "([^=&]*)"
            "(?=(&|$))");

        const std::cregex_iterator param_begin_iter(
            query_.data(), query_.data() + query_.size(), query_param_regex);
        std::cregex_iterator param_end_iter;
        for (auto iter = param_begin_iter; iter != param_end_iter; ++iter)
        {
            if (iter->length(2) == 0)
            {
                continue;
            }
            query_params_.emplace_back(
                std::string((*iter)[2].first, (*iter)[2].second),
                std::string((*iter)[3].first, (*iter)[3].second));
        }
    }
    return query_params_;
}

} // namespace utility
} // namespace kr
