#include <cstdlib>
#include <fstream>
#include <string>
#include <optional>

#include <boost/program_options.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <sol/sol.hpp>

#include <kr/log/logging.h>

int main(int argc, char *argv[])
{
    boost::program_options::options_description desc("Alloed options");
    desc.add_options()
        ("help", "help message.")
        ("config", boost::program_options::value<std::string>()->default_value("etc/server.app.lua"), "server config file path");

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

    if (std::optional<std::string> log_conf = config["log_conf"]; log_conf)
    {
        std::ifstream settings(*log_conf);
        boost::log::init_from_stream(settings);
    }

    kr::log::logger_mt logger("server");
    KRLOG_DEBUG(logger, "main") << "logger init.";
    return EXIT_SUCCESS;
}