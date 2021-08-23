#pragma once

#include <algorithm>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace kr {
namespace network {
namespace http {
class request
{
};
class response
{
};

using Handler = std::function<void(const request &, response &)>;

class router
{
public:
    struct node
    {
        explicit node(std::string path = "", std::string part = "", bool is_wild = false)
            : path(std::move(path)), part(std::move(part)), is_wild(is_wild)
        {
        }
        std::string path;
        std::string part;
        std::unordered_map<std::string, std::shared_ptr<node>> children;
        bool is_wild;
    };

    router();
    ~router();

    void add_route(const std::string &method, const std::string &path, Handler callback);
    auto get_route(const std::string &method, const std::string &path)
        -> std::tuple<std::shared_ptr<node>, std::map<std::string, std::string>>;

private:
    static auto parse_path(const std::string &path) -> std::vector<std::string>;

    std::unordered_map<std::string, std::shared_ptr<node>> root_;
    std::unordered_map<std::string, Handler> route_;
};

} // namespace http
} // namespace network
} // namespace kr