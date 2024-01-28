#ifndef LIBTTFB__MACRO_H
#define LIBTTFB__MACRO_H

#include <stdio.h>
extern void abort(void);

#define LOGFILE                                                                          \
    do                                                                                   \
    {                                                                                    \
        fprintf(stderr, "FILE: %s:%d\n", __FILE__, __LINE__);                            \
    } while (0)

#define LOGFUNC                                                                          \
    do                                                                                   \
    {                                                                                    \
        fprintf(stderr, "FUNCTION: %s\n", __func__);                                     \
    } while (0)

#define LOG(...)                                                                         \
    do                                                                                   \
    {                                                                                    \
        LOGFILE;                                                                         \
        LOGFUNC;                                                                         \
        fprintf(stderr, "LOG: ");                                                        \
        fprintf(stderr, __VA_ARGS__);                                                    \
        fprintf(stderr, "\n");                                                           \
    } while (0)

#define TODO(...)                                                                        \
    {                                                                                    \
        LOG("TODO: " #__VA_ARGS__);                                                      \
        abort();                                                                         \
    }                                                                                    \
    while (0)

#endif // LIBTTFB__MACRO_H
