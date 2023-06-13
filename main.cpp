#include <iostream>
#include <fstream>
#include <cstring>

#include "cell.h"
#include "utils.h"
#include "table.h"

void test_table_constructor()
{
    // Test 1: Valid input file
    {
        std::ifstream inputFile("test1.csv");
        Table table(inputFile);
        inputFile.close();

        std::cout << "Test 1: Printing table:\n";
        table.print_dimensions();
        table.print_table();
        table.print_types();

        std::ofstream outputFile("test1out.csv");
        table.write_to_file(outputFile);
        outputFile.close();

        std::ifstream input2("test1out.csv");
        Table table2(input2);
        input2.close();

        std::cout << "Test 1: Re-Printing table:\n";
        table2.print_dimensions();
        table2.print_table();
        table2.print_types();
    }

    // Test 2: Empty input file
    {
        std::ifstream inputFile("test2.csv");
        Table table(inputFile);
        inputFile.close();

        std::cout << "Test 2: Printing table:\n";
        table.print_dimensions();
        table.print_table();
        table.print_types();
    }

    // Test 3: Input file with multiple rows and columns
    {
        std::ifstream inputFile("test3.csv");
        Table table(inputFile);
        inputFile.close();

        std::cout << "Test 3: Printing table:\n";
        table.print_dimensions();
        table.print_table();
        table.print_types();
    }
}

int main()
{
    user_interface();
    return 0;
}