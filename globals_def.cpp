#include <iostream>

#include "globals.h"
#include "cell.h"

bool is_whole(const char *whole)
{
    int index{};
    if (whole[index] == '-' || whole[index] == '+')
        index++;
    while (whole[index] != '\0')
        if (whole[index] < '0' || whole[index] > '9')
            return 0;

    return 1;
}

bool is_float(const char *flo)
{
    int index{}, dot_count{};
    if (flo[index] == '-' || flo[index] == '+')
        index++;

    while (flo[index] != '\0' && dot_count <= 1)
    {
        if (flo[index] == '.')
        {
            dot_count++;
            index++;
            continue;
        }

        if (flo[index] < '0' || flo[index] > '9')
            return 0;
    }

    return dot_count <= 1;
}

bool is_string(const char *str)
{
    return (str[0] == '/"') && (str[strlen(str - 1) == '/"']);
}

bool is_formula(const char *str)
{
    return str[0] == '=';
}

void printData(const Cell &src)
{
    switch (src.getType())
    {
    case DataType::Integer:
        std::cout << src.getLongData() << '\n';
        return;
    case DataType::FloatingPoint:
        std::cout << src.getDoubleData() << '\n';
        return;
    case DataType::CharString:
        std::cout << src.getStringData() << '\n';
        return;
    case DataType::Formula:
        std::cout << src.getStringData() << '\n';
        return;

    default:
        return;
    }
}

int countChars(long src)
{
    if (!src)
        return 1;

    int count{src < 0}; // count '-' if negative
    while (src)
    {
        src /= 10;
        ++count;
    }
    return count;
}

int countChars(double src)
{
}