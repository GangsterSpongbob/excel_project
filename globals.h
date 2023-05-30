#pragma once

extern void copy_string(char *&, const char *);
extern bool is_string(const char *);
extern bool is_whole(const char *);
extern bool is_float(const char *);
extern bool is_formula(const char *);
extern long str_to_whole(const char *);
extern double str_to_float(const char *);
// extern int countChars(long);
// extern int countChars(double);