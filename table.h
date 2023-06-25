#ifndef TABLE_H
#define TABLE_H

#include <fstream>

#include "cell.h"

using expression_value = double;

class Table
{
private:
    size_t rows, cols;
    Cell *cells;

    void free();
    void copy_from(const Table &);

    void count_rows_and_cols(std::ifstream &);
    void build_table(std::ifstream &);

    expression_value evaluate_expression(const Buffer_string &) const;

public:
    Table();
    Table(std::ifstream &);
    Table(const Table &);
    Table(Table &&);
    Table &operator=(const Table &);
    Table &operator=(Table &&);

    void print_table() const;
    void print_types() const;
    void print_dimensions() const;
    void print_invalid_cells() const;
    void full_print() const; // Currently unused

    const Buffer_string get_text_by_index(size_t, size_t) const;
    expression_value get_numeric_value_by_index(size_t, size_t) const;
    size_t get_rows() const;
    size_t get_cols() const;
    size_t get_count_of_invalid_cells() const;

    void write_to_file(std::ofstream &) const;

    bool mod_cell(size_t, size_t, const Buffer_string &);
    bool mod_cell_with_formula(size_t, size_t, const Buffer_string &);
    bool string_is_valid_formula(const Buffer_string &);

    ~Table();
};

#endif // TABLE_H