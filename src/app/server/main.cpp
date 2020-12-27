#include <iostream>
#include <memory>

int main() {
    auto deleter = [](auto&& obj)
    {
        std::cout << "11111111111111" << std::endl;
        delete obj;
    };
    std::unique_ptr<int, decltype(deleter)> vptr{new int(110)};
    return 0;
}
