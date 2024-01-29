#include <ttfb/cli.h>

#include "macro.h"

#include <libttfb/ttfb.h>

#include <assert.h>
#include <string.h>

method
parse_method(char const* const m)
{
    assert(m);
    static char const* const diff = "-d";
    static char const* const diff_by_ver = "-dv";

    if (0 == strncmp(diff, m, strlen(diff)))
    {
        return METHOD_PACKAGESETS_DIFFERENCE;
    }
    else if (0 == strncmp(diff_by_ver, m, strlen(diff_by_ver)))
    {
        return METHOD_PACKAGESETS_DIFFERENCE_BY_VERSION;
    }

    return METHOD_PACKAGESETS_UNKNOWN;
}

char const*
parse_args_error_to_cstr(parse_args_error const errc)
{
    switch (errc)
    {
    case PARSE_ARGS_ERROR_OK:
        return "Ok";
    case PARSE_ARGS_ERROR_TOO_FEW_ARGUMENTS:
        return "Too few arguments";
    case PARSE_ARGS_ERROR_UNKNOWN_METHOD:
        assert(2 == METHOD_PACKAGESETS_ENUM_SIZE && "add new methods to hint");
        return "Unknow method. Available methods -d and -dv";

    case PARSE_ARGS_ERROR_UNKNOWN_BRANCH:
        assert(3 == TTFB_BRANCH_ENUM_SIZE && "add new branches to hint");
        return "branch not found. Available branches is \"sisyphus\", \"p10\" and \"p9\"";

    default:
        LOG("Unknow parse args error");
        assert(0);
    }
}

parse_args_error
parse_args(int const argc, char const* const* const argv, args* const ret)
{
    assert(argv);
    assert(ret);

    if (4 != argc)
    {
        return PARSE_ARGS_ERROR_TOO_FEW_ARGUMENTS;
    }

    ret->self_filename = argv[0];

    ret->fn = parse_method(argv[1]);
    if (METHOD_PACKAGESETS_UNKNOWN == ret->fn)
    {
        return PARSE_ARGS_ERROR_UNKNOWN_METHOD;
    }

    ret->first = ttfb_branch_from_cstr(argv[2]);
    if (TTFB_BRANCH_UNKNOWN == ret->first)
    {
        return PARSE_ARGS_ERROR_UNKNOWN_BRANCH;
    }
    ret->second = ttfb_branch_from_cstr(argv[3]);
    if (TTFB_BRANCH_UNKNOWN == ret->second)
    {
        return PARSE_ARGS_ERROR_UNKNOWN_BRANCH;
    }

    return PARSE_ARGS_ERROR_OK;
}
