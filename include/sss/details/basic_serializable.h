#pragma once

#include "to_tuple.hpp"


namespace Strict {

template<class T, const char * m_name>
class SerializableField
{
public:
    SerializableField() = default;
    SerializableField(const T & value) : m_value(value)
    {}

    T & value() { return m_value; }
    const T & value() const { return m_value; }
    const char * name() const { return m_name; }
private:
    T m_value;
};

#define TO_STRING(str) #str
#define SERIALIZABLE_FIELD(type, name) \
static inline const char _##name[] = TO_STRING(name); \
SerializableField<type, _##name> name


template<class T>
class Serializable
{
public:
    std::ostream & operator<<(std::ostream & os) const
    {
        auto thisTuple = to_tuple(*static_cast<const T*>(this));
        std::apply([this, &os](auto& ...field) {(..., serializeField(os, field)); }, thisTuple);
        return os;
    }

private:
    template<class Field>
    void serializeField(std::ostream & os, const Field & field) const
    {
        os << "(" << field.name() << ": " << field.value() << ")";
    }
};

template<class T>
std::ostream & operator << (std::ostream & os, const Serializable<T> & serializable)
{
    serializable.operator<<(os);
    return os;
}

} // namespace Strict
