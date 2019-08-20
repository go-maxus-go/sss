#include <string>

#include <catch2/catch.hpp>

#include <sss/serializable.h>

namespace {

struct data : sss::json_serializable<data>
{
    SSS_FIELD(std::string, string_value) {""};
};

} // anonymous namespace

TEST_CASE("Empty string types to json", "[json_string_types]")
{
    auto init_data = data();

    const auto init_data_text = init_data.to_string();

    auto restored_data = data();
    restored_data.from_string(init_data_text);

    REQUIRE(init_data_text == restored_data.to_string());
}

TEST_CASE("String types to json", "[json_string_types]")
{
    auto init_data = data();
    init_data.string_value = "hello";

    const auto init_data_text = init_data.to_string();

    auto restored_data = data();
    restored_data.from_string(init_data_text);

    REQUIRE(init_data_text == restored_data.to_string());
}
