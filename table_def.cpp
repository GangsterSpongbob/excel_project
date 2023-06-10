#include "table.h"

#include <iostream>
#include <fstream>

#include "globals.h"

void Table::free()
{
    delete[] cells;
    cells = nullptr;
    rows = cols = 0;
}

Table::Table() : rows(0), cols(0), cells(nullptr) {}

Table::~Table()
{
    free();
}

char *Table::parse_cell(const char *line, size_t &line_index)
{
    if (*(line + line_index) == '\0' || line == nullptr)
    {
        std::cerr << "NULLPTR!\n"; //
        return nullptr;
    }

    const size_t buffer_size{1024};
    char text_buffer[buffer_size];
    size_t index{0};

    while (line[line_index + index] != ',' && line[line_index + index] != '\0')
    {
        if (line[line_index + index] == '"')
        {
            do
            {
                text_buffer[index] = line[line_index + index];
                index++;
            } while (line[line_index + index] != '"' && line[line_index + index] != '\0');
        }

        text_buffer[index] = line[line_index + index];
        index++;
    }

    text_buffer[index] = '\0';
    line_index += index;

    char *cell_data;
    copy_dynamic_str(cell_data, text_buffer);

    line_index++;

    return cell_data;
}

void Table::build_table(std::ifstream &file_in)
{
    cells = new Cell[rows * cols];
    const size_t buffer_size{1024};

    char row_buffer[buffer_size];
    char cell_buffer[buffer_size];

    size_t row_index{0};

    for (size_t i = 0; i < rows; i++)
    {
        file_in.getline(row_buffer, buffer_size);
        size_t line_index{0};
        char *temp{row_buffer};
        for (size_t j = 0; j < cols; j++)
        {
            char *cell_data{parse_cell(temp, line_index)};
            if (cell_data == nullptr)
            {
                cells[cols * i + j] = Cell();
            }
            else
            {
                cells[cols * i + j] = Cell(remove_whitespaces(cell_data));
            }
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

    count_rows_cols(file_in);

    std::cout << "Rows: " << rows << ", Cols: " << cols << '\n'; //

    file_in.clear();
    file_in.seekg(0, std::ios::beg);
}

// Table::Table(std::ifstream &file_in) : rows(0), cols(0)
// {
//     if (!file_in.is_open())
//     {
//         std::cout << "Failed to open input file.\n";
//         return;
//     }

//     count_rows_cols(file_in);
//     cells = new Cell[rows * cols];
//     build_table(file_in);

//     file_in.clear();
//     file_in.seekg(0, std::ios::beg);
// }

void Table::count_rows_cols(std::ifstream &file_in)
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

// void Table::build_table(std::ifstream &file_in)
// {
//     ;
// }

const char *print_type(const DataType &c1)
{
    const char *output = nullptr;

    switch (c1)
    {
    case DataType::Integer:
        output = "Integer";
        break;
    case DataType::FloatingPoint:
        output = "FloatingPoint";
        break;
    case DataType::CharString:
        output = "CharString";
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

void Table::print_table() const
{
    for (size_t i = 0; i < rows; i++)
    {
        std::cout << '|';
        for (size_t j = 0; j < cols; j++)
        {
            std::cout << cells[i * cols + j].get_text() << " (" << print_type(cells[i * cols + j].get_type()) << ") |"; // Remove get_type()
        }
        std::cout << '\n';
    }
}