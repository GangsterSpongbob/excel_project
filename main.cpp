#include <iostream>
#include <fstream>
#include <cstring>

#include "cell.h"
#include "utils.h"
#include "table.h"

// void test_formula(const char *formula)
// {
//     std::ifstream in_file("test1.csv");
//     Table table(in_file);
//     in_file.close();

//     table.mod_with_formula(0, 0, formula);
// }

// void test_number_to_str()
// {
//     char buffer1[100];

//     number_to_str(buffer1, 0);
//     std::cout << buffer1 << "\n\n\n";

//     char buffer2[100];
//     number_to_str(buffer2, -123456789);
//     std::cout << buffer2 << "\n\n\n";

//     char buffer3[100];
//     number_to_str(buffer3, 987654321);
//     std::cout << buffer3 << "\n\n\n";

//     char buffer4[100];
//     number_to_str(buffer4, -3.14159);
//     std::cout << buffer4 << "\n\n\n";

//     char buffer5[100];
//     number_to_str(buffer5, 123.456);
//     std::cout << buffer5 << "\n\n\n";

//     char buffer6[100];
//     number_to_str(buffer6, 0.0000123456);
//     std::cout << buffer6 << "\n\n\n";
// }

int main()
{
    user_interface();
    return 0;
}
