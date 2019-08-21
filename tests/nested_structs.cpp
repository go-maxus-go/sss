#include <string>

#include <catch2/catch.hpp>

#include <sss/serializable.h>

#include "serialize_deserialize_check.h"


namespace {

struct empty_struct : sss::json_serializable<empty_struct>
{};

struct first_level_nesting : sss::json_serializable<first_level_nesting>
{
    SSS_FIELD(std::string, name);
    SSS_FIELD(empty_struct, empty);
};

struct second_level_nesting : sss::json_serializable<second_level_nesting>
{
    SSS_FIELD(first_level_nesting, name);
    SSS_FIELD(std::string, surname);
    SSS_FIELD(empty_struct, empty);
};

struct third_level_nesting : sss::json_serializable<third_level_nesting>
{
    SSS_FIELD(second_level_nesting, name_surname);
    SSS_FIELD(first_level_nesting, name);
    SSS_FIELD(int, age);
    SSS_FIELD(empty_struct, empty);
};

} // anonymous namespace

TEST_CASE("", "[json_nested_structs]")
{
    auto data = third_level_nesting();
    data.name_surname->name->name = "init_data.name_surname->name->name";
    data.name_surname->surname = "init_data.name_surname->surname";
    data.name->name = "init_data.name->name";
    data.age = 42;

    serialize_deserialize_check(data);
}