#ifndef LIBTTFB__EXPORT_MACRO_H
#define LIBTTFB__EXPORT_MACRO_H

#ifndef LIBTTFB_EXPORT
#if defined(_MSC_VER)
#define LIBTTFB_EXPORT __declspec(dllexport)
#else
#define LIBTTFB_EXPORT extern
#endif
#endif

#endif // LIBTTFB__EXPORT_MACRO_H
