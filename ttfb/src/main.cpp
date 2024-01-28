#include <ttfb/ttfb.h>

#include <json-c/json_object.h>
#include <json_util.h>

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char** argv)
{
    json_object* ret = ttfb_get_binary_packages(TTFB_BRANCH_p10);
    int errc = json_object_to_fd(2, ret, 0);
    if (0 != errc)
    {
        printf("%s\n", json_util_get_last_err());
    }
    printf("is freed %d", json_object_put(ret));

    return errc;
}