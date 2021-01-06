//
// Created by kaiser on 2021/1/6.
//

#include <boost/test/unit_test.hpp>

#include <sstream>

#include <boost/core/null_deleter.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>

#include <kr/log/logging.h>

BOOST_AUTO_TEST_SUITE(Logging)

    struct LogSinkFixture
    {
        using LogBackend = boost::log::sinks::text_ostream_backend;
        using LogFrontend = boost::log::sinks::synchronous_sink<LogBackend>;

        LogSinkFixture() : logger("Unittest")
        {
            auto backend = boost::make_shared<LogBackend>();
            backend->auto_flush(true);
            backend->add_stream(boost::shared_ptr<std::ostream>(&sstrm, boost::null_deleter()));
            sink = boost::make_shared<LogFrontend>(backend);
            sink->set_formatter(
                    boost::log::expressions::stream
                            << "[" << boost::log::expressions::attr<std::string>("File") << ":" << boost::log::expressions::attr<int>("Line") << "]"
                            << " <" << boost::log::expressions::attr<kr::log::severity_level>("Severity") << ">"
                            << " <" << boost::log::expressions::attr<std::string>("Tag") << ">"
                            << " " << boost::log::expressions::message
            );
            boost::log::core::get()->add_sink(sink);
        }

        ~LogSinkFixture()
        {
            boost::log::core::get()->remove_sink(sink);
        }

        kr::log::logger logger;
        std::stringstream sstrm;
        boost::shared_ptr<LogFrontend> sink;
    };

    BOOST_FIXTURE_TEST_CASE(attributes, LogSinkFixture)
    {
        KRLOG_DEBUG(logger, "nn.core") << "hello";
        auto logText = sstrm.str();

        BOOST_CHECK_NE(logText.find("logging_unittest.cpp"), std::string::npos);
        BOOST_CHECK_NE(logText.find("47"), std::string::npos);
        BOOST_CHECK_NE(logText.find("DEBUG"), std::string::npos);
        BOOST_CHECK_NE(logText.find("kr.core"), std::string::npos);
        BOOST_CHECK_NE(logText.find("hello"), std::string::npos);
    }

    BOOST_FIXTURE_TEST_CASE(severity, LogSinkFixture)
    {
        std::string logText;

        sstrm.str("");
        KRLOG_TRACE(logger, "kr.core") << "hello";
        logText = sstrm.str();
        BOOST_CHECK_NE(logText.find("TRACE"), std::string::npos);

        sstrm.str("");
        KRLOG_DEBUG(logger, "kr.core") << "hello";
        logText = sstrm.str();
        BOOST_CHECK_NE(logText.find("DEBUG"), std::string::npos);

        sstrm.str("");
        KRLOG_INFO(logger, "kr.core") << "hello";
        logText = sstrm.str();
        BOOST_CHECK_NE(logText.find("INFO"), std::string::npos);

        sstrm.str("");
        KRLOG_WARN(logger, "kr.core") << "hello";
        logText = sstrm.str();
        BOOST_CHECK_NE(logText.find("WARN"), std::string::npos);

        sstrm.str("");
        KRLOG_ERROR(logger, "kr.core") << "hello";
        logText = sstrm.str();
        BOOST_CHECK_NE(logText.find("ERROR"), std::string::npos);
    }


BOOST_AUTO_TEST_SUITE_END()

