#include "user_interface.h"

#include <iostream>

#include "utils.h"
#include "table.h"

void user_interface()
{
    Buffer_string text_buff{"asd"};

    std::cout << "Enter name of existing file to read from: ";
    text_buff.getline(std::cin);
    std::cout << "Buffer: " << text_buff << '\n';

    std::ifstream file_in(text_buff.get_text_ptr());
    Table current_table(file_in);
    Buffer_string output_file_name(text_buff);
    file_in.close();

    bool do_print_types{0};
    Buffer_string msg_current{"Table read successfully.\n"};
    Buffer_string msg_default{"Enter next command:   <open> , <print types> , <close> , <save> , <save as> , <edit> , <exit>\n"};

    while (text_buff != "exit")
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

        std::cout << msg_current << msg_default;

        text_buff.getline(std::cin);

        if (text_buff == "open")
        {
            std::cout << "Enter name of new file: ";
            text_buff.getline(std::cin);
            std::ifstream file_in(text_buff.get_text_ptr());
            current_table = Table(file_in);
            output_file_name = text_buff;
            file_in.close();
            msg_current = "New table opened.\n";
            continue;
        }
        else if (text_buff == "close")
        {
            std::cout << "Table closed. Enter name of new file or exit program: ";
            text_buff.getline(std::cin);

            if (text_buff == "exit")
            {
                break;
            }

            std::ifstream file_in(text_buff.get_text_ptr());
            current_table = Table(file_in);
            output_file_name = text_buff;
            file_in.close();

            msg_current = "Table opened successfully.\n";
            continue;
        }
        else if (text_buff == "print types")
        {
            do_print_types = 1;
            msg_current = "";
            continue;
        }
        else if (text_buff == "save")
        {
            std::ofstream file_out(output_file_name.get_text_ptr());
            current_table.write_to_file(file_out);
            file_out.close();
            msg_current = "Table saved to source file.\n";
            continue;
        }
        else if (text_buff == "save as")
        {
            std::cout << "Enter name of destination file: ";
            text_buff.getline(std::cin);
            output_file_name = text_buff;
            std::ofstream file_out(output_file_name.get_text_ptr());
            current_table.write_to_file(file_out);
            file_out.close();
            msg_current = "Table saved to new file.\n";
            continue;
        }
        else if (text_buff == "edit")
        {
            std::cout << "Enter row index starting from 1: ";

            size_t row_index;
            text_buff.getline(std::cin);
            if (text_buff.is_whole())
            {
                row_index = text_buff.to_whole() - 1;
                if (row_index < 0 || row_index > current_table.get_rows())
                {
                    msg_current = "Invalid row index.\n";
                    continue;
                }
            }
            else
            {
                msg_current = "Invalid input for row.\n";
                continue;
            }

            size_t col_index;
            std::cout << "Enter column index starting from 1: ";
            text_buff.getline(std::cin);
            if (text_buff.to_whole())
            {
                col_index = text_buff.to_whole() - 1;
                if (col_index < 0 || col_index > current_table.get_cols())
                {
                    msg_current = "Invalid column index!\n";
                    continue;
                }
            }
            else
            {
                msg_current = "Invalid input for column!\n";
                continue;
            }

            std::cout << "Data of current cell (" << row_index << ',' << col_index << ") is: " << current_table.get_text_by_index(row_index, col_index) << '\n';
            std::cout << "Enter new data for cell (" << row_index << ',' << col_index << "): ";
            text_buff.getline(std::cin);

            if (text_buff.is_quoted() || text_buff.is_whole() || text_buff.is_decimal())
            {
                if (current_table.mod_cell(row_index, col_index, text_buff))
                {
                    msg_current = "Cell updated successfully.\n";
                }
                else
                {
                    msg_current = "Cell was not updated due to invalid data input!\n";
                }
            }
            else if (current_table.string_is_valid_formula(text_buff))
            {
                if (current_table.mod_cell_with_formula(row_index, col_index, text_buff))
                {
                    msg_current = "Cell updated successfully.\n";
                }
                else
                {
                    msg_current = "Cell was updated to \"ERROR!\" due to invalid expression!\n";
                }
            }
            else
            {
                msg_current = "Invalid cell data! Cell will not be updated!\n";
            }
            continue;
        }
        else if (text_buff == "exit")
        {
            break;
        }
        else
        {
            msg_current = "Invalid command!\n";
            continue;
        }
    }
}
