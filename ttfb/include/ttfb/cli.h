#ifndef TTFB__CLI_H
#define TTFB__CLI_H

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

    typedef enum
    {
        METHOD_PACKAGESETS_DIFFERENCE = 0,

        // result will contain packages, version of
        // which in first set greater than in
        // second.
        METHOD_PACKAGESETS_DIFFERENCE_BY_VERSION,

        METHOD_PACKAGESETS_ENUM_SIZE, // should always be last
        METHOD_PACKAGESETS_UNKNOWN = METHOD_PACKAGESETS_ENUM_SIZE
    } method;

    TTFB_EXPORT method parse_method(char const* const m);

    typedef struct
    {
        char const* self_filename;
        method fn;
        ttfb_branch first;
        ttfb_branch second;
    } args;

    typedef enum
    {
        PARSE_ARGS_ERROR_OK = 0,
        PARSE_ARGS_ERROR_TOO_FEW_ARGUMENTS,
        PARSE_ARGS_ERROR_UNKNOWN_METHOD,
        PARSE_ARGS_ERROR_UNKNOWN_BRANCH,
    } parse_args_error;

    TTFB_EXPORT char const* parse_args_error_to_cstr(parse_args_error const errc);

    TTFB_EXPORT parse_args_error parse_args(int const argc,
                                            char const* const* const argv,
                                            args* const ret);

#ifdef __cplusplus
}
#endif

#endif // TTFB__CLI_H
