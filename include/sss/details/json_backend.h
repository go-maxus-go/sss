#pragma once

#include <string>
#include <utility>
#include <algorithm>
#include <initializer_list>

#include <nlohmann/json.hpp>


namespace sss::details {

using json = nlohmann::json;

class json_backend
{
public:
    json_backend() = default;
    json_backend(json backend)
        : backend(std::move(backend))
    {}

    template<class field_t>
    void add(const char * field_name, const field_t & value)
    {
        backend[field_name] = value;
    }

    void add_object(const char * field_name, const json_backend & object)
    {
        backend[field_name] = object.backend;
    }

    template<class field_t>
    void add_container(const char * field_name, const field_t & value)
    {
        backend[field_name] = value;
    }

    std::string to_string(int indent) const
    {
        return backend.dump(indent);
    }

    void parse(const std::string & text)
    {
        backend = backend.parse(text);
    }

    auto get(const char * field_name)
    {
        return *backend.find(field_name);
    }

    auto get_object(const char * field_name)
    {
        return json_backend(*backend.find(field_name));
    }

private:
    json backend;
};

} //  namespace sss::details
