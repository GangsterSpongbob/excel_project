#include <iostream>
#include <cstring>
#include <exception>
#include <cmath>

#include "globals.h"

void copy_string(char *&dest, const char *src)
{
    delete[] dest;
    if (src != nullptr)
    {
        dest = new char[strlen(src) + 1];
        strcpy(dest, src);
    }
    else
    {
        dest = nullptr;
    }
}

bool is_whole(const char *whole)
{
    if (whole == nullptr || whole[0] == '\0')
    {
        return 0;
    }

    int index{0}, digit_count{0};
    if (whole[index] == '-' || whole[index] == '+')
    {
        index++;
    }

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

bool is_float(const char *flo)
{
    if (flo == nullptr || flo[0] == '\0')
    {
        return 0;
    }

    int index{0}, dot_count{0}, digit_count{0};
    if (flo[index] == '-' || flo[index] == '+')
    {
        index++;
    }

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

bool is_string(const char *str)
{
    return (str[0] == '"') && (str[strlen(str) - 1 == '"']) && strlen(str) >= 2;
}

bool is_formula(const char *str)
{
    return str[0] == '=';
}

long str_to_whole(const char *str)
{
    try
    {
        if (str == nullptr || str[0] == '\0' || !is_whole(str))
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
        if (str == nullptr || str[0] == '\0' || !is_float(str))
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