#include <iostream>

#include "cell.h"
#include "globals.h"

void printType(const Cell &c1)
{
    switch (c1.get_type())
    {
    case DataType::Integer:
        std::cout << "Integer\n";
        break;
    case DataType::FloatingPoint:
        std::cout << "FloatingPoint\n";
        break;
    case DataType::CharString:
        std::cout << "CharString\n";
        break;
    case DataType::Formula:
        std::cout << "Formula\n";
        break;
    case DataType::Invalid:
        std::cout << "Invalid\n";
        break;

    default:
        break;
    }
}

int main()
{
    // Create a Cell object with default constructor
    Cell cell1;
    printType(cell1);
    cell1.print_data(); // Output:

    // Create a Cell object with a char string
    Cell cell2("\"Hello, world!\"");
    printType(cell2);
    cell2.print_data(); // Output: Hello, world!

    // Create a Cell object with an integer value
    Cell cell3("42");
    printType(cell3);
    cell3.print_data(); // Output: 42

    // Create a Cell object with a floating-point value
    Cell cell4("3.14");
    printType(cell4);
    cell4.print_data(); // Output: 3.14

    // Assign a Cell object to another
    Cell cell5 = cell2;
    printType(cell5);
    cell5.print_data(); // Output: Hello, world!

    // Modify the original Cell object
    cell2 = "Goodbye, world!";
    printType(cell2);
    cell2.print_data(); // Output: Goodbye, world!
    printType(cell5);
    cell5.print_data(); // Output: Hello, world!

    return 0;
}