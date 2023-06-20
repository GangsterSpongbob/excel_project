#include "utils.h"

#include <iostream>
#include <cstring>
#include <exception>

namespace utils
{
    char empty_string[1]{""};
}

char *remove_whitespaces(char *src)
{
    if (src == nullptr)
    {
        return nullptr;
    }

    char *runner = src;

    while (*runner == ' ' || *runner == '\t')
    {
        ++runner;
    }

    char *dest = src;
    while (*runner != '\0')
    {
        *dest++ = *runner++;
    }

    --dest;
    while (dest >= src && (*dest == ' ' || *dest == '\t'))
    {
        --dest;
    }

    *(dest + 1) = '\0';

    return src;
}

bool str_is_whole_number(const char *whole)
{
    if (whole == nullptr || whole[0] == '\0')
    {
        return 0;
    }

    size_t index{0};
    if (whole[index] == '-' || whole[index] == '+')
    {
        index++;
    }

    size_t digit_count{0};
    while (whole[index] != '\0')
    {
        if (whole[index] >= '0' && whole[index] <= '9')
        {
            digit_count++;
        }

        else
        {
            return false;
        }
        index++;
    }

    return digit_count;
}

bool str_is_decimal_number(const char *flo)
{
    if (flo == nullptr || flo[0] == '\0')
    {
        return 0;
    }

    size_t index{0};
    if (flo[index] == '-' || flo[index] == '+')
    {
        index++;
    }

    size_t dot_count{0};
    size_t digit_count{0};
    while (flo[index] != '\0' && dot_count <= 1)
    {
        if (flo[index] == '.')
        {
            dot_count++;
        }
        else if (flo[index] >= '0' && flo[index] <= '9')
        {
            digit_count++;
        }
        else
        {
            return false;
        }

        index++;
    }

    return dot_count == 1 && digit_count > 0;
}

bool str_is_in_quotes(const char *str)
{
    return (str[0] == '"') && (str[strlen(str) - 1 == '"']) && strlen(str) >= 2;
}

bool str_is_formula(const char *str)
{
    return str[0] == '=';
}

long str_to_whole(const char *str)
{
    try
    {
        if (str == nullptr || str[0] == '\0' || !str_is_whole_number(str))
        {
            throw std::invalid_argument("Invaid string format or non-existent string!");
        }

        size_t index{0};
        bool negative{str[0] == '-'};
        long long result{0};

        if (str[index] == '-' || str[index] == '+')
        {
            index++;
        }

        while (str[index] != '\0')
        {
            result = result * 10 + (str[index++] - '0');
        }

        return negative ? -result : result;
    }

    catch (const std::exception &ia)
    {
        std::cerr << ia.what() << '\n';
        return 0;
    }
}

double str_to_float(const char *str)
{
    try
    {
        if (str == nullptr || str[0] == '\0' || !str_is_decimal_number(str))
        {
            throw std::invalid_argument("Invaid string format or non-existent string!");
        }

        size_t index{0}, digits_after_dot{0};
        bool negative{str[0] == '-'}, dot_met{0};
        double result{0};

        if (str[index] == '-' || str[index] == '+')
        {
            index++;
        }

        while (str[index] != '\0')
        {
            if (str[index] != '.')
            {
                result = result * 10.0 + (str[index] - '0');
                if (dot_met)
                {
                    digits_after_dot++;
                }
            }
            else
            {
                dot_met = 1;
            }
            index++;
        }

        for (size_t i = 0; i < digits_after_dot; i++)
        {
            result /= 10.0;
        }

        return negative ? -result : result;
    }

    catch (const std::exception &ia)
    {
        std::cerr << ia.what() << '\n';
        return 0.0;
    }
}

bool char_is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool char_is_numeric(char c)
{
    return (c >= '0' && c <= '9') || c == '.';
}

void reverse_string(char *rev)
{
    if (rev == nullptr)
    {
        return;
    }

    size_t length = strlen(rev);
    char *copy = new char[length + 1];
    strncpy(copy, rev, length + 1);

    size_t start{0};
    size_t end{length - 1};

    while (start <= end)
    {
        if (start == end)
        {
            break;
        }
        else
        {
            char temp{copy[start]};
            copy[start++] = copy[end];
            copy[end--] = temp;
        }
    }

    strncpy(rev, copy, length + 1);

    delete[] copy;
}

void long_to_str(char *buffer, long src)
{
    if (src == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    bool is_negative{0};
    if (src < 0)
    {
        is_negative = 1;
        src = -src;
    }

    size_t index{0};

    while (src != 0)
    {
        int digit = src % 10;
        try
        {
            buffer[index++] = '0' + digit;
        }
        catch (const std::out_of_range &oor)
        {
            buffer[index - 2] = '\0';
            break;
        }
        src /= 10;
    }

    if (is_negative)
    {
        try
        {
            buffer[index++] = '-';
        }
        catch (const std::out_of_range &oor)
        {
            buffer[index - 2] = '\0';
        }
    }

    buffer[index] = '\0';
    reverse_string(buffer);
}

void number_to_str(char *dest, double src)
{
    long whole_part{(long)src};
    double fractional_part{std::abs(src - whole_part)};

    const double difference_buffer{0.0001};
    if (fractional_part < difference_buffer)
    {
        long_to_str(dest, whole_part);
        return;
    }

    const double fract_to_long{10000};
    fractional_part *= fract_to_long;

    const size_t buffer_size{64};
    char fract_string[buffer_size]{};
    long_to_str(fract_string, (long)fractional_part);

    size_t index{strlen(fract_string) - 1};

    while (index > 0 && fract_string[index] == '0')
    {
        fract_string[index] = '\0';
    }

    char whole_string[buffer_size]{};
    long_to_str(whole_string, whole_part);

    for (size_t i = 0; i < strlen(whole_string); i++)
    {
        try
        {
            dest[i] = whole_string[i];
        }
        catch (const std::out_of_range &oor)
        {
            dest[i - 1] = '\0';
            break;
        }
    }

    try
    {
        dest[strlen(whole_string)] = '.';
    }
    catch (const std::out_of_range &oor)
    {
    }

    for (size_t i = 0; i < strlen(fract_string); i++)
    {
        try
        {
            dest[strlen(whole_string) + 1 + i] = fract_string[i];
        }
        catch (const std::out_of_range &oor)
        {
            dest[strlen(whole_string) + i] = '\0';
            break;
        }
    }

    dest[strlen(whole_string) + strlen(fract_string) + 1] = '\0';
}