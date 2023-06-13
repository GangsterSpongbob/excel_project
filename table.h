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

    void free();
    void count_rows_cols(std::ifstream &);
    // char *parse_cell(const char *, size_t &);
    void build_table(std::ifstream &);
    void copy_from(const Table &);

public:
    Table();
    Table(std::ifstream &);
    Table(const Table &);
    Table &operator=(const Table &);
    void print_table() const, print_types() const, print_dimensions() const;
    char *get_text_by_index(size_t, size_t) const;
    void write_to_file(std::ofstream &) const;
    void full_print() const;
    size_t get_rows() const, get_cols() const;
    void mod_cell(size_t, size_t, char *);
    ~Table();
};

extern Table open_table(const char *);
extern void edit_cell(const char *);
extern void save_table();
extern void save_table_as(const char *);
extern void close_table();
extern void exit();

extern void user_interface();

#endif // TABLE_H