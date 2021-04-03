//
// Created by kaiser on 2021/1/1.
//

#include <iostream>

#include <kr/utility/string.h>

#include "auto_timer.h"

namespace kr {
namespace utility {

logger::logger(): logger_("TimeProfiler")
{
}

void logger::operator()(std::string_view msg, const std::chrono::duration<double> &sec) const
{
    if (msg.empty())
    {
        return;
    }
    KRLOG_DEBUG(logger_, "TimeProfiler") << "msg: " << msg << " in " << pretty_print(sec.count(), PrettyType::PRETTY_TIME, false);
}

} // utility
} // kr
