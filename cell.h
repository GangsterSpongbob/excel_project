#ifndef CELL_H
#define CELL_H

#include "buffer_string.h"

enum class DataType
{
    Integer,
    Decimal,
    Quoted,
    Invalid
};

extern const char *type_to_char(const DataType &);

class Cell
{
private:
    DataType _type;
    Buffer_string _text;
    double _value;

    void free();
    void copy_from(const Cell &);

public:
    DataType get_type() const;
    double get_numeric_value() const;
    size_t get_text_capacity() const;
    Buffer_string get_text() const;

    Cell();
    Cell(const char *);
    Cell(const Buffer_string &);
    Cell(const Cell &);
    Cell(Cell &&);
    Cell &operator=(const Cell &);
    Cell &operator=(Cell &&);
    ~Cell() = default;
};

extern void parse_row(const Buffer_string &, Cell *, size_t);

#endif // CELL_H