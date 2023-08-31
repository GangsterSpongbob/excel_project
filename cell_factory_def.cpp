#include "cell_factory.h"

Cell *create_cell(const Buffer_string &src)
{
    if (src.is_decimal())
    {
        return new Cell_decimal(src, src.to_decimal());
    }
    else if (src.is_whole())
    {
        return new Cell_integer(src, src.to_whole());
    }
    else if (src.is_quoted() || src.is_empty())
    {
        return new Cell_quoted(src);
    }
    else
    {
        return new Cell_invalid();
    }
}