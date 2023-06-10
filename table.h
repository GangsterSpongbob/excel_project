#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <fstream>

#include "cell.h"

class Table
{
private:
    size_t rows, cols;
    Cell *cells;

    // size_t max_width;

    void free();
    void count_rows_cols(std::ifstream &);
    char *parse_cell(const char *, size_t &);
    void build_table(std::ifstream &);
    // void copy_from(const Table &);

public:
    Table();
    Table(std::ifstream &);
    // Table(const Table &);
    // Table operator=(const Table &);
    void print_table() const; // print_types();
    void save() const, save_as(const char *) const, close() const;
    ~Table();
};

#endif // TABLE_H