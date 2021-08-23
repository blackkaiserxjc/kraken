//
// Created by kaiser on 2021/8/22.
//

#include <boost/test/unit_test.hpp>
#include <kr/network/http.h>

BOOST_AUTO_TEST_SUITE(HttpTest)

BOOST_AUTO_TEST_CASE(RouterTest)
{
    using namespace kr::network;

    auto r = std::make_shared<http::router>();
    r->add_route("GET", "/hello/:name", [](auto &&...) {});
    r->add_route("GET", "/hello/:name/profile", [](auto &&...) {});
    r->add_route("GET", "/hello/a/b", [](auto &&...) {});
    r->add_route("GET", "/hi/:name", [](auto &&...) {});
    r->add_route("GET", "/asserts/*filepath", [](auto &&...) {});

    auto &&[n1, params1] = r->get_route("GET", "/hello/world");
    if (n1 != nullptr)
    {
        std::cout << n1->path << std::endl;
    }

    auto &&[n2, params2] = r->get_route("GET", "/hello/a");
    if (n2 != nullptr)
    {
        std::cout << n2->path << std::endl;
    }
}

BOOST_AUTO_TEST_SUITE_END()