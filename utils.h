#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

#include "buffer_string.h"

namespace string_utils
{
    constexpr char empty[1]{""};
    constexpr char integer[8]{"Integer"};
    constexpr char decimal[8]{"Decimal"};
    constexpr char quoted[7]{"Quoted"};
    constexpr char invalid[8]{"Invalid"};
    constexpr char broken[7]{"Broken"};
    constexpr char error[7]{"ERROR!"};
}

extern bool char_is_operator(char);
extern bool char_is_numeric(char);

extern Buffer_string long_to_str(long);
extern Buffer_string decimal_to_str(double);

extern long min(long, long);

#endif // UTILS_H