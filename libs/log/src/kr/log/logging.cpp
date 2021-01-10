//
// Created by kaiser on 2021/1/2.
//

#include <thread>
#include <mutex>

#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>

#include "logging.h"

namespace kr {
namespace log {

struct init_attributes
{
    init_attributes()
    {
        init();
    }

    void init()
    {
        static std::once_flag flag;
        std::call_once(flag, []()
        {
            boost::log::register_simple_filter_factory<severity_level>("Severity");
            boost::log::register_simple_formatter_factory<severity_level, char>("Severity");
            boost::log::add_common_attributes();
        });
    }
} init_attributes;

logger::logger(const std::string &channel)
    : boost::log::sources::severity_channel_logger<severity_level, std::string>(boost::log::keywords::channel = channel)
{
    this->add_attribute("File", boost::log::attributes::mutable_constant<std::string>(""));
    this->add_attribute("Line", boost::log::attributes::mutable_constant<int>(0));
    this->add_attribute("Tag", boost::log::attributes::mutable_constant<std::string>(""));
    init_attributes.init();
}

logger& logger::get()
{
    static logger glog{"global"};
    return glog;
}

logger_mt::logger_mt(const std::string& channel)
    : boost::log::sources::severity_channel_logger_mt<severity_level, std::string>(boost::log::keywords::channel = channel)
{
    this->add_attribute("File", boost::log::attributes::mutable_constant<std::string>(""));
    this->add_attribute("Line", boost::log::attributes::mutable_constant<int>(0));
    this->add_attribute("Tag", boost::log::attributes::mutable_constant<std::string>(""));
    init_attributes.init();
}

logger_mt& logger_mt::get()
{
    static logger_mt glog{"global"};
    return glog;
}

}// log
}// kr
