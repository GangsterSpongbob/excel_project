#pragma once
#include <cstring>

enum class DataType
{
    Integer,
    FloatingPoint,
    CharString,
    Formula,
    Invalid
};

class Cell
{
private:
    DataType _type;
    char *_text;
    union Data
    {
        long long_value;
        double double_value;
        char *string_value;
    } _data;
    void free(), copy_from(const Cell &);

public:
    DataType get_type() const;
    char *get_string_data() const;
    double get_double_data() const;
    long get_long_data() const;
    Cell();
    Cell(const char *);
    Cell(const Cell &);
    Cell &operator=(const Cell &);
    ~Cell();

    void print_data() const;
};
