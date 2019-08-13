#pragma once

#include <string>
#include <sstream>

#include <type_traits>

#include <functional>
#include <utility>
#include <tuple>

#include "details/to_tuple.hpp"
#include "details/json_backend.h"
#include "details/backend_wrapper.h"


namespace sss {

#define TO_STRING(str) #str
#define SSS_FIELD(type, name) \
    static inline const char __##name[] = TO_STRING(name); \
    sss::serializable_field<type, __##name> name

template<class T, const char * m_name>
class serializable_field
{
public:
    using type = T;

    serializable_field() = default;
    serializable_field(const T & value) : m_value(value)
    {}

    T & value() { return m_value; }
    const T & value() const { return m_value; }
    const char * name() const { return m_name; }

    operator T & ()
    {
        return m_value;
    }

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
        backend_impl_t backend;
        serialize(backend);
        return backend.to_string();
    }

    void serialize(backend_impl_t & backend) const
    {
        const auto this_tuple = details::to_tuple(*static_cast<const T*>(this));

        std::apply([this, &backend](auto & ...field) {
            (..., serialize_field(backend, field.name(), field.value()));
        }, this_tuple);
    }

    void from_string(const std::string & text)
    {
        backend_impl_t backend;
        backend.parse(text);
        deserialize(backend);
    }

    void deserialize(backend_impl_t & backend)
    {
        auto this_tuple = details::to_tuple(*static_cast<T const *>(this));

        std::apply([this, &backend](auto & ...field) {
            (..., deserialize_field(backend, field.name(), field.value()));
        }, this_tuple);

        auto data = std::tuple_cat(std::make_tuple(serializable<T, backend_t>()), std::move(this_tuple));
        *static_cast<T*>(this) = std::make_from_tuple<T>(std::move(data));
    }

private:
    template<class value_t>
    void serialize_field(backend_impl_t & backend, const char * name, const value_t & value) const
    {
        constexpr auto is_serializable = std::is_base_of<sss::serializable<value_t, backend_t>, value_t>::value;
        if constexpr (is_serializable) {
            backend_impl_t object;
            value.serialize(object);
            backend.add_object(name, std::move(object));
        }
        else
            backend.add(name, value);
    }

    template<class value_t>
    void deserialize_field(backend_impl_t & backend, const char * name, value_t & value) const
    {
        constexpr auto is_serializable = std::is_base_of<sss::serializable<value_t, backend_t>, value_t>::value;
        if constexpr (!is_serializable)
            value = backend.get<value_t>(name);
        //constexpr auto is_serializable = std::is_base_of<sss::serializable<value_t, backend_t>, value_t>::value;
        //if constexpr (is_serializable) {
        //    backend_impl_t object;
        //    value.serialize(object);
        //    backend.add_object(name, std::move(object));
        //}
        //else
        //    backend.add(name, value);
    }
};


template<class T>
using json_serializable = serializable<T, details::json_backend>;

} // namespace sss
