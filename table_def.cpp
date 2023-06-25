#include "table.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include "utils.h"
#include "buffer_string.h"

void Table::free()
{
    delete[] cells;
    cells = nullptr;
    rows = cols = 0;
}

void Table::copy_from(const Table &src)
{
    rows = src.rows;
    cols = src.cols;
    if (src.cells != nullptr)
    {
        cells = new Cell[src.rows * src.cols];
        for (size_t i = 0; i < rows * cols; i++)
        {
            cells[i] = src.cells[i];
        }
    }
    else
    {
        cells = nullptr;
    }
}

Table::Table() : rows(0), cols(0), cells(nullptr) {}

Table::~Table()
{
    free();
}

void Table::build_table(std::ifstream &file_in)
{
    if (cells != nullptr)
    {
        delete[] cells;
    }

    cells = new Cell[rows * cols];

    Buffer_string row_buffer;
    Buffer_string cell_buffer;

    for (size_t i = 0; i < rows; i++)
    {
        row_buffer.getline(file_in);
        Cell temp_cells[cols];
        parse_row(row_buffer, temp_cells, cols);
        for (size_t j = 0; j < cols; j++)
        {
            cells[cols * i + j] = temp_cells[j];
        }
    }

    file_in.clear();
    file_in.seekg(0, std::ios::beg);
}

void Table::count_rows_and_cols(std::ifstream &file_in)
{
    rows = cols = 1;
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
            rows++;
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

        if (current_cols > cols)
        {
            cols = current_cols;
        }
    }

    file_in.clear();
    file_in.seekg(0, std::ios::beg);
}

Table::Table(std::ifstream &file_in) : rows(0), cols(0), cells(nullptr)
{
    if (!file_in.is_open())
    {
        std::cout << "Failed to open input file.\n";
        cells = new Cell[1];
        return;
    }

    count_rows_and_cols(file_in);

    file_in.clear();
    file_in.seekg(0, std::ios::beg);

    build_table(file_in);

    file_in.clear();
    file_in.seekg(0, std::ios::beg);
}

Table::Table(const Table &src)
{
    if (this != &src)
    {
        free();
        copy_from(src);
    }
}

Table::Table(Table &&src) : rows(src.rows), cols(src.cols), cells(src.cells)
{
    src.rows = 0;
    src.cols = 0;
    src.cells = nullptr;
}

Table &Table::operator=(const Table &src)
{
    if (this != &src)
    {
        free();
        copy_from(src);
    }

    return *this;
}

Table &Table::operator=(Table &&src)
{
    if (this != &src)
    {
        free();
        rows = src.rows;
        cols = src.cols;
        cells = src.cells;

        src.rows = 0;
        src.cols = 0;
        src.cells = nullptr;
    }

    return *this;
}

bool Table::mod_cell(size_t row_index, size_t col_index, const Buffer_string &input_data)
{
    if (row_index < 0 || row_index > rows || col_index < 0 || col_index > cols)
    {
        return 0;
    }

    if (!input_data.is_whole() && !input_data.is_decimal() && !input_data.is_quoted())
    {
        return 0;
    }

    cells[cols * row_index + col_index] = Cell(input_data.trim_whitespaces());
    return 1;
}

void Table::write_to_file(std::ofstream &file_out) const
{
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            file_out << cells[i * cols + j].get_text();
            if (j != cols - 1)
            {
                file_out << ',';
            }
        }
        if (i != rows - 1)
        {
            file_out << '\n';
        }
    }
}

void Table::print_table() const
{
    size_t col_width[cols];
    for (size_t i = 0; i < cols; i++)
    {
        col_width[i] = 0;
    }

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            size_t current_len{0};
            if (cells[i * cols + j].get_type() == DataType::Quoted)
            {
                current_len = cells[i * cols + j].get_text_capacity();
            }
            else if (cells[i * cols + j].get_type() == DataType::Integer || cells[i * cols + j].get_type() == DataType::Decimal)
            {
                current_len = cells[i * cols + j].get_text_capacity();
            }
            else
            {
                current_len = 0;
            }

            if (current_len > col_width[j])
            {
                col_width[j] = current_len;
            }
        }
    }

    std::cout << '+';
    for (size_t i = 0; i < cols; i++)
    {
        for (size_t j = 0; j < col_width[i] + 2; j++)
        {
            std::cout << '~';
        }
        std::cout << '+';
    }
    std::cout << '\n';

    for (size_t i = 0; i < rows; i++)
    {
        std::cout << '|';
        for (size_t j = 0; j < cols; j++)
        {
            if (cells[i * cols + j].get_type() == DataType::Integer || cells[i * cols + j].get_type() == DataType::Decimal)
            {
                std::cout << ' ' << std::setw(col_width[j]) << std::left << cells[i * cols + j].get_text() << " |";
            }
            else if (cells[i * cols + j].get_type() == DataType::Quoted)
            {
                std::cout << ' ' << std::setw(col_width[j]) << std::left << cells[i * cols + j].get_text() << " |";
            }
            else
            {
                std::cout << ' ' << std::setw(col_width[j]) << std::left << ""
                          << " |";
            }
        }
        std::cout << '\n';
    }

    std::cout << '+';
    for (size_t i = 0; i < cols; i++)
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
    for (size_t i = 0; i < cols; i++)
    {
        for (size_t j = 0; j < cell_width + 2; j++)
        {
            std::cout << '~';
        }
        std::cout << '+';
    }
    std::cout << '\n';

    for (size_t i = 0; i < rows; i++)
    {
        std::cout << '|';
        for (size_t j = 0; j < cols; j++)
        {
            std::cout << ' ' << std::setw(cell_width) << std::left << type_to_char(cells[i * cols + j].get_type()) << " |";
        }
        std::cout << '\n';
    }
    std::cout << '+';
    for (size_t i = 0; i < cols; i++)
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
    return rows;
}

size_t Table::get_cols() const
{
    return cols;
}

void Table::print_dimensions() const
{
    std::cout << "Rows: " << rows << ", Cols: " << cols << '\n';
}

void Table::print_invalid_cells() const
{
    if (get_count_of_invalid_cells() == 0)
    {
        return;
    }

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            if (cells[i * cols + j].get_type() == DataType::Invalid)
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

const Buffer_string Table::get_text_by_index(size_t row_index, size_t col_index) const
{
    if (row_index < rows && col_index < cols && row_index >= 0 && col_index >= 0)
    {
        return cells[row_index * cols + col_index].get_text();
    }
    else
    {
        return string_utils::empty;
    }
}

double Table::get_numeric_value_by_index(size_t row_index, size_t col_index) const
{
    if (row_index < rows && col_index < cols && row_index >= 0 && col_index >= 0)
    {
        return cells[row_index * cols + col_index].get_numeric_value();
    }
    else
    {
        return 0.0;
    }
}

size_t Table::get_count_of_invalid_cells() const
{
    size_t invalids{0};
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            invalids += cells[i * cols + j].get_type() == DataType::Invalid;
        }
    }

    return invalids;
}