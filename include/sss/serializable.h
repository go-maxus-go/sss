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
class serializable;

template<class backend_t>
using backend_impl_t = details::backend_wrapper<backend_t>;

enum class field_type { primitive, character, serializable, container };

template<field_type>
struct field_type_t {};

template<typename T, typename _ = void>
struct is_container_t : std::false_type {};

template<typename... Ts>
struct is_container_helper {};

template<typename T>
struct is_container_t<
        T,
        std::conditional_t<
            false,
            is_container_helper<
                typename T::value_type,
                typename T::size_type,
                typename T::allocator_type,
                typename T::iterator,
                typename T::const_iterator,
                decltype(std::declval<T>().size()),
                decltype(std::declval<T>().begin()),
                decltype(std::declval<T>().end()),
                decltype(std::declval<T>().cbegin()),
                decltype(std::declval<T>().cend())
                >,
            void
            >
        > : public std::true_type {};

template<class value_t, class backend_t>
struct get_field_type
{
    static constexpr field_type value()
    {
        constexpr auto is_string =
            std::is_same<value_t, std::string>::value ||
            std::is_same<value_t, std::wstring>::value ||
            std::is_same<value_t, std::u16string>::value ||
            std::is_same<value_t, std::u32string>::value;
        constexpr auto is_serializable = std::is_base_of<serializable<value_t, backend_t>, value_t>::value;
        constexpr auto is_character = std::is_same<char, value_t>::value;
        constexpr auto is_container = is_container_t<value_t>();

        if constexpr (is_string)
            return field_type::primitive;
        else if (is_character)
            return field_type::character;
        else if (is_serializable)
            return field_type::serializable;
        else if (is_container)
            return field_type::container;
        else
            return field_type::primitive;
    }
};

template<class value_t, class backend_t>
void serialize_impl(backend_impl_t<backend_t> & backend, const char * name, const value_t & value, field_type_t<field_type::primitive>)
{
    backend.add(name, value);
}

template<class value_t, class backend_t>
void serialize_impl(backend_impl_t<backend_t> & backend, const char * name, const value_t & value, field_type_t<field_type::character>)
{
    backend.add(name, static_cast<int>(value));
}

template<class value_t, class backend_t>
void serialize_impl(backend_impl_t<backend_t> & backend, const char * name, const value_t & value, field_type_t<field_type::serializable>)
{
    backend_impl_t<backend_t> object;
    value.serialize(object);
    backend.add_object(name, std::move(object));
}

template<class value_t, class backend_t>
void serialize_impl(backend_impl_t<backend_t> & backend, const char * name, const value_t & value, field_type_t<field_type::container>)
{
    backend.add_container(name, value);
}

template<class value_t, class backend_t, field_type>
struct field_deserializer;

template<class value_t, class backend_t>
struct field_deserializer<value_t, backend_t, field_type::primitive>
{
    static void deserialize(backend_impl_t<backend_t> & backend, const char * name, value_t & value)
    {
        value = backend.get(name);
    }
};

template<class value_t, class backend_t>
struct field_deserializer<value_t, backend_t, field_type::character>
{
    static void deserialize(backend_impl_t<backend_t> & backend, const char * name, value_t & value)
    {
        value = static_cast<char>(static_cast<int>(backend.get(name)));
    }
};

template<class value_t, class backend_t>
struct field_deserializer<value_t, backend_t, field_type::serializable>
{
    static void deserialize(backend_impl_t<backend_t> & backend, const char * name, value_t & value)
    {
        auto object = backend.get_object(name);
        value.deserialize(object);
    }
};

template<class value_t, class backend_t>
struct field_deserializer<value_t, backend_t, field_type::container>
{
    static void deserialize(backend_impl_t<backend_t> & backend, const char * name, value_t & value)
    {
        auto container = backend.get(name);
        value.clear();
        for (auto v : container)
            value.emplace(value.end(), v);
    }
};

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
        const auto this_tuple = details::to_tuple(*reinterpret_cast<T * const>(this));
        std::apply([this, &backend](auto & ...field) {
            (..., deserialize_field(backend, field->name(), field->value()));
        }, this_tuple);
    }

private:
    template<class value_t>
    void serialize_field(backend_impl_t & backend, const char * name, const value_t & value) const
    {
        constexpr auto type = get_field_type<value_t, backend_t>::value();
        serialize_impl(backend, name, value, field_type_t<type>());
    }

    template<class value_t>
    void deserialize_field(backend_impl_t & backend, const char * name, value_t & value) const
    {
        constexpr auto type = get_field_type<value_t, backend_t>::value();
        field_deserializer<value_t, backend_t, type>::deserialize(backend, name, value);
    }
};

} // namespace sss
