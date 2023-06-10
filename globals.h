#ifndef GLOBALS_H
#define GLOBALS_H

extern void copy_dynamic_str(char *&, const char *); // No deallocation!
extern char *remove_whitespaces(char *);

extern bool is_string(const char *);
extern bool is_whole(const char *);
extern bool is_float(const char *);
extern bool is_formula(const char *);

extern long str_to_whole(const char *);
extern double str_to_float(const char *);

// extern void exit();
// extern void open(const char *);
// extern void close();
// extern void save();
// extern void save_as(const char *);

// extern void user_interface(); Maybe add to new .h

#endif // GLOBALS_H