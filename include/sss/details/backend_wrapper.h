#pragma once

#include <string>
#include <utility>


namespace sss::details {

template<class backend_t>
class backend_wrapper
{
public:
    backend_wrapper() = default;
    backend_wrapper(backend_t backend)
        : backend(std::move(backend))
    {}

    template<class field_t>
    void add(const char * field_name, field_t && value)
    {
        backend.add(field_name, std::forward<field_t>(value));
    }

    void add_object(const char * field_name, backend_wrapper<backend_t> && value)
    {
        backend.add_object(field_name, std::forward<backend_t>(value.backend));
    }

    void parse(const std::string & text)
    {
        backend.parse(text);
    }

    std::string to_string(int indent) const
    {
        return backend.to_string(indent);
    }

    auto get(const char * field_name)
    {
        return backend.get(field_name);
    }

    backend_wrapper<backend_t> get_object(const char * field_name)
    {
        return backend_wrapper<backend_t>(backend.get(field_name));
    }

private:
    backend_t backend;
};

} //  namespace sss::details
