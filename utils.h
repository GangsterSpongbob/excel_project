#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

#include "buffer_string.h"

extern bool char_is_operator(char);
extern bool char_is_numeric(char);
extern bool string_is_valid_addresss(Buffer_string);

extern Buffer_string long_to_str(long);
extern Buffer_string decimal_to_str(double);

extern long min(long, long);

#endif // UTILS_H