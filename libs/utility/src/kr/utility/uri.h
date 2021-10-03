//
// Created by kaiser on 2021/6/22.
//

#pragma once

#include <cctype>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace kr {
namespace utility {

/** uri解析类
 * The class for URI scheme : http://en.wikipedia.org/wiki/URI_scheme
 *
 *  foo://username:password@example.com:8042/over/there/index.dtb?type=animal&name=narwhal#nose
 *  \_/   \_______________/ \_________/ \__/            \___/ \_/ \______________________/ \__/
 *   |           |               |       |                |    |            |                |
 *   |       userinfo           host    port              |    |          query          fragment
 *   |    \________________________________/\_____________|____|/ \__/        \__/
 * scheme                 |                          |    |    |    |          |
 *                    authority                      |    |    |    |          |
 *                                                 path   |    |    interpretable as keys
 *                                                        |    |
 *        \_______________________________________________|____|/       \____/     \_____/
 *                             |                          |    |          |           |
 *                     hierarchical part                  |    |    interpretable as values
 *                                                        |    |
 *                                   interpretable as filename |
 *                                                             |
 *                                                             |
 * 示例 https://user:password@www.contoso.com:80/Home/Index.htm?q1=v1&q2=v2#FragmentName
 *
 * scheme: http
 * username: user
 * password: password
 * host: www.contoso.com
 * port: 80
 * path: /home/index/htm
 * query: q1=v1 q2=v2
 * fragment: FragmentName
 */
class uri
{
public:
    /**
     * 构造函数
     * @param sv 字符串
     */
    explicit uri(std::string_view str);

    /**
     * 获取scheme
     * @return scheme
     */
    std::string_view scheme() const
    {
        return scheme_;
    }

    /**
     * 获取username
     * @return username
     */
    std::string_view username() const
    {
        return username_;
    }

    /**
     * 获取password
     * @return password
     */
    std::string_view password() const
    {
        return password_;
    }

    /**
     * 获取host
     * @return host
     */
    std::string_view host() const
    {
        return host_;
    }

    /**
     * 获取port
     * @return port
     */
    uint16_t port() const
    {
        return port_;
    }

    /**
     * 获取path
     * @return path
     */
    std::string_view path() const
    {
        return path_;
    }

    /**
     * 获取query
     * @return query
     */
    std::string_view query() const
    {
        return query_;
    }

    /**
     * 获取fragment
     * @return fragment
     */
    std::string_view fragment() const
    {
        return fragment_;
    }

    /**
     * 获取hostname
     * @return hostname
     */
    std::string hostname() const;

    /**
     * 获取authority
     * @return authority
     */
    std::string authority() const;

    /**
     * 转换为字符串
     * @return uri string
     */
    std::string to_string() const;

    /**
     *  设置端口号
     * @param port 端口号
     */
    void set_port(uint16_t port)
    {
        has_authority_ = true;
        port_ = port;
    }

    /**
     * 获取query params
     * @return query params
     */
    const std::vector<std::pair<std::string, std::string>> &query_params();

private:
    friend class uri_builder;

    std::string scheme_;
    std::string username_;
    std::string password_;
    std::string host_;
    bool has_authority_;
    uint16_t port_;
    std::string path_;
    std::string query_;
    std::string fragment_;
    std::vector<std::pair<std::string, std::string>> query_params_;
};

} // namespace utility
} // namespace kr