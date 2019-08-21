#include <string>

#include <catch2/catch.hpp>

#include <sss/serializable.h>

#include "serialize_deserialize_check.h"


namespace {

struct string_data : sss::json_serializable<string_data>
{
    SSS_FIELD(std::string, string_value) {""};
};

} // anonymous namespace

TEST_CASE("Empty string types to json", "[json_string_types]")
{
    const auto data = string_data();

    serialize_deserialize_check(data);
}

TEST_CASE("String types to json", "[json_string_types]")
{
    auto data = string_data();
    data.string_value = "hello";

    serialize_deserialize_check(data);
}
