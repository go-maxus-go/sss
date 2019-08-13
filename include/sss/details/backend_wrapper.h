#pragma once

#include <string>
#include <utility>


namespace sss::details {

template<class backend_t>
class backend_wrapper
{
public:
    template<class field_t>
    void add(const char * field_name, field_t && value)
    {
        backend.add(field_name, std::forward<field_t>(value));
    }

    void add_object(const char * field_name, backend_wrapper<backend_t> && value)
    {
        backend.add_object(field_name, std::forward<backend_t>(value.backend));
    }

    std::string to_string() const
    {
        return backend.to_string();
    }

private:
    backend_t backend;
};

} //  namespace sss::details
