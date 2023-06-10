#include <iostream>
#include <fstream>
#include <cstring>

#include "cell.h"
#include "globals.h"
#include "table.h"

void test_table_constructor()
{
    // Test 1: Valid input file
    {
        std::ifstream inputFile("test1.csv");
        Table table(inputFile);
        inputFile.close();

        std::cout << "Test 1: Printing table:\n";
        table.print_table();
    }

    // Test 2: Empty input file
    {
        std::ifstream inputFile("test2.csv");
        Table table(inputFile);
        inputFile.close();

        std::cout << "Test 2: Printing table:\n";
        table.print_table();
    }

    // Test 3: Input file with multiple rows and columns
    {
        std::ifstream inputFile("test3.csv");
        Table table(inputFile);
        inputFile.close();

        std::cout << "Test 3: Printing table:\n";
        table.print_table();
    }
}

int main()
{
    test_table_constructor();

    return 0;
}