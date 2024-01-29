#ifndef TTFB__METHOD_H
#define TTFB__METHOD_H

#include <libttfb/ttfb.h>

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

    TTFB_EXPORT int cmp_name_arch(json_object** lhs, json_object** rhs);

    TTFB_EXPORT int packagesets_difference(ttfb_branch const lhs, ttfb_branch const rhs);

    TTFB_EXPORT int cmp_name_arch_version(json_object** lhs, json_object** rhs);

    TTFB_EXPORT int packagesets_difference_by_version(ttfb_branch const lhs,
                                                      ttfb_branch const rhs);

#ifdef __cplusplus
}
#endif

#endif //  TTFB__METHOD_H
