#pragma once


#ifndef SSS_TO_STRING
#define SSS_TO_STRING(str) #str
#endif

#define SSS_FIELD(type, name) \
    static inline const char __##name[] = SSS_TO_STRING(name); \
    sss::details::serializable_field<type, __##name> name
