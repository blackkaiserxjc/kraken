#include <boost/test/unit_test.hpp>

#include <kr/utility/ioc_container.h>

BOOST_AUTO_TEST_SUITE(IocTest)

struct Interface
{
    virtual void Func() = 0;
    virtual ~Interface(){};
};

struct Derivedb : public Interface
{
    void Func() override
    {
        std::cout << __FUNCTION__ << std::endl;
    }
};

struct Bus
{
    void Func() const
    {
        std::cout << __FUNCTION__ << std::endl;
    }
};

struct Car
{
    Car(int i, float f, std::string str)
        : i(i), f(f), str(std::move(str))
    {
    }

    void Func() const
    {
        std::cout << __FUNCTION__ << std::endl;
        std::cout << "i:" << i << " f:" << f << " str:" << str << std::endl;
    }
    int i;
    float f;
    std::string str;
};

BOOST_AUTO_TEST_CASE(Basic)
{
    {
        kr::utility::ioc_container ioc;
        ioc.bind<Derivedb>();
        ioc.bind<Bus>();
        auto derive = ioc.load<Derivedb>();
        derive->Func();
        auto bus = ioc.load<Bus>();
        bus->Func();
    }

    {
        kr::utility::ioc_container ioc;
        ioc.bind<Car>(42, 3.14, "hello world");
        auto car = ioc.load<Car>();
        car->Func();
    }
}
BOOST_AUTO_TEST_SUITE_END()