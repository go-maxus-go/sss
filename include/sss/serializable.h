#pragma once

#include <string>
#include <sstream>

#include <type_traits>

#include <functional>
#include <utility>

#include "details/to_tuple.hpp"
#include "details/json_backend.h"
#include "details/backend_wrapper.h"


namespace sss {

#define TO_STRING(str) #str
#define SSS_FIELD(type, name) \
    static inline const char __##name[] = TO_STRING(name); \
    sss::SerializableField<type, __##name> name

template<class T, const char * m_name>
class SerializableField
{
public:
    using type = T;

    SerializableField() = default;
    SerializableField(const T & value) : m_value(value)
    {}

    T & value() { return m_value; }
    const T & value() const { return m_value; }
    const char * name() const { return m_name; }
private:
    T m_value;
};

template<class T, class backend_t>
class serializable
{
    using backend_impl_t =  details::backend_wrapper<backend_t>;
public:
    std::string to_string() const
    {
        details::backend_wrapper<backend_t> backend;
        const auto this_tuple = details::to_tuple(*static_cast<const T*>(this));

        std::apply([this, &backend](auto & ...field) {
            (..., serialize_field(backend, field.name(), field.value()));
        }, this_tuple);

        return backend.to_string();
    }

private:
    template<class value_t>
    void serialize_field(backend_impl_t & backend, const char * name, const value_t & value) const
    {
        constexpr auto is_serializable = std::is_base_of<sss::serializable<value_t, backend_t>, value_t>::value;
        if constexpr (is_serializable)
            backend.add(name, value.to_string());
        else
            backend.add(name, value);
    }
};


template<class T>
using json_serializable = serializable<T, details::json_backend>;

} // namespace sss
