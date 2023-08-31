#ifndef ROW_H
#define ROW_H

#include <cstddef>

#include "cell.h"
#include "cell_factory.h"

constexpr size_t MAX_ROW_SIZE{32};

class Row
{
private:
    Cell *cells[MAX_ROW_SIZE];
    size_t size;
    void free();
    void copy_from(const Row &);
    void move_from(Row &&);
    void set_all_empty();

public:
    Row() { set_all_empty(); }
    Row(const Buffer_string &);
    Row(const Row &src) { copy_from(src); }
    Row(Row &&src) { move_from(std::move(src)); }
    ~Row() { free(); }

    Row *operator=(const Row &);
    Row *operator=(Row &&);

    size_t get_cols() const { return size; }
    const Cell *operator[](size_t index) const { return cells[index]; }
    Row &modify_cell_at_index(size_t, const Buffer_string &);

    Row &clear();
};

#endif // ROW_H