//
// Created by kaiser on 2021/1/1.
//

#include <iostream>

#include "auto_timer.h"

namespace kr {
namespace utility {

void logger::operator()(std::string_view msg, const std::chrono::duration<double> &sec) const {
    if (msg.empty()) {
        return ;
    }
    std::cout << "msg: " << msg << "in " << sec.count() << " seconds" << std::endl;
}
} // utility
} // kr
