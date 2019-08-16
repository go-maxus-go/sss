#pragma once


namespace sss {

namespace details {

class json_backend;

} // namespace details

template<class T, class backend_t>
class serializable;

template<class T>
using json_serializable = serializable<T, details::json_backend>;

} // namespace sss
