#ifndef LIBTTFB__VERSION_H
#define LIBTTFB__VERSION_H

#include <libttfb/export_macro.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        size_t major;
        size_t minor;
        size_t patch;
    } version;

    LIBTTFB_EXPORT version version_new(void);

    LIBTTFB_EXPORT int version_new_from_cstr(version* const ver, char const* const s);

    LIBTTFB_EXPORT int version_cmp(version const lhs, version const rhs);

#ifdef __cplusplus
}
#endif

#endif // LIBTTFB__VERSION_H
