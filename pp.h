#ifndef PP_H
#define PP_H
/*
 * Helper function (macro) for people who loves printf-debugging.
 * This dumps content of any C data/structure/expression without prior
 * knowledge of actual format. Works just like "p" or "pp" in Ruby.
 *
 * Usage:
 *   p(anyexpr);
 *
 * NOTE:
 * - Program should be compiled with "-g" and preferrably, with "-O0".
 *
 * FIXME:
 * - Would be better if this doesn't depend on external debugger to run.
 * - Needs improvement on a way to prevent variable from being optimized away.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Counts number of actual arguments.
#define COUNT_(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define COUNT(...) COUNT_(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

// Dispatches macro call by number of actual arguments.
// Following is an example of actual macro expansion performed in case
// of 3 arguments:
//
//   p(a, b, c)
//   -> FUNC_N(p, COUNT(a, b, c), a, b, c)
//   -> FUNC_N(p, 3, a, b, c)
//   -> p_3(a, b, c)
//
// This means calling with simple "p(...)" is fine for any number of
// arguments, simulating "true" variadic macro.
#define CONCAT(name, count) name##count
#define FUNC_N(name, count, ...) CONCAT(name, count)(__VA_ARGS__)

// Forbids variable from being optimized out, so debugger can access it.
//
// FIXME:
// - Current implementation does not work with certain type of symbols
#define ENSURE(...) FUNC_N(ENSURE_, COUNT(__VA_ARGS__), __VA_ARGS__)
#define ENSURE_1(a) asm(""::"m"(a))
#define ENSURE_2(a, ...) do { ENSURE_1(a); ENSURE_1(__VA_ARGS__); } while (0)
#define ENSURE_3(a, ...) do { ENSURE_1(a); ENSURE_2(__VA_ARGS__); } while (0)
#define ENSURE_4(a, ...) do { ENSURE_1(a); ENSURE_3(__VA_ARGS__); } while (0)
#define ENSURE_5(a, ...) do { ENSURE_1(a); ENSURE_4(__VA_ARGS__); } while (0)
#define ENSURE_6(a, ...) do { ENSURE_1(a); ENSURE_5(__VA_ARGS__); } while (0)
#define ENSURE_7(a, ...) do { ENSURE_1(a); ENSURE_6(__VA_ARGS__); } while (0)
#define ENSURE_8(a, ...) do { ENSURE_1(a); ENSURE_7(__VA_ARGS__); } while (0)

// Dumps content of given symbol (uses external GDB for now)
//
// NOTE:
// - Should use libbfd instead of gdb? (but that'll add complexity...)
#define PP_D(...) do {                                \
        char *arg[] = { __VA_ARGS__, NULL };          \
        char **argp = arg;                            \
        char  cmd[1024];                              \
        FILE *tmp = tmpfile();                        \
        fprintf(tmp, "attach %d\n", getpid());        \
        fprintf(tmp, "frame 2\n");                    \
        while (*argp)                                 \
            fprintf(tmp, "p %s\n", *argp++);          \
        fprintf(tmp, "detach\n");                     \
        fflush(tmp);                                  \
        sprintf(cmd, "gdb -batch -x /proc/%d/fd/%d",  \
                getpid(), fileno(tmp));               \
        system(cmd);                                  \
        fclose(tmp);                                  \
    } while (0)

#define PP(...) do {                                  \
        FUNC_N(PP_, COUNT(__VA_ARGS__), __VA_ARGS__); \
        ENSURE(__VA_ARGS__);                          \
    } while (0)
#define PP_1(a) do { PP_D(#a); } while (0)
#define PP_2(a,b) do { PP_D(#a,#b); } while (0)
#define PP_3(a,b,c) do { PP_D(#a,#b,#c); } while (0)
#define PP_4(a,b,c,d) do { PP_D(#a,#b,#c,#d); } while (0)
#define PP_5(a,b,c,d,e) do { PP_D(#a,#b,#c,#d,#e); } while (0)
#define PP_6(a,b,c,d,e,f) do { PP_D(#a,#b,#c,#d,#e,#f); } while (0)
#define PP_7(a,b,c,d,e,f,g) do { PP_D(#a,#b,#c,#d,#e,#f,#g); } while (0)
#define PP_8(a,b,c,d,e,f,g,h) do { PP_D(#a,#b,#c,#d,#e,#f,#g,#h); } while (0)

// Comment this out if you think this is too aggressive.
#define p PP

#endif
