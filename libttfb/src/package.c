#include <libttfb/package.h>
#include <libttfb/version.h>

#include <json-c/json.h>
#include <json_types.h>

#include <assert.h>

package
package_new(void)
{
    return (package){
        .name = NULL,
        .epoch = 0,
        .ver = version_new(),
        .release = NULL,
        .arch = NULL,
        .disttag = NULL,
        .buildtime = 0,
        .source = NULL,
    };
}

int
package_init_from_json(package* const pkg, json_object const* const obj)
{
    assert(obj);
    assert(pkg);
    assert(json_object_is_type(obj, json_type_object));

    json_object* name = NULL;
    if (!json_object_object_get_ex(obj, "name", &name) ||
        !json_object_is_type(name, json_type_string))
    {
        return -1;
    }

    pkg->name = json_object_get_string(name);

    json_object* epoch = NULL;
    if (!json_object_object_get_ex(obj, "epoch", &epoch) ||
        !json_object_is_type(epoch, json_type_int))
    {
        return -1;
    }

    pkg->epoch = json_object_get_int(epoch);

    json_object* ver_obj = NULL;
    if (!json_object_object_get_ex(obj, "version", &ver_obj) ||
        !json_object_is_type(ver_obj, json_type_string))
    {
        return -1;
    }

    if (0 != version_new_from_cstr(&pkg->ver, json_object_get_string(ver_obj)))
        pkg->ver = version_new();

    json_object* release = NULL;
    if (!json_object_object_get_ex(obj, "release", &release) ||
        !json_object_is_type(release, json_type_string))
    {
        return -1;
    }

    pkg->release = json_object_get_string(release);

    json_object* arch = NULL;
    if (!json_object_object_get_ex(obj, "arch", &arch) ||
        !json_object_is_type(arch, json_type_string))
    {
        return -1;
    }

    pkg->arch = json_object_get_string(arch);

    json_object* disttag = NULL;
    if (!json_object_object_get_ex(obj, "disttag", &disttag) ||
        !json_object_is_type(disttag, json_type_string))
    {
        return -1;
    }

    pkg->disttag = json_object_get_string(disttag);

    json_object* buildtime = NULL;
    if (!json_object_object_get_ex(obj, "buildtime", &buildtime) ||
        !json_object_is_type(buildtime, json_type_int))
    {
        return -1;
    }

    pkg->buildtime = json_object_get_int(buildtime);

    json_object* source = NULL;
    if (!json_object_object_get_ex(obj, "source", &source) ||
        !json_object_is_type(source, json_type_string))
    {
        return -1;
    }
    pkg->source = json_object_get_string(source);

    return 0;
}
