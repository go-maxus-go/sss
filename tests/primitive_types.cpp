#include <catch2/catch.hpp>

#include <sss/serializable.h>

#include "serialize_deserialize_check.h"


namespace {

struct empty_struct : sss::json_serializable<empty_struct>
{};

struct integral_data : sss::json_serializable<integral_data>
{
    SSS_FIELD(int          , int_value          );
    SSS_FIELD(long         , long_value         );
    SSS_FIELD(bool         , bool_value         );
    SSS_FIELD(char         , char_value         );
    SSS_FIELD(wchar_t      , wchar_value        );
    SSS_FIELD(short        , short_value        );
    SSS_FIELD(long long    , long_long_value    );
    SSS_FIELD(float        , float_value        );
    SSS_FIELD(double       , double_value       );
    SSS_FIELD(long double  , long_double_value  );
    SSS_FIELD(long long int, long_long_int_value);

    SSS_FIELD(unsigned int          , unsigned_int_value          );
    SSS_FIELD(unsigned long         , unsigned_long_value         );
    SSS_FIELD(unsigned char         , unsigned_char_value         );
    SSS_FIELD(unsigned long long    , unsigned_long_long_value    );
    SSS_FIELD(unsigned long long int, unsigned_long_long_int_value);
};

} // anonymous namespace

TEST_CASE("Empty serializable struct is not an error", "[json_primitive_types]")
{
    const auto empty = empty_struct();
    serialize_deserialize_check(empty);
}

TEST_CASE("Primitive types to json", "[json_primitive_types]")
{
    auto init_data = integral_data();

    init_data.int_value           = 0;
    init_data.long_value          = 1;
    init_data.bool_value          = true;
    init_data.char_value          = 3;
    init_data.wchar_value         = 33;
    init_data.short_value         = 4;
    init_data.long_long_value     = 5;
    init_data.float_value         = 6;
    init_data.double_value        = 7;
    init_data.long_double_value   = 8;
    init_data.long_long_int_value = 88;

    init_data.unsigned_int_value           = 9;
    init_data.unsigned_long_value          = 10;
    init_data.unsigned_char_value          = 11;
    init_data.unsigned_long_long_value     = 12;
    init_data.unsigned_long_long_int_value = 122;

    serialize_deserialize_check(init_data);
}

TEST_CASE("Negative values to json", "[json_primitive_types]")
{
    auto init_data = integral_data();

    init_data.int_value           = -1;
    init_data.long_value          = -2;
    init_data.bool_value          = false;
    init_data.char_value          = -3;
    init_data.wchar_value         = static_cast<wchar_t>(-33);
    init_data.short_value         = -4;
    init_data.long_long_value     = -5;
    init_data.float_value         = -6.1f;
    init_data.double_value        = -7.2;
    init_data.long_double_value   = -8.3;
    init_data.long_long_int_value = -88;

    init_data.unsigned_int_value           = 9;
    init_data.unsigned_long_value          = 10;
    init_data.unsigned_char_value          = 11;
    init_data.unsigned_long_long_value     = 12;
    init_data.unsigned_long_long_int_value = 122;

    serialize_deserialize_check(init_data);
}
