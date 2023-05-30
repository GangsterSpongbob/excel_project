#pragma once

#include <iostream>

class Cell;

class Table
{
private:
    size_t rows, cols;
    Cell *cells;
    void free();

public:
    Table();
    void print_table() const, save() const, save_as() const;
    ~Table();
};

void Table::free()
{
    rows = cols = 0;
    delete[] cells;
    cells = nullptr;
}

Table::Table() : rows(0), cols(0), cells(nullptr) {}

Table::~Table()
{
    free();
}