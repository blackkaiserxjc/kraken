#include <utility>

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>

#include <kr/network/http.h>

namespace kr {
namespace network {
namespace http {

router::router()
    : root_{}, route_{}
{
}

router::~router()
{
    root_.clear();
    route_.clear();
}

void router::add_route(const std::string &method, const std::string &path, Handler handler)
{
    auto parts = parse_path(path);
    if (!root_.contains(method))
    {
        root_[method] = std::make_shared<node>();
    }
    auto root = root_[method];
    std::string key(method + "-" + path);
    for (auto &&part : parts)
    {
        if (!root->children.contains(part))
        {
            root->children[part] = std::make_shared<node>("", part, part.front() == ':' || part.front() == '*');
        }
        root.swap(root->children[part]);
    }
    root->path = path;
    route_[key] = std::move(handler);
}

auto router::get_route(const std::string &method, const std::string &path)
    -> std::tuple<std::shared_ptr<node>, std::unordered_map<std::string, std::string>>
{
    std::unordered_map<std::string, std::string> params;
    if (!root_.contains(method))
    {
        return std::make_tuple(nullptr, params);
    }

    auto &&node = root_[method];
    auto search_parts = parse_path(path);
    for (auto iter = search_parts.begin(); iter != search_parts.end(); ++iter)
    {
        std::string temp;
        if (node != nullptr && node->children.size() > 0)
        {
            for (auto &&[_, child] : node->children)
            {
                if (child->part == *iter || child->is_wild)
                {
                    if (child->part.front() == '*')
                    {
                        std::vector<std::string> vec(iter, search_parts.end());
                        params[child->part.substr(1)] = boost::algorithm::join(vec, "/");
                    }
                    if (child->part.front() == ':')
                    {
                        params[child->part.substr(1)] = *iter;
                    }
                    temp = child->part;
                }
            }
        }
        if (temp.front() == '*')
        {
            return std::make_tuple(node->children[temp], params);
        }
        node.swap(node->children[temp]);
    }
    return std::make_tuple(node, params);
}

auto router::parse_path(const std::string &path)
    -> std::vector<std::string>
{
    std::vector<std::string> parts, result;
    boost::algorithm::split(parts, path, boost::is_any_of("/"));
    for (auto &&value : parts)
    {
        if (!value.empty())
        {
            result.emplace_back(value);
            if (value.front() == '*')
                break;
        }
    }
    return result;
}

} // namespace http
} // namespace network
} // namespace kr
