#include <cstring>
#include <iostream>

#include "globals.h"
#include "cell.h"

void Cell::free()
{
    switch (_type)
    {
    case DataType::CharString:
        delete _data.string_value;
        _data.string_value = nullptr;
        break;
    case DataType::Formula:
        delete _data.string_value;
        _data.string_value = nullptr;
        break;
    case DataType::Integer:
        _data.long_value = 0;
        break;
    case DataType::FloatingPoint:
        _data.double_value = 0;
        break;

    default:
        break;
    }
}

DataType Cell::getType() const
{
    return _type;
}

void Cell::copy_from(const Cell &src)
{
    _type = src._type;
    switch (_type)
    {
    case DataType::Integer:
        _data.long_value = src._data.long_value;
        break;
    case DataType::FloatingPoint:
        _data.double_value = src._data.double_value;
        break;
    case DataType::CharString:
        _data.string_value = new char[strlen(src._data.string_value) + 1];
        strcpy(_data.string_value, src._data.string_value);
        break;
    case DataType::Formula:
        _data.string_value = new char[strlen(src._data.string_value) + 1];
        strcpy(_data.string_value, src._data.string_value);
        break;
    default:
        break;
    }
}

char *Cell::getStringData() const
{
    return _data.string_value;
}

long Cell::getLongData() const
{
    return _data.long_value;
}

double Cell::getDoubleData() const
{
    return _data.double_value;
}

Cell::Cell() : _type(DataType::Integer), _data(0L) {}

Cell::Cell(const long &src = 0) : _type(DataType::Integer), _data(src) {}

Cell::Cell(const double &src) : _type(DataType::FloatingPoint), _data(src) {}

Cell::Cell(const char *src) : _type(is_formula(src) ? DataType::Formula : DataType::CharString), _data(src) {}

Cell::Cell(const Cell &src)
{
    if (this != &src)
    {
        free();
        copy_from(src);
    }
}

Cell &Cell::operator=(const Cell &src)
{
    if (this != &src)
    {
        free();
        copy_from(src);
    }

    return *this;
}

Cell::~Cell()
{
    free();
}