#include "table.h"

#include <cmath>

#include "utils.h"

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
                catch (const std::logic_error &le)
                {
                    std::cerr << "In parentheses: " << le.what() << '\n';
                    throw std::logic_error(le.what());
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Something else went wrong.";
                }
                if (bracket_difference != 0)
                {
                    throw std::invalid_argument("Mismatched parentheses.");
                }

                numbers_stack_1[numbers_index++] = bracket_value;
                line_index++;
            }

            else if (char_is_numeric(calc[line_index]))
            {
                char current_number[small_string_buffer_size]{};
                size_t digit_index{0};

                while (char_is_numeric(calc[line_index]) && calc[line_index] != '\0')
                {
                    current_number[digit_index++] = calc[line_index++];
                }

                if (!last_was_number && str_is_whole_number(current_number))
                {
                    numbers_stack_1[numbers_index++] = str_to_whole(current_number);
                    last_was_number = 1;
                }
                else if (!last_was_number && str_is_decimal_number(current_number))
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

                if (!str_is_whole_number(row_index) || !str_is_whole_number(col_index))
                {
                    throw std::invalid_argument("Invalid cell indice.");
                }
                if (last_was_number)
                {
                    throw std::invalid_argument("Two operators with no numbers inbetween.");
                }

                double indexed_cell_value{0.0};
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

            else if (char_is_operator(calc[line_index]))
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

    return total;
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

bool Table::mod_cell_with_formula(size_t row_index, size_t col_index, const char *expression)
{
    double mod_value;
    try
    {
        mod_value = evaluate_expression(expression);
        const size_t buffer_size(1024);
        char str_buffer[buffer_size]{};
        number_to_str(str_buffer, mod_value);
        return mod_cell(row_index, col_index, str_buffer);
    }
    catch (const std::logic_error &le)
    {
        std::cerr << le.what() << '\n';
        char error_string[7]{"ERROR!"};
        mod_cell(row_index, col_index, error_string);
        return 0;
    }
}