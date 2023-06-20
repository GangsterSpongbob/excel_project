#include "cell.h"

#include <iostream>
#include <cstring>

#include "utils.h"

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
        if (_data.string_value != utils::empty_string)
        {
            delete[] _data.string_value;
        }
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
    if (src._text != nullptr)
    {
        delete[] _text;
        _text = new char[strlen(src._text) + 1];
        strncpy(_text, src._text, strlen(src._text) + 1);
    }
    else
    {
        _text = nullptr;
    }

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
        if (src._data.string_value != nullptr)
        {
            _data.string_value = new char[strlen(src._data.string_value) + 1];
            strncpy(_data.string_value, src._data.string_value, strlen(src._data.string_value) + 1);
        }
    }
}

double Cell::get_numeric_value() const
{
    if (_type == DataType::Integer)
    {
        return _data.long_value;
    }
    else if (_type == DataType::FloatingPoint)
    {
        return _data.double_value;
    }
    else
    {
        return 0.0;
    }
}

const char *Cell::get_string_data() const
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

Cell::Cell() : _type(DataType::Invalid), _text(new char[1]{}) { _data.string_value = nullptr; }

Cell::Cell(const char *input)
{
    if (input != nullptr)
    {
        _text = new char[strlen(input) + 1];
        strncpy(_text, input, strlen(input) + 1);
    }
    else
    {
        _text = nullptr;
    }

    if (str_is_whole_number(input))
    {
        _type = DataType::Integer;
        _data.long_value = str_to_whole(input);
    }
    else if (str_is_decimal_number(input))
    {
        _type = DataType::FloatingPoint;
        _data.double_value = str_to_float(input);
    }
    else if (str_is_in_quotes(input))
    {
        if (input != nullptr)
        {
            _type = DataType::CharString;
            if (strlen(input) == 0)
            {
                _data.string_value = utils::empty_string;
            }
            else
            {
                _data.string_value = new char[strlen(input) - 1]{};
                for (size_t i = 1; i < strlen(input) - 1; i++)
                {
                    _data.string_value[i - 1] = input[i];
                }
            }
        }
        else
        {
            _type = DataType::CharString;
            _data.string_value = utils::empty_string; // Can't happen
        }
    }
    else
    {
        if (input == nullptr || input == utils::empty_string)
        {
            _type = DataType::CharString;
            _data.string_value = utils::empty_string;
        }
        else
        {
            _type = DataType::Invalid;
            _data.string_value = utils::empty_string;
        }
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

Cell::Cell(Cell &&src) : _text(src._text), _type(src._type)
{
    src._text = nullptr;
    if (src._type == DataType::Integer)
    {
        _data.long_value = src._data.long_value;
        src._data.long_value = 0;
    }
    else if (src._type == DataType::FloatingPoint)
    {
        _data.double_value = src._data.double_value;
        src._data.double_value = 0;
    }
    else
    {
        _data.string_value = src._data.string_value;
        src._data.string_value = nullptr;
    }

    src._type = DataType::Invalid;
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

Cell &Cell::operator=(Cell &&src)
{
    if (this != &src)
    {
        free();
        _text = src._text;
        src._text = nullptr;

        if (src._type == DataType::Integer)
        {
            _data.long_value = src._data.long_value;
            src._data.long_value = 0;
        }
        else if (src._type == DataType::FloatingPoint)
        {
            _data.double_value = src._data.double_value;
            src._data.double_value = 0;
        }
        else
        {
            _data.string_value = src._data.string_value;
            src._data.string_value = nullptr;
        }

        _type = src._type;
        src._type = DataType::Invalid;
    }

    return *this;
}

Cell::~Cell()
{
    free();
}

const char *Cell::get_text() const
{
    if (_text != nullptr)
    {
        return _text;
    }
    else
    {
        return utils::empty_string;
    }
}

const char *type_to_char(const DataType &c1)
{
    switch (c1)
    {
    case DataType::Integer:
        return "Integer";
    case DataType::FloatingPoint:
        return "Float";
    case DataType::CharString:
        return "String";
    case DataType::Invalid:
        return "Invalid";
    default:
        return "Broke";
    }
}

void parse_row(const char *line, Cell *cells, size_t cell_count)
{
    size_t cell_index{0}, line_index{0};

    const size_t buffer_size{1024};
    char text_buffer[buffer_size]{};

    while (line[line_index] != '\0' && cell_index < cell_count)
    {
        size_t buffer_index{0};
        while (line[line_index] != ',' && line[line_index] != '\0' && buffer_index < buffer_size)
        {
            if (line[line_index] == '"')
            {
                do
                {
                    text_buffer[buffer_index++] = line[line_index++];
                } while (line[line_index] != '"' && line[line_index] != '\0' && buffer_index < buffer_size);
            }

            text_buffer[buffer_index++] = line[line_index++];
        }
        text_buffer[buffer_index] = '\0';
        cells[cell_index++] = Cell(remove_whitespaces(text_buffer));
        text_buffer[0] = '\0';
        if (line[line_index] == ',')
        {
            line_index++;
        }
    }
}