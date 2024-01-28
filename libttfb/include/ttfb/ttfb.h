#ifndef LIBTTFB__TTFB_H
#define LIBTTFB__TTFB_H

#include <json-c/json.h>
#include <json_object.h>

#ifndef TTFB_EXPORT
#if defined(_MSC_VER)
#define TTFB_EXPORT __declspec(dllexport)
#else
#define TTFB_EXPORT extern
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    typedef enum
    {
        TTFB_BRANCH_sisyphus,
        TTFB_BRANCH_p10,
        TTFB_BRANCH_p9,
    } ttfb_branch;

    TTFB_EXPORT const char* ttfb_branch_to_cstr(const ttfb_branch branch);

    TTFB_EXPORT json_object* ttfb_get_binary_packages(const ttfb_branch branch);

    typedef enum
    {
        TTFB_COMPARE_RESULT_LESS = -1,
        TTFB_COMPARE_RESULT_EQUAL = 0,
        TTFB_COMPARE_RESULT_GREATER = 1,
    } ttfb_compare_result;
    typedef ttfb_compare_result (*ttfb_json_object_compare)(void const* const,
                                                            void const* const);

    TTFB_EXPORT json_object* ttfb_set_difference(json_object* const lhs,
                                                 json_object* const rhs,
                                                 ttfb_json_object_compare cmp);

#ifdef __cplusplus
}
#endif

#endif // LIBTTFB__TTFB_H
