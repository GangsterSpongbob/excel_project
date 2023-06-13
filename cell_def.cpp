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
    if (input != nullptr)
    {
        _text = new char[strlen(input) + 1];
        strncpy(_text, input, strlen(input) + 1);
    }
    else
    {
        _text = nullptr;
    }

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
        if (input != nullptr)
        {
            _data.string_value = new char[strlen(input) + 1];
            strncpy(_data.string_value, input, strlen(input) + 1);
        }
        else
        {
            _data.string_value = nullptr;
        }
    }
    else if (is_formula(input))
    {
        _type = DataType::Formula;
        if (input != nullptr)
        {
            _data.string_value = new char[strlen(input) + 1];
            strncpy(_data.string_value, input, strlen(input) + 1);
        }
        else
        {
            _data.string_value = nullptr;
        }
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
    const char *output = nullptr;

    switch (c1)
    {
    case DataType::Integer:
        output = "Integer";
        break;
    case DataType::FloatingPoint:
        output = "Float";
        break;
    case DataType::CharString:
        output = "String";
        break;
    case DataType::Formula:
        output = "Formula";
        break;
    case DataType::Invalid:
        output = "Invalid";
        break;
    default:
        output = "Broke";
        break;
    }

    return output;
}

const Cell *parse_row(const char *line, size_t cell_count)
{
    Cell *cells = new Cell[cell_count];

    size_t cell_index{0}, line_index{0};

    const size_t buffer_size{1024};
    char text_buffer[buffer_size]{};

    try
    {
        while (line[line_index] != '\0' && cell_index < cell_count)
        {
            size_t buffer_index{0};
            while (line[line_index] != ',' && line[line_index] != '\0' && buffer_index < buffer_size)
            {
                if (buffer_index > buffer_size - 1)
                {
                    throw std::runtime_error("Line size exceeds buffer capacity!");
                }

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

    catch (const std::runtime_error &ex)
    {
        std::cerr << ex.what() << '\n';
    }
    return cells;
}