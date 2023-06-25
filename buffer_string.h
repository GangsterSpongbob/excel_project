#ifndef BUFF_H
#define BUFF_H

#include <cstddef>
#include <fstream>

constexpr size_t MAX_BUFFER_SIZE{1024};

class Buffer_string
{
private:
    char text[MAX_BUFFER_SIZE];
    size_t capacity;

public:
    Buffer_string();
    Buffer_string(const char *);
    Buffer_string(const Buffer_string &);

    void copy_from(const Buffer_string &);
    void copy_from(const char *);
    void clear();
    Buffer_string remove_quotes();

    bool is_empty() const;
    bool is_whole() const;
    bool is_decimal() const;
    bool is_quoted() const;

    long to_whole() const;
    double to_decimal() const;

    friend std::ostream &operator<<(std::ostream &, const Buffer_string &);
    Buffer_string getline(std::istream &);

    Buffer_string operator=(const Buffer_string &);
    const char operator[](size_t) const;
    bool operator==(const char *) const;
    bool operator==(const Buffer_string &) const;
    bool operator!=(const char *) const;
    bool operator!=(const Buffer_string &) const;

    char get_char_at_index(size_t) const;
    Buffer_string mod_at_index(size_t, char);
    Buffer_string append(char);
    Buffer_string trim_whitespaces() const;
    Buffer_string reverse() const;
    size_t get_capacity() const;
    const char *get_text_ptr() const;
};

#endif // BUFF_H