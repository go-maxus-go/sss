#pragma once


template<class T>
void serialize_deserialize_check(const T & data)
{
    const auto init_data_text = data.to_string();

    auto restored_data = T();
    restored_data.from_string(init_data_text);

    REQUIRE(init_data_text == restored_data.to_string());
}