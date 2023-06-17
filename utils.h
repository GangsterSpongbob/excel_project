#ifndef GLOBALS_H
#define GLOBALS_H

#include "table.h"

namespace utils
{
    extern char empty_string[1];
}

extern char *remove_whitespaces(char *);

extern bool is_string(const char *);
extern bool is_whole(const char *);
extern bool is_float(const char *);
extern bool is_formula(const char *);

extern bool is_operator(char);
extern bool is_numeric(char);

extern long str_to_whole(const char *);
extern double str_to_float(const char *);

extern void reverse_string(char *);

extern void long_to_str(char *, long);
extern void number_to_str(char *, double);

#endif // GLOBALS_H