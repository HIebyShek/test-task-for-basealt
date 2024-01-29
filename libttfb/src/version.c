#include <libttfb/version.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

version
version_new(void)
{
    return (version){ 0 };
}

int
version_new_from_cstr(version* const ver, char const* const s)
{
    assert(s);
    assert(ver);

    *ver = version_new();

    char* endptr = NULL;
    ver->major = strtoul(s, &endptr, 10);
    if (*endptr == '\0')
    {
        return 0;
    }
    if (*endptr != '.')
    {
        return -1;
    }
    ver->minor = strtoul(endptr + 1, &endptr, 10);
    if (*endptr == '\0')
    {
        return 0;
    }
    if (*endptr != '.')
    {
        return -1;
    }
    ver->patch = strtoul(endptr + 1, &endptr, 10);
    if (*endptr != '\0')
    {
        return -1;
    }

    return 0;
}

int
version_cmp(version const lhs, version const rhs)
{
    if (lhs.major < rhs.major)
    {
        return -1;
    }
    else if (lhs.major > rhs.major)
    {
        return 1;
    }

    if (lhs.minor < rhs.minor)
    {
        return -1;
    }
    else if (lhs.minor > rhs.minor)
    {
        return 1;
    }

    if (lhs.patch < rhs.patch)
    {
        return -1;
    }
    else if (lhs.patch > rhs.patch)
    {
        return 1;
    }

    return 0;
}
