#ifndef GLOBALS_H
#define GLOBALS_H

namespace utils
{
    extern char empty_string[1];
}

extern char *remove_whitespaces(char *);

extern bool str_is_in_quotes(const char *);
extern bool str_is_whole_number(const char *);
extern bool str_is_decimal_number(const char *);
extern bool str_is_formula(const char *);

extern bool char_is_operator(char);
extern bool char_is_numeric(char);

extern long str_to_whole(const char *);
extern double str_to_float(const char *);

extern void reverse_string(char *);

extern void long_to_str(char *, long);
extern void number_to_str(char *, double);

#endif // GLOBALS_H