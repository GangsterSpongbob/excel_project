#include "utils.h"

#include "buffer_string.h"

bool char_is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool char_is_numeric(char c)
{
    return (c >= '0' && c <= '9') || c == '.';
}

Buffer_string long_to_str(long src)
{
    if (src == 0)
    {
        return Buffer_string("0");
    }

    bool is_negative{0};
    if (src < 0)
    {
        is_negative = 1;
        src = -src;
    }

    Buffer_string num_str;

    while (src != 0)
    {
        int digit = src % 10;
        num_str.append('0' + digit);
        src /= 10;
    }

    if (is_negative)
    {
        num_str.append('-');
    }

    return num_str.reverse();
}

Buffer_string decimal_to_str(double src)
{
    long whole_part{(long)src};
    double fractional_part{std::abs(src - whole_part)};

    const double difference_buffer{0.0001};
    if (fractional_part < difference_buffer)
    {
        return long_to_str(whole_part);
    }

    const double fract_to_long{10000};
    fractional_part *= fract_to_long;

    Buffer_string dec_str{long_to_str((long)fractional_part)};

    size_t index{dec_str.get_capacity() - 1};

    while (index > 0 && dec_str[index] == '0')
    {
        dec_str.mod_at_index(index, '\0');
    }

    Buffer_string result_str{long_to_str(whole_part)};

    result_str.append('.');

    for (size_t i = 0; i < dec_str.get_capacity(); i++)
    {
        result_str.append(dec_str[i]);
    }

    result_str.append('\0'); // Just in case again
    return result_str;
}

long min(long lhs, long rhs)
{
    return lhs < rhs ? lhs : rhs;
}