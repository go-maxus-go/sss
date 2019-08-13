#pragma once

#include <string>
#include <utility>
#include <sstream>
#include <iomanip>

#include <nlohmann/json.hpp>


namespace sss::details {

namespace json = nlohmann;

class json_backend
{
public:
    template<class field_t>
    void add(const char * field_name, const field_t & value)
    {
        backend[field_name] = value;
    }

    void add_object(const char * field_name, const json_backend & object)
    {
        backend[field_name] = object.backend;
    }

    std::string to_string() const
    {
        return backend.dump(4);
    }

    void parse(const std::string & text)
    {
        backend = backend.parse(text);
    }

    template<class field_t>
    field_t get(const char * field_name)
    {
        return *backend.find<field_t>(field_name);
    }

private:
    json::json backend;
};

} //  namespace sss::details
