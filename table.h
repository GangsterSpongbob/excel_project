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
    void copy_from(const Table &);

    void count_rows_cols(std::ifstream &);
    void build_table(std::ifstream &);

    double evaluate_expression(const char *) const;

public:
    Table();
    Table(std::ifstream &);
    Table(const Table &);
    Table &operator=(const Table &);

    void print_table() const;
    void print_types() const;
    void print_dimensions() const;
    void full_print() const;

    const char *get_text_by_index(size_t, size_t) const;
    double get_numeric_value_by_index(size_t, size_t) const;
    size_t get_rows() const, get_cols() const;

    void write_to_file(std::ofstream &) const;

    void mod_cell(size_t, size_t, char *); // const char* ?
    void mod_with_formula(size_t, size_t, const char *);
    bool string_is_valid_formula(const char *);

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