//
// Created by kaiser on 2021/3/7.
//

#include <google/protobuf/compiler/plugin.h>
#include <kr/protobuf/compiler/lua_plugin.h>

int main(int argc, char *argv[])
{
    kr::protobuf::LuaGenerator generator;
    return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}