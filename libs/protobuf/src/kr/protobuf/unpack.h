#pragma once

namespace kr {
namespace protobuf {

// read handler concept
template <typename Layer>
class unpacker
{
public:
    template <typename T>
    T as() const;

    template <typename T, typename Action, typename Expect>
    void to(Action &&action, Expect &&expect);
};

} // namespace protobuf
} // namespace kr