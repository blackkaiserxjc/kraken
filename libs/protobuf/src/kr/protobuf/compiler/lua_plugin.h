#ifndef KR_COMMON_PROTOBUF_LUA_PLUGIN_H_
#define KR_COMMON_PROTOBUF_LUA_PLUGIN_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/descriptor.h>

namespace kr {
namespace protobuf {

/** Lua 生成器 */
class LuaGenerator : public google::protobuf::compiler::CodeGenerator
{
public:
    LuaGenerator() = default;
    ~LuaGenerator() override = default;

    bool Generate(const google::protobuf::FileDescriptor *file,
        const std::string &parameter,
        google::protobuf::compiler::GeneratorContext *context,
        std::string *error) const override
    {
        return true;
    }
};

} // namespace protobuf
} // namespace kr
#endif //KR_COMMON_PROTOBUF_LUA_PLUGIN_H_
