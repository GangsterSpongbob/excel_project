#include "cell.h"
#include "globals.h"

int main()
{
    Cell cell1;
    Cell cell2((long)342);
    Cell cell3("Foo Bar");
    printData(cell1);
    printData(cell2);
    printData(cell3);
}