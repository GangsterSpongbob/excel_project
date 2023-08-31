#include "row.h"

void Row::free()
{
    for (size_t i = 0; i < MAX_ROW_SIZE; i++)
    {
        delete cells[i];
    }
}

void Row::copy_from(const Row &src)
{
    size = src.size;
    for (size_t i = 0; i < MAX_ROW_SIZE; i++)
    {
        cells[i] = src.cells[i]->clone();
    }
}

void Row::move_from(Row &&src)
{
    size = src.size;
    src.size = 0;
    for (size_t i = 0; i < MAX_ROW_SIZE; i++)
    {
        cells[i] = src.cells[i];
        src.cells[i] = nullptr;
    }
}

void Row::set_all_empty()
{
    for (size_t i = 0; i < MAX_ROW_SIZE; i++)
    {
        cells[i] = create_cell("");
    }
}

Row::Row(const Buffer_string &src_str) : size(0)
{
    set_all_empty();

    size_t cell_index{0}, line_index{0};

    Buffer_string current_cell_text;

    while (src_str[line_index] != '\0' && cell_index < MAX_ROW_SIZE)
    {
        current_cell_text.clear();
        while (src_str[line_index] != ',' && src_str[line_index] != '\0')
        {
            if (src_str[line_index] == '"')
            {
                do
                {
                    current_cell_text.append(src_str[line_index++]);
                } while (src_str[line_index] != '"' && src_str[line_index] != '\0');
            }

            current_cell_text.append(src_str[line_index++]);
        }

        cells[cell_index++] = create_cell(current_cell_text.trim_whitespaces());
        size++;

        if (src_str[line_index] == ',')
        {
            line_index++;
        }
    }

    while (cell_index < MAX_ROW_SIZE) // Fill rest with empty cells
    {
        cells[cell_index++] = create_cell("");
    }
}

Row &Row::modify_cell_at_index(size_t index, const Buffer_string &src)
{
    if (index < 0 || index > size)
    {
        return *this;
    }

    delete cells[index];
    cells[index] = create_cell(src);

    return *this;
}

Row *Row::operator=(const Row &src)
{
    if (this != &src)
    {
        free();
        copy_from(src);
    }

    return this;
}

Row *Row::operator=(Row &&src)
{
    if (this != &src)
    {
        free();
        move_from(std::move(src));
    }

    return this;
}

Row &Row::clear()
{
    for (size_t i = 0; i < size; i++)
    {
        modify_cell_at_index(i, "0");
    }

    return *this;
}