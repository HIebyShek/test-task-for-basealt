#ifndef LIBTTFB__PACKAGE_H
#define LIBTTFB__PACKAGE_H

#include <libttfb/export_macro.h>
#include <libttfb/version.h>

#include <json_types.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        char const* name;
        size_t epoch;
        version ver;
        char const* release;
        char const* arch;
        char const* disttag;
        size_t buildtime;
        char const* source;
    } package;

    LIBTTFB_EXPORT package package_new(void);

    // lifetime of char* in package is equal to lifetime of obj
    LIBTTFB_EXPORT int package_init_from_json(package* const pkg,
                                              json_object const* const obj);

#ifdef __cplusplus
}
#endif

#endif // LIBTTFB__PACKAGE_H
