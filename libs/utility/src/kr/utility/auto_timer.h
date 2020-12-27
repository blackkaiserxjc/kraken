#ifndef KR_COMMON_UTILITY_AUTO_TIMER_H_
#define KR_COMMON_UTILITY_AUTO_TIMER_H_

#include <chrono>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include <fmt/core.h>

namespace kr {
    namespace utility {
        // 默认日志
        enum class LoggerStyle {SECONDS, PRETTY};
        template <LoggerStyle>
        struct Logger;

        template <
                class Logger = Logger<LoggerStyle::SECONDS>,
                class Clock = std::chrono::high_resolution_clock>
        class auto_timer final {
        public:
            using DoubleSeconds = std::chrono::duration<double>;

            explicit auto_timer(
                    std::string &&msg = "",
                    const DoubleSeconds &min_time_to_log = DoubleSeconds::zero(),
                    Logger &&logger = Logger())
                : message_(std::move(msg)),
                  min_time_to_log_(min_time_to_log),
                  logger_(std::move(logger)) {}

            auto_timer(const auto_timer &) = delete;
            auto_timer(auto_timer &&) = default;
            auto_timer &operator=(const auto_timer &) = delete;
            auto_timer &operator=(auto_timer &&) = default;

            ~auto_timer() {
                if (message_) {
                    log(message_.value());
                }
            }

            DoubleSeconds log(std::string_view msg = "") {
                return log_impl(Clock::now(), msg);
            }

            template<typename... Args>
            DoubleSeconds log_format(Args &&...args) {
                auto now = Clock::now();
                return log_impl(now, fmt::format(std::forward<Args>(args)...));
            }

        private:
            DoubleSeconds log_impl(std::chrono::time_point<Clock> now, std::string_view msg) {
                auto duration = now - start_;
                if (duration >= min_time_to_log_) {
                    logger_(msg, duration);
                }
                start_ = Clock::now();
                return duration;
            }

            std::optional<std::string> message_;
            std::chrono::time_point<Clock> start_;
            DoubleSeconds min_time_to_log_;
            Logger logger_;
        };

        template<
                class Logger = Logger<LoggerStyle::SECONDS>,
                class Clock = std::chrono::high_resolution_clock>
        auto make_auto_timer(std::string &&msg = "",
                             const std::chrono::duration<double> &min_time_to_log = std::chrono::duration<double>::zero(),
                             Logger &&logger = Logger()) {
            return auto_timer<Logger, Clock>(std::move(msg), min_time_to_log, std::move(logger));
        }

        template <LoggerStyle Style>
        struct Logger final {
            void operator()(std::string_view msg, const std::chrono::duration<double>& sec) const {
                if (msg.empty()) {
                    return;
                }

                if (Style == LoggerStyle::PRETTY) {

                } else {
                    std::cout << "msg" << "in " << sec.count() << " seconds";
                }
            }
        };
    }// namespace utility
}// namespace kr

#endif
