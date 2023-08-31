#include "cell.h"

Buffer_string type_to_char2(const DataType &c1)
{
    switch (c1)
    {
    case DataType::Integer:
        return "Integer";
    case DataType::Decimal:
        return "Decimal";
    case DataType::Quoted:
        return "Quoted";
    case DataType::Invalid:
        return "Invalid";
    default:
        return "Broken";
    }
}

Cell *Cell_decimal::clone() const
{
    Cell *dest{new Cell_decimal(*this)};
    return dest;
}

Cell *Cell_integer::clone() const
{
    Cell *dest{new Cell_integer(*this)};
    return dest;
}

Cell *Cell_quoted::clone() const
{
    Cell *dest{new Cell_quoted(*this)};
    return dest;
}

Cell *Cell_invalid::clone() const
{
    Cell *dest{new Cell_invalid(*this)};
    return dest;
}