#include "cell.h"

#include "utils.h"
#include "buffer_string.h"

void Cell::free()
{
    _value = 0.0;
    _type = DataType::Invalid;
    _text.clear();
}

DataType Cell::get_type() const
{
    return _type;
}

void Cell::copy_from(const Cell &src)
{
    _type = src._type;
    _text = src._text;
    _value = src._value;
}

double Cell::get_numeric_value() const
{
    return _value;
}

size_t Cell::get_text_capacity() const
{
    return _text.get_capacity();
}

Cell::Cell() : _type(DataType::Invalid), _text(""), _value(0.0) {}

Cell::Cell(const char *input)
{
    _text = input; // Buffer_string has nullptr protection

    copy_from(Cell(_text));
}

Cell::Cell(const Buffer_string &input)
{
    _text = input;

    if (input.is_whole())
    {
        _type = DataType::Integer;
        _value = input.to_whole();
    }
    else if (input.is_decimal())
    {
        _type = DataType::Decimal;
        _value = input.to_decimal();
    }
    else if (input.is_quoted())
    {
        _type = DataType::Quoted;
        _text = _text.remove_quotes();
        _value = 0.0;
    }
    else
    {
        if (input.is_empty())
        {
            _type = DataType::Quoted;
        }
        else
        {
            _type = DataType::Invalid;
        }
        _text = "";
        _value = 0.0;
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

Cell::Cell(Cell &&src) : _type(src._type), _text(src._text), _value(src._value)
{
    src.free();
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
        _type = src._type;
        _value = src._value;
    }

    return *this;
}

Buffer_string Cell::get_text() const
{
    return _text;
}

const char *type_to_char(const DataType &c1)
{
    switch (c1)
    {
    case DataType::Integer:
        return string_utils::integer;
    case DataType::Decimal:
        return string_utils::decimal;
    case DataType::Quoted:
        return string_utils::quoted;
    case DataType::Invalid:
        return string_utils::invalid;
    default:
        return string_utils::broken;
    }
}

void parse_row(const Buffer_string &line, Cell *cells, size_t cell_count)
{
    size_t cell_index{0}, line_index{0};

    Buffer_string row_str;

    while (line[line_index] != '\0' && cell_index < cell_count)
    {
        row_str.clear();
        while (line[line_index] != ',' && line[line_index] != '\0')
        {
            if (line[line_index] == '"')
            {
                do
                {
                    row_str.append(line[line_index++]);
                } while (line[line_index] != '"' && line[line_index] != '\0');
            }

            row_str.append(line[line_index++]);
        }

        cells[cell_index++] = Cell(row_str.trim_whitespaces());
        if (line[line_index] == ',')
        {
            line_index++;
        }
    }
}