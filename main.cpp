#include "user_interface.h"
#include "table.h"

void test_polymortphic_implementation()
{
    std::ifstream file_in("test1.csv");
    Table table(file_in);
    file_in.close();
    table.full_print();

    table.mod_cell(1, 1, "\"Doe\"");
    table.print_table();

    table.mod_cell(2, 0, "Kur haha");
    table.print_table();

    std::ofstream file_out("foo.csv");
    table.write_to_file(file_out);
    file_out.close();
}

int main()
{
    user_interface();
    // test_polymortphic_implementation();

    return 0;
}