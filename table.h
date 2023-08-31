#ifndef TABLE_H
#define TABLE_H

#include <fstream>
#include <iostream>
#include <iomanip>

#include "cell_factory.h"
#include "row.h"
#include "cell.h"
#include "utils.h"

using expression_value = double;

constexpr size_t MAX_COLUMN_COUNT{32};

class Table
{
private:
    size_t row_count, column_count;
    Row data[MAX_COLUMN_COUNT];

    void set_all_to_empty();
    void count_rows_and_cols(std::ifstream &);

    expression_value evaluate_expression(const Buffer_string &) const;

public:
    Table();
    Table(std::ifstream &);
    Table(const Table &src) = default;
    Table &operator=(const Table &) = default;
    ~Table() = default;

    Table(Table &&src) = default;
    Table &operator=(Table &&) = default;

    void print_table() const;
    void print_types() const;
    void print_dimensions() const;
    void print_invalid_cells() const;
    void full_print() const; // Currently unused

    size_t get_rows() const;
    size_t get_cols() const;
    size_t get_count_of_invalid_cells() const;

    double get_value_at_index(size_t, size_t) const;

    void write_to_file(std::ofstream &) const;

    bool mod_cell(size_t, size_t, const Buffer_string &);
    bool mod_cell_with_formula(size_t, size_t, const Buffer_string &);
    bool string_is_valid_formula(const Buffer_string &);
};

#endif // TABLE_H