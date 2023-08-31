#ifndef CELL_H
#define CELL_H

#include "buffer_string.h"
#include "utils.h"

enum class DataType
{
    Integer,
    Decimal,
    Quoted,
    Invalid
};

extern Buffer_string type_to_char2(const DataType &);

class Cell
{
protected:
    DataType _type;
    Buffer_string _text;
    double _value;
    Cell(DataType type, Buffer_string text, double value) : _type(type), _text(text), _value(value) {}

public:
    virtual ~Cell() = default;

    virtual Cell *clone() const = 0;

    DataType get_type() const { return _type; }
    double get_numeric_value() const { return _value; }
    size_t get_text_capacity() const { return _text.get_capacity(); }
    Buffer_string get_text() const { return _text; }
};

class Cell_decimal : public Cell
{
public:
    Cell_decimal(Buffer_string text, double value) : Cell(DataType::Decimal, text, value) {}

    Cell *clone() const override final;
};

class Cell_integer : public Cell
{
public:
    Cell_integer(Buffer_string text, double value) : Cell(DataType::Integer, text, value) {}

    Cell *clone() const override final;
};

class Cell_quoted : public Cell
{
public:
    Cell_quoted(Buffer_string src) : Cell(DataType::Quoted, src, 0.0) {}

    Cell *clone() const override final;
};

class Cell_invalid : public Cell
{
public:
    Cell_invalid() : Cell(DataType::Invalid, "", 0.0) {}

    Cell *clone() const override final;
};

#endif // CELL_H