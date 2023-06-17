#include "table.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cmath>

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

const char *Table::get_text_by_index(size_t row_index, size_t col_index) const
{
    if (row_index < rows && col_index < cols && row_index >= 0 && col_index >= 0)
    {
        return cells[row_index * cols + col_index].get_text();
    }

    return utils::empty_string;
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

            if (is_string(text_buffer) || is_whole(text_buffer) || is_float(text_buffer))
            {
                current_table.mod_cell(row_index, col_index, remove_whitespaces(text_buffer));
                strncpy(msg, "Cell updated successfully!\n", buffer_size);
            }
            else if (current_table.string_is_valid_formula(text_buffer))
            {
                current_table.mod_with_formula(row_index, col_index, remove_whitespaces(text_buffer));
                strncpy(msg, "Cell updated successfully!\n", buffer_size);
            }
            else
            {
                strncpy(msg, "Invalid cell data!\n", buffer_size);
            }
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

double Table::evaluate_expression(const char *calc) const
{
    if (calc == nullptr)
    {
        throw std::invalid_argument("Argument doesn't exist.");
    }

    size_t line_index{0};
    if (calc[0] == '=')
    {
        line_index = 1;
    }

    const size_t stack_buffer_size{128};
    double numbers_stack_1[stack_buffer_size]{};
    char operators_stack_1[stack_buffer_size]{};
    size_t numbers_index{0};
    size_t operators_index{0};
    bool last_was_number{0};

    const size_t string_buffer_size{1024};
    const size_t small_string_buffer_size{32};

    try
    {
        while (calc[line_index] != '\0')
        {
            if (calc[line_index] == ' ')
            {
                line_index++;
            }

            else if (calc[line_index] == '(')
            {
                line_index++;
                char bracket_buffer[string_buffer_size]{};
                size_t bracket_index{0};
                size_t bracket_difference{1};
                while (bracket_difference && calc[line_index] != '\0')
                {
                    if (calc[line_index] == '(')
                    {
                        bracket_difference++;
                    }
                    else if (calc[line_index] == ')')
                    {
                        bracket_difference--;
                        if (bracket_difference == 0)
                        {
                            continue;
                        }
                    }

                    bracket_buffer[bracket_index++] = calc[line_index++];
                }

                double bracket_value;
                try
                {
                    bracket_value = evaluate_expression(bracket_buffer);
                }
                catch (const std::logic_error &le) // Assess for other possible error types
                {
                    std::cerr << "In parentheses: " << le.what() << '\n';
                    throw std::logic_error(le.what());
                }
                if (bracket_difference != 0)
                {
                    throw std::invalid_argument("Mismatched parentheses.");
                }

                numbers_stack_1[numbers_index++] = bracket_value;
                line_index++;
            }

            else if (is_numeric(calc[line_index]))
            {
                char current_number[small_string_buffer_size]{};
                size_t digit_index{0};

                while (is_numeric(calc[line_index]) && calc[line_index] != '\0')
                {
                    current_number[digit_index++] = calc[line_index++];
                }

                if (!last_was_number && is_whole(current_number))
                {
                    numbers_stack_1[numbers_index++] = str_to_whole(current_number);
                    last_was_number = 1;
                }
                else if (!last_was_number && is_float(current_number))
                {
                    numbers_stack_1[numbers_index++] = str_to_float(current_number);
                    last_was_number = 1;
                }
                else if (last_was_number)
                {
                    throw std::invalid_argument("Two consecutive numbers with no operator inbetween.");
                }
                else
                {
                    throw std::invalid_argument("One or more values are invalid.");
                }
            }

            else if (calc[line_index] == 'R')
            {
                line_index++;
                char row_index[small_string_buffer_size]{""};
                size_t row_char_index{0};
                while (calc[line_index] >= '0' && calc[line_index] <= '9' && calc[line_index] != '\0')
                {
                    row_index[row_char_index++] = calc[line_index++];
                }

                if (calc[line_index] != 'C')
                {
                    throw std::invalid_argument("Wrong cell format.");
                }

                char col_index[small_string_buffer_size]{""};
                size_t col_char_index{0};
                line_index++;
                while (calc[line_index] >= '0' && calc[line_index] <= '9' && calc[line_index] != '\0')
                {
                    col_index[col_char_index++] = calc[line_index++];
                }

                if (!is_whole(row_index) || !is_whole(col_index))
                {
                    throw std::invalid_argument("Invalid cell indice.");
                }
                if (last_was_number)
                {
                    throw std::invalid_argument("Two operators with no numbers inbetween.");
                }

                double indexed_cell_value{0.0};
                std::cout << "Row index is: " << str_to_whole(row_index) << " and col index is : " << str_to_whole(col_index) << '\n'; // REMOVE!
                try
                {
                    indexed_cell_value = get_numeric_value_by_index(str_to_whole(row_index) - 1, str_to_whole(col_index) - 1);
                }
                catch (const std::invalid_argument &ia)
                {
                    std::cerr << ia.what() << '\n';
                    throw std::invalid_argument("Error while fetching cell data.");
                }

                numbers_stack_1[numbers_index++] = indexed_cell_value;
                last_was_number = 1;
            }

            else if (is_operator(calc[line_index]))
            {
                operators_stack_1[operators_index++] = calc[line_index++];
                last_was_number = 0;
            }

            else
            {
                throw std::invalid_argument("Invalid character.");
            }
        }
    }
    catch (const std::invalid_argument &ia)
    {
        std::cerr << ia.what() << '\n';
        throw std::logic_error("Due to previous errors, cell data will be set to \"ERROR\"");
    }

    size_t numbers_count = numbers_index;
    size_t operators_count = operators_index;

    if (operators_count + 1 != numbers_count)
    {
        throw std::invalid_argument("There exists a number pair without an operator, or an operator with only one number");
    }

    numbers_index = operators_index = 0;
    double numbers_stack_2[stack_buffer_size]{};
    char operators_stack_2[stack_buffer_size]{};

    for (size_t i = 0; i < operators_count; i++)
    {
        if (operators_stack_1[i] == '^')
        {
            if (i > numbers_count - 1)
            {
                throw std::out_of_range("Accessed invalid index."); // Should not happen.
            }
            double temp_result{numbers_stack_1[i]};
            while (operators_stack_1[i] == '^' && i < operators_count)
            {
                try
                {
                    if (i > numbers_count - 1)
                    {
                        throw std::out_of_range("Accessed invalid index."); // Should not happen.
                    }
                    temp_result = pow(temp_result, numbers_stack_1[i + 1]);
                    i++;
                }
                catch (const std::logic_error &le)
                {
                    std::cerr << le.what() << '\n';
                    throw std::logic_error("Calculation error.");
                }
            }
            numbers_stack_2[numbers_index++] = temp_result;
            if (i < operators_count)
            {
                operators_stack_2[operators_index++] = operators_stack_1[i];
            }
        }
        else
        {
            operators_stack_2[operators_index++] = operators_stack_1[i];
            numbers_stack_2[numbers_index++] = numbers_stack_1[i];
        }
    }
    if (operators_stack_1[operators_count - 1] != '^')
    {
        numbers_stack_2[numbers_index++] = numbers_stack_1[numbers_count - 1]; // add last number if not used in calculation;
    }

    numbers_count = numbers_index;
    operators_count = operators_index;

    numbers_index = operators_index = 0;
    double numbers_stack_3[stack_buffer_size]{};
    char operators_stack_3[stack_buffer_size]{};

    for (size_t i = 0; i < operators_count; i++)
    {
        if (operators_stack_2[i] == '/' || operators_stack_2[i] == '*')
        {
            if (i > numbers_count - 1)
            {
                throw std::out_of_range("Accessed invalid index."); // Should not happen.
            }
            double temp_result{numbers_stack_2[i]};
            while (operators_stack_2[i] == '/' || operators_stack_2[i] == '*' && i < operators_count)
            {
                try
                {
                    if (i > numbers_count - 1)
                    {
                        throw std::out_of_range("Accessed invalid index."); // Should not happen.
                    }

                    if (operators_stack_2[i] == '/')
                    {
                        if (numbers_stack_2[i + 1] != 0)
                        {
                            temp_result /= numbers_stack_2[i + 1];
                        }
                        else
                        {
                            throw std::invalid_argument("Division by 0.");
                        }
                    }
                    else if (operators_stack_2[i] == '*')
                    {
                        temp_result *= numbers_stack_2[i + 1];
                    }

                    i++;
                }
                catch (const std::logic_error &le)
                {
                    std::cerr << le.what() << '\n';
                    throw std::logic_error("Calculation error.");
                }
            }
            numbers_stack_3[numbers_index++] = temp_result;
            if (i < operators_count)
            {
                operators_stack_3[operators_index++] = operators_stack_2[i];
            }
        }
        else
        {
            operators_stack_3[operators_index++] = operators_stack_2[i];
            numbers_stack_3[numbers_index++] = numbers_stack_2[i];
        }
    }
    if (operators_stack_2[operators_count - 1] != '/' && operators_stack_2[operators_count - 1] != '*')
    {
        numbers_stack_3[numbers_index++] = numbers_stack_2[numbers_count - 1]; // add last number if not used in calculation;
    }

    for (size_t i = 0; i < operators_index; i++)
    {
        if (operators_stack_3[i] != '-' && operators_stack_3[i] != '+')
        {
            throw std::logic_error("Unexpected operator error."); // also sohuldn't happen
        }
    }

    double total{numbers_stack_3[0]};

    for (size_t i = 0; i < operators_index; i++)
    {
        if (i > numbers_index - 1)
        {
            throw std::out_of_range("Accessed invalid index."); // Should not happen.
        }

        if (operators_stack_3[i] == '+')
        {
            total += numbers_stack_3[i + 1];
        }
        else if (operators_stack_3[i] == '-')
        {
            total -= numbers_stack_3[i + 1];
        }
    }

    return total; // Implement rest of logic here.
}

bool Table::string_is_valid_formula(const char *expression)
{
    try
    {
        evaluate_expression(expression);
        return 1;
    }
    catch (const std::logic_error &le)
    {
        return 0;
    }
}

void Table::mod_with_formula(size_t row_index, size_t col_index, const char *expression)
{
    double mod_value;
    try
    {
        mod_value = evaluate_expression(expression);
        const size_t buffer_size(1024);
        char str_buffer[buffer_size]{};
        number_to_str(str_buffer, mod_value);
        mod_cell(row_index, col_index, str_buffer);
    }
    catch (const std::logic_error &le)
    {
        std::cerr << le.what() << '\n';
        char error_string[7]{"ERROR!"};
        mod_cell(row_index, col_index, error_string);
    }
}