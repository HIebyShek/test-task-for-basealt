#include <json_types.h>
#include <stdlib.h>
#include <ttfb/cli.h>
#include <ttfb/method.h>

#include "libttfb/ttfb.h"
#include "macro.h"

int
main(int const argc, char const* const* const argv)
{
    args input;
    int errc = parse_args(argc, argv, &input);
    if (PARSE_ARGS_ERROR_OK != errc)
    {
        LOG("%s", parse_args_error_to_cstr(errc));
        return errc;
    }

    switch (input.fn)
    {
    case METHOD_PACKAGESETS_DIFFERENCE:
        return packagesets_difference(input.first, input.second);
    case METHOD_PACKAGESETS_DIFFERENCE_BY_VERSION:
        return packagesets_difference_by_version(input.first, input.second);

    case METHOD_PACKAGESETS_UNKNOWN:
        LOG("Something went wrong");
        return EXIT_FAILURE;
    }

    return errc;
}
