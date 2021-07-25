#include <algorithm>
#include <atomic>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <thread>

#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/program_options.hpp>
#include <sol/sol.hpp>

#include <kr/log/logging.h>
#include <kr/utility/string.h>
#include <kr/utility/uri.h>

int main(int argc, char *argv[])
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()("help", "help message.")("config", boost::program_options::value<std::string>()->default_value("etc/server.app.lua"), "server config file path");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }

    // 配置读取
    auto config_path = vm["config"].as<std::string>();
    sol::state config;
    config.open_libraries();
    if (auto result = config.do_file(config_path); !result.valid())
    {
        std::cout << "load lua config error:" << result.get<sol::error>().what() << std::endl;
        return EXIT_FAILURE;
    }

    // 日志配置
    if (std::optional<std::string> log_conf = config["log_conf"]; log_conf)
    {
        std::ifstream settings(*log_conf);
        boost::log::init_from_stream(settings);
    }

    // 服务配置
    auto address = config["address"].get<std::string>();
    auto thread_count = config["thread"].get<int>();

    // 地址解析
    auto to_endpoint = [](std::string_view address) {
        kr::utility::uri uri(address);
        return boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(uri.host().data()),
            boost::lexical_cast<std::uint16_t>(uri.port()));
    };

    // 主线程logger
    kr::log::logger logger{"Service"};
    KRLOG_INFO(logger, "Main") << "Service Start.";
    // 调度器
    boost::asio::io_context io_context;

    // 信号处理
    boost::asio::signal_set signal_set{io_context};
#if defined(SIGUSR1)
    signal_set.add(SIGUSR1);
#endif
#if defined(SIGUSR2)
    signal_set.add(SIGUSR2);
#endif

    std::function<void(const boost::system::error_code &error, int signal_number)> signal_handler;
    signal_handler = [&](const boost::system::error_code &error, int signal_number) {
#if defined(SIGUSR1)
        if (signal_number == SIGUSR1)
        {
            KRLOG_INFO(logger, "Signal") << "Siguser1";
            io_context.stop();
        }
#endif

#if defined(SIGUSR2)
        // TODO: 预留信号: 配置重读,服务存盘
        if (signal_number == SIGUSR2)
        {
            KRLOG_INFO(logger, "Signal") << "Signal2";
        }
#endif
        signal_set.async_wait(signal_handler);
    };
    signal_set.async_wait(signal_handler);

    // 开启服务
    std::vector<std::thread> threads;
    std::generate_n(std::back_insert_iterator(threads), thread_count, [&io_context]() {
        return std::thread([&io_context] { io_context.run(); });
    });
    for (auto &&thread : threads)
    {
        thread.join();
    }
    return EXIT_SUCCESS;
}