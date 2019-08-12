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

    std::string to_string() const
    {
        return backend.dump(4);
    }

private:
    json::json backend;
};

} //  namespace sss::details
