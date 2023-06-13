#ifndef GLOBALS_H
#define GLOBALS_H

namespace utils
{
    extern char empty_string[1];
}

extern char *remove_whitespaces(char *);

extern bool is_string(const char *);
extern bool is_whole(const char *);
extern bool is_float(const char *);
extern bool is_formula(const char *);

extern long str_to_whole(const char *);
extern double str_to_float(const char *);

#endif // GLOBALS_H