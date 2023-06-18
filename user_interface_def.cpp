#include "user_interface.h"

#include <iostream>
#include <cstring>

#include "utils.h"
#include "table.h"

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

    while (strcmp(text_buffer, "exit"))
    {
        std::cout << "\033[H\033[J";
        current_table.print_dimensions();
        current_table.print_table();

        if (current_table.get_count_of_invalid_cells() != 0)
        {
            current_table.print_invalid_cells();
        }

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
                if (current_table.mod_cell(row_index, col_index, remove_whitespaces(text_buffer)))
                {
                    strncpy(msg, "Cell updated successfully!\n", buffer_size);
                }
                else
                {
                    strncpy(msg, "Cell was not updated due to invalid data input!\n", buffer_size); // shouldn't happen!
                }
            }
            else if (current_table.string_is_valid_formula(text_buffer))
            {
                if (current_table.mod_with_formula(row_index, col_index, remove_whitespaces(text_buffer)))
                {
                    strncpy(msg, "Cell updated successfully!\n", buffer_size);
                }
                else
                {
                    strncpy(msg, "Cell was updated to \"ERROR!\" due to invalid expression!\n", buffer_size); // also shouldn't happen
                }
            }
            else
            {
                strncpy(msg, "Invalid cell data! Cell will not be updated!\n", buffer_size);
            }
            continue;
        }
        else if (!strcmp(text_buffer, "exit"))
        {
            break;
        }
        else
        {
            strncpy(msg, "Invalid command!\n", buffer_size);
            continue;
        }
    }
}
