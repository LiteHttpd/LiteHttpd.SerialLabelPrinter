#pragma once

#if defined (__clang__)
#define HTTPDDEV_CLANG 1

#elif defined (__GNUC__)
#define HTTPDDEV_GCC 1

#elif defined (_MSC_VER)
#define HTTPDDEV_MSVC 1

#else
#error Unknown cpp compiler
#endif

#if HTTPDDEV_MSVC
#define HTTPDDEV_EXPORT __declspec(dllexport)
#define HTTPDDEV_IMPORT __declspec(dllimport)
#define HTTPDDEV_CALL _cdecl
#endif

#if HTTPDDEV_CLANG
#define HTTPDDEV_EXPORT __attribute__((visibility("default")))
#define HTTPDDEV_IMPORT __attribute__((visibility("default")))
#define HTTPDDEV_CALL
#endif

#if HTTPDDEV_GCC
#define HTTPDDEV_EXPORT __attribute__((visibility("default")))
#define HTTPDDEV_IMPORT __attribute__((visibility("default")))
#define HTTPDDEV_CALL
#endif

#if !(HTTPDDEV_MSVC || HTTPDDEV_CLANG || HTTPDDEV_GCC)
#define HTTPDDEV_EXPORT 
#define HTTPDDEV_IMPORT 
#define HTTPDDEV_CALL
#endif
