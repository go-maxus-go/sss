#pragma once

#include <tuple>
#include <string>
#include <utility>
#include <functional>
#include <type_traits>

#include "fwd.h"

#include "details/to_tuple.hpp"
#include "details/json_backend.h"
#include "details/backend_wrapper.h"
#include "details/serializable_field.h"
#include "details/serializable_field_macro.h"


namespace sss {

template<class T, class backend_t>
class serializable
{
    using backend_impl_t = details::backend_wrapper<backend_t>;
public:
    std::string to_string(int indent = 4) const
    {
        backend_impl_t backend;
        serialize(backend);
        return backend.to_string(indent);
    }

    void serialize(backend_impl_t & backend) const
    {
        auto this_tuple = details::to_tuple(*static_cast<const T*>(this));

        std::apply([this, &backend](auto & ...field) {
            (..., serialize_field(backend, field->name(), field->value()));
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
            (..., deserialize_field(backend, field->name(), field->value()));
        }, this_tuple);
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
        if constexpr (is_serializable)
        {
            auto object = backend.get_object(name);
            value.deserialize(object);
        }
        else
            value = backend.get(name);
    }
};

} // namespace sss
