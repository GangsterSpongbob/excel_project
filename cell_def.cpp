#include "cell.h"

#include <iostream>

#include "globals.h"

void Cell::free()
{
    if (_type == DataType::Integer)
    {
        _data.long_value = 0;
    }
    else if (_type == DataType::FloatingPoint)
    {
        _data.double_value = 0.0;
    }
    else
    {
        delete[] _data.string_value;
        _data.string_value = nullptr;
    }

    _type = DataType::Invalid;
    delete[] _text;
    _text = nullptr;
}

DataType Cell::get_type() const
{
    return _type;
}

void Cell::copy_from(const Cell &src)
{
    _type = src._type;
    delete[] _text;
    copy_dynamic_str(_text, src._text);

    if (_type == DataType::Integer)
    {
        _data.long_value = src._data.long_value;
    }
    else if (_type == DataType::FloatingPoint)
    {
        _data.double_value = src._data.double_value;
    }
    else
    {
        delete[] _data.string_value;
        copy_dynamic_str(_data.string_value, src._data.string_value);
    }
}

char *Cell::get_string_data() const
{
    return _data.string_value;
}

long Cell::get_long_data() const
{
    return _data.long_value;
}

double Cell::get_double_data() const
{
    return _data.double_value;
}

// template <typename T>
// T *Cell::get_data() const;

Cell::Cell() : _type(DataType::Invalid), _text(new char[1]{}) { _data.string_value = nullptr; }

Cell::Cell(const char *input)
{
    copy_dynamic_str(_text, input);
    if (is_whole(input))
    {
        _type = DataType::Integer;
        _data.long_value = str_to_whole(input);
    }
    else if (is_float(input))
    {
        _type = DataType::FloatingPoint;
        _data.double_value = str_to_float(input);
    }
    else if (is_string(input))
    {
        _type = DataType::CharString;
        copy_dynamic_str(_data.string_value, input);
    }
    else if (is_formula(input))
    {
        _type = DataType::Formula;
        copy_dynamic_str(_data.string_value, input);
    }
    else
    {
        _type = DataType::Invalid;
        _data.string_value = nullptr;
    }
}

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

char *Cell::get_text() const
{
    return _text;
}