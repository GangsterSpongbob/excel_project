#include "table.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

#include "utils.h"

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
    cells = new Cell[rows * cols];
    const size_t buffer_size{1024};

    char row_buffer[buffer_size];
    char cell_buffer[buffer_size];

    for (size_t i = 0; i < rows; i++)
    {
        file_in.getline(row_buffer, buffer_size);
        const Cell *temp_cells{parse_row(row_buffer, cols)};
        for (size_t j = 0; j < cols; j++)
        {
            cells[cols * i + j] = temp_cells[j];
        }
        delete[] temp_cells;
    }

    file_in.clear();
    file_in.seekg(0, std::ios::beg);
}

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

Table::Table(std::ifstream &file_in) : rows(0), cols(0), cells(nullptr)
{
    if (!file_in.is_open())
    {
        std::cout << "Failed to open input file.\n";
        cells = new Cell[1];
        return;
    }

    count_rows_cols(file_in);

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

Table &Table::operator=(const Table &src)
{
    if (this != &src)
    {
        free();
        copy_from(src);
    }

    return *this;
}

void Table::mod_cell(size_t row_index, size_t col_index, char *input_data)
{
    if (row_index < 0 || row_index > rows || col_index < 0 || col_index > cols)
    {
        return;
    }

    cells[cols * row_index + col_index] = Cell(remove_whitespaces(input_data));
}

void Table::write_to_file(std::ofstream &file_out) const
{
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            file_out << cells[i * cols + j].get_text(); // write_cell_text_to_file()
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
            if (strlen(cells[i * cols + j].get_text()) > col_width[j])
            {
                col_width[j] = strlen(cells[i * cols + j].get_text());
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
            std::cout << ' ' << std::setw(col_width[j]) << std::left << cells[i * cols + j].get_text() << " |";
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
    std::cout << "Rows: " << rows << ", Cols: " << cols << '\n'; //
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
}

char *Table::get_text_by_index(size_t row_index, size_t col_index) const
{
    if (row_index < rows && col_index < cols && row_index >= 0 && col_index >= 0)
    {
        return cells[row_index * cols + col_index].get_text();
    }

    return utils::empty_string;
}

void user_interface()
{
    const size_t buffer_size{1024};
    char text_buffer[buffer_size]{};

    std::cout
        << "Enter name of file to read from or name of new file: ";
    std::cin.getline(text_buffer, buffer_size);

    std::ifstream file_in(remove_whitespaces(text_buffer));
    Table current_table(file_in);
    char output_file_name[buffer_size];
    strncpy(output_file_name, remove_whitespaces(text_buffer), buffer_size);
    file_in.close();

    bool do_print_types{0};
    char msg[buffer_size]{"Table read successfully.\n"};
    const char default_msg[]{"Enter next command:   <open> , <print types> , <close> , <save> , <save as> , <edit> , <exit>\n"};
    while (true)
    {
        system("clear");
        current_table.print_dimensions();
        current_table.print_table();

        if (do_print_types)
        {
            std::cout << '\n';
            current_table.print_types();
            std::cout << '\n';
            do_print_types = 0;
        }

        std::cout << msg << default_msg;
        std::cin.getline(text_buffer, buffer_size);
        // std::cin >> text_buffer;

        if (!strcmp(remove_whitespaces(text_buffer), "open"))
        {
            std::cout << "Enter name of new file: ";
            std::cin.getline(text_buffer, buffer_size);
            std::ifstream file_in(remove_whitespaces(text_buffer));
            current_table = Table(file_in);
            // Table table_new(file_in);
            // current_table = table_new;
            strncpy(output_file_name, remove_whitespaces(text_buffer), buffer_size);
            file_in.close();
            strncpy(msg, "New table opened.\n", buffer_size);
            continue;
        }
        else if (!strcmp(remove_whitespaces(text_buffer), "close"))
        {
            std::cout << "Table closed. Enter name of new file or exit program: ";
            std::cin.getline(text_buffer, buffer_size);
            if (!strcmp(remove_whitespaces(text_buffer), "exit"))
            {
                break;
            }

            std::ifstream file_in(remove_whitespaces(text_buffer));
            current_table = Table(file_in);
            // Table table_new(file_in);
            // current_table = table_new;
            strncpy(output_file_name, remove_whitespaces(text_buffer), buffer_size);
            file_in.close();

            strncpy(msg, "Table opened successfully.\n", buffer_size);
            continue;
        }
        else if (!strcmp(remove_whitespaces(text_buffer), "print types"))
        {
            do_print_types = 1;
            strncpy(msg, "", buffer_size);
            continue;
        }
        else if (!strcmp(remove_whitespaces(text_buffer), "save"))
        {
            std::ofstream file_out(output_file_name);
            current_table.write_to_file(file_out);
            file_out.close();
            strncpy(msg, "Table saved to source file.\n", buffer_size);
            continue;
        }
        else if (!strcmp(remove_whitespaces(text_buffer), "save as"))
        {
            std::cout << "Enter name of destination file: ";
            std::cin.getline(text_buffer, buffer_size);
            strncpy(output_file_name, text_buffer, buffer_size);
            std::ofstream file_out(output_file_name);
            current_table.write_to_file(file_out);
            file_out.close();
            strncpy(msg, "Table saved to new file.\n", buffer_size);
            continue;
        }
        else if (!strcmp(remove_whitespaces(text_buffer), "edit"))
        {
            std::cout << "Enter row index starting from 1: ";
            long row_index;
            std::cin.getline(text_buffer, buffer_size);
            if (is_whole(remove_whitespaces(text_buffer)))
            {
                row_index = str_to_whole(remove_whitespaces(text_buffer)) - 1;
                if (row_index < 0 || row_index > current_table.get_rows())
                {
                    strncpy(msg, "Invalid row index!\n", buffer_size);
                    continue;
                }
            }
            else
            {
                strncpy(msg, "Invalid input for row!\n", buffer_size);
                continue;
            }

            long col_index;
            std::cout << "Enter column index starting from 1: ";
            std::cin.getline(text_buffer, buffer_size);
            if (is_whole(remove_whitespaces(text_buffer)))
            {
                col_index = str_to_whole(remove_whitespaces(text_buffer)) - 1;
                if (col_index < 0 || col_index > current_table.get_cols())
                {
                    strncpy(msg, "Invalid column index!\n", buffer_size);
                    continue;
                }
            }
            else
            {
                strncpy(msg, "Invalid input for column!\n", buffer_size);
                continue;
            }

            std::cout << "Data of current cell (" << row_index << ',' << col_index << ") is: " << current_table.get_text_by_index(row_index, col_index) << '\n';
            std::cout << "Enter new data for cell (" << row_index << ',' << col_index << "): ";
            std::cin.getline(text_buffer, buffer_size);
            current_table.mod_cell(row_index, col_index, text_buffer);
            strncpy(msg, "Cell updated successfully!\n", buffer_size);
            continue;
        }
        else if (!strcmp(text_buffer, "exit"))
        {
            // exit(); //  optional
            break;
        }
        else
        {
            strncpy(msg, "Invalid command!\n", buffer_size);
            continue;
        }
    }
}