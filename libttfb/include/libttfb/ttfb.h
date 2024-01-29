#ifndef LIBTTFB__TTFB_H
#define LIBTTFB__TTFB_H

#include <libttfb/export_macro.h>

#include <json-c/json.h>
#include <json_object.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        TTFB_BRANCH_sisyphus = 0,
        TTFB_BRANCH_p10,
        TTFB_BRANCH_p9,

        TTFB_BRANCH_ENUM_SIZE,
        TTFB_BRANCH_UNKNOWN = TTFB_BRANCH_ENUM_SIZE
    } ttfb_branch;

    LIBTTFB_EXPORT const char* ttfb_branch_to_cstr(const ttfb_branch branch);
    LIBTTFB_EXPORT ttfb_branch ttfb_branch_from_cstr(char const* const branch);

    LIBTTFB_EXPORT json_object* ttfb_get_binary_packages(const ttfb_branch branch);

    typedef int (*ttfb_json_object_compare)(json_object**, json_object**);

    LIBTTFB_EXPORT json_object* ttfb_set_difference(json_object* const lhs,
                                                    json_object* const rhs,
                                                    ttfb_json_object_compare const cmp);

#ifdef __cplusplus
}
#endif

#endif // LIBTTFB__TTFB_H
