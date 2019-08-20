#pragma once

#include <utility>
#include <initializer_list>


namespace sss::details {

template<class T, const char * m_name>
class serializable_field
{
public:
    using type = T;

    serializable_field() = default;
    serializable_field(const T & value)
        : m_value(value)
    {}

    T & value() { return m_value; }
    const T & value() const { return m_value; }
    const char * name() const { return m_name; }

    operator T & () { return m_value; }
    operator const T & () const { return m_value; }

    T * operator -> () { return &m_value; }
    const T * operator -> () const { return &m_value; }

    serializable_field & operator = (const T & value)
    {
        if (&m_value == &value)
            return *this;
        m_value = value;
        return *this;
    }

private:
    T m_value;
};

} // namespace sss::details
