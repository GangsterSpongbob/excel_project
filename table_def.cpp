#include "table.h"

#include "cell.h"

void Table::set_all_to_empty()
{
    for (size_t i = 0; i < MAX_COLUMN_COUNT; i++)
    {
        data[i] = Row();
    }
}

void Table::count_rows_and_cols(std::ifstream &file_in)
{
    row_count = column_count = 1;
    if (!file_in.is_open())
    {
        std::cout << "Failed to open input file.\n";
        return;
    }

    char ch;

    while (file_in.get(ch))
    {
        if (ch == '\n')
        {
            row_count++;
        }
    }

    file_in.clear();
    file_in.seekg(0, std::ios::beg);

    while (file_in.get(ch))
    {
        size_t current_cols{1};

        while (ch != '\n' && !file_in.eof())
        {
            if (ch == ',')
            {
                current_cols++;
            }
            else if (ch == '\"')
            {
                file_in.get(ch);
                while (ch != '\"' && ch != '\n')
                {
                    file_in.get(ch);
                }

                if (ch == '\n')
                {
                    break;
                }
            }
            file_in.get(ch);
        }

        if (current_cols > column_count)
        {
            column_count = current_cols;
        }
    }

    file_in.clear();
    file_in.seekg(0, std::ios::beg);
}

Table::Table() : row_count(1), column_count(1)
{
    set_all_to_empty();
}

Table::Table(std::ifstream &file_in)
{
    set_all_to_empty();
    count_rows_and_cols(file_in);
    for (size_t i = 0; i < row_count; i++)
    {
        Buffer_string temp;
        temp.getline(file_in);
        data[i] = Row(temp);
    }
}

void Table::print_table() const
{
    size_t col_width[MAX_COLUMN_COUNT]{}; // Initialize separately

    for (size_t i = 0; i < row_count; i++)
    {
        for (size_t j = 0; j < column_count; j++)
        {
            size_t current_len{0};
            current_len = data[i][j]->get_text_capacity();
            col_width[j] = current_len > col_width[j] ? current_len : col_width[j];
        }
    }

    std::cout << '+';
    for (size_t i = 0; i < column_count; i++)
    {
        for (size_t j = 0; j < col_width[i] + 2; j++)
        {
            std::cout << '~';
        }
        std::cout << '+';
    }
    std::cout << '\n';

    for (size_t i = 0; i < row_count; i++)
    {
        std::cout << '|';
        for (size_t j = 0; j < column_count; j++)
        {
            std::cout << ' ' << std::setw(col_width[j]) << std::left << data[i][j]->get_text().remove_quotes() << " |";
        }
        std::cout << '\n';
    }

    std::cout << '+';
    for (size_t i = 0; i < column_count; i++)
    {
        for (size_t j = 0; j < col_width[i] + 2; j++)
        {
            std::cout << '~';
        }
        std::cout << '+';
    }
    std::cout << '\n';
}

void Table::print_types() const
{
    const size_t cell_width{7}; // Max length of typename
    std::cout << '+';
    for (size_t i = 0; i < column_count; i++)
    {
        for (size_t j = 0; j < cell_width + 2; j++)
        {
            std::cout << '~';
        }
        std::cout << '+';
    }
    std::cout << '\n';

    for (size_t i = 0; i < row_count; i++)
    {
        std::cout << '|';
        for (size_t j = 0; j < column_count; j++)
        {
            std::cout << ' ' << std::setw(cell_width) << std::left << type_to_char2(data[i][j]->get_type()) << " |";
        }
        std::cout << '\n';
    }
    std::cout << '+';
    for (size_t i = 0; i < column_count; i++)
    {
        for (size_t j = 0; j < cell_width + 2; j++)
        {
            std::cout << '~';
        }
        std::cout << '+';
    }
    std::cout << '\n';
}

size_t Table::get_rows() const
{
    return row_count;
}

size_t Table::get_cols() const
{
    return column_count;
}

void Table::print_dimensions() const
{
    std::cout << "Rows: " << row_count << ", Cols: " << column_count << '\n';
}

size_t Table::get_count_of_invalid_cells() const
{
    size_t invalids{0};
    for (size_t i = 0; i < row_count; i++)
    {
        for (size_t j = 0; j < column_count; j++)
        {
            invalids += data[i][j]->get_type() == DataType::Invalid;
        }
    }

    return invalids;
}

double Table::get_value_at_index(size_t row_index, size_t col_index) const
{
    if (row_index < 0 || row_index > row_count - 1 || col_index < 0 || col_index > column_count - 1)
    {
        return 0.0;
    }

    return data[row_index][col_index]->get_numeric_value();
}

void Table::print_invalid_cells() const
{
    if (get_count_of_invalid_cells() == 0)
    {
        return;
    }

    for (size_t i = 0; i < row_count; i++)
    {
        for (size_t j = 0; j < column_count; j++)
        {
            if (data[i][j]->get_type() == DataType::Invalid)
            {
                std::cout << "Data at position (" << i + 1 << ',' << j + 1 << ") is invalid!\n";
            }
        }
    }
}

void Table::full_print() const
{
    print_dimensions();
    std::cout << '\n';
    std::cout << '\n';
    print_table();
    std::cout << '\n';
    std::cout << '\n';
    print_types();
    std::cout << '\n';
    std::cout << '\n';
    print_invalid_cells();
    std::cout << '\n';
    std::cout << '\n';
}

bool Table::mod_cell(size_t row_index, size_t col_index, const Buffer_string &input_data)
{
    if (row_index < 0 || row_index > row_count || col_index < 0 || col_index > column_count)
    {
        return 0;
    }

    if (!input_data.is_whole() && !input_data.is_decimal() && !input_data.is_quoted())
    {
        return 0;
    }

    data[row_index].modify_cell_at_index(col_index, input_data.trim_whitespaces());
    return 1;
}

void Table::write_to_file(std::ofstream &file_out) const
{
    for (size_t i = 0; i < row_count; i++)
    {
        for (size_t j = 0; j < column_count; j++)
        {
            if (data[i][j]->get_type() == DataType::Quoted)
            {
                file_out << data[i][j]->get_text().add_quotes();
            }
            else
            {
                file_out << data[i][j]->get_text();
            }

            if (j != column_count - 1)
            {
                file_out << ',';
            }
        }
        if (i != row_count - 1)
        {
            file_out << '\n';
        }
    }
}