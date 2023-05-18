#pragma once
#include <cstring>

enum class DataType
{
    Integer,
    FloatingPoint,
    CharString,
    Formula
};

class Cell
{
private:
    DataType _type;
    union Data
    {
        long long_value;
        double double_value;
        char *string_value;

        Data(const long &src = 0) : long_value(src) {}
        Data(const double &src) : double_value(src) {}
        Data(const char *src)
        {
            string_value = new char[strlen(src) + 1];
            strcpy(string_value, src);
        }
    } _data;
    void free(), copy_from(const Cell &);

public:
    DataType getType() const;
    char *getStringData() const;
    double getDoubleData() const;
    long getLongData() const;
    Cell();
    Cell(const long &);
    Cell(const double &);
    Cell(const char *);
    Cell(const Cell &);
    Cell &operator=(const Cell &);
    ~Cell();
};
