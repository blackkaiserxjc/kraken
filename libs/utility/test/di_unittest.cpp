#include <boost/di.hpp>
#include <iostream>
#include <boost/test/unit_test.hpp>

namespace di = boost::di;

BOOST_AUTO_TEST_SUITE(Di)

BOOST_AUTO_TEST_CASE(basic_test) 
{
    struct renderer {
        int device;
    };

    class view {
    public:
        view(std::string_view, const renderer &) {}
    };

    class model {};

    class controller {
    public:
        controller(model &, view &){};
    };

    class user {};

    class app {
    public:
        app(controller &, user &){};
    };

    {
        renderer renderer_;
        view view_{"", renderer_};
        model model_;
        controller controller_{model_, view_};
        user user_;
        app app_{controller_, user_};
    }

    {
        auto injector = di::make_injector();
        injector.create<app>();
    }
}

BOOST_AUTO_TEST_CASE(bind_test)
{

}

BOOST_AUTO_TEST_SUITE_END()