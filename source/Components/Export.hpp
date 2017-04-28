#pragma once

#include "Compiler.hpp"
#include "OperatingSystem.hpp"

#ifdef CM_PLATFORM_WINDOWS
#  define CM_EXPORT __declspec(dllexport)
#  define CM_IMPORT __declspec(dllimport)
#  define CM_LOCAL
#elif                                                           \
  CM_COMPILER_GNU_VERSION_MAJOR >= 4                         || \
  defined (CM_COMPILER_CLANG)
#  define CM_EXPORT __attribute__((visibility("default")))
#  define CM_IMPORT __attribute__((visibility("default")))
#  define CM_LOCAL  __attribute__((visibility("hidden")))
#else
#  define CM_EXPORT
#  define CM_IMPORT
#  define CM_LOCAL
#endif

#ifdef __cplusplus
#  define CM_DEMANGLED extern "C"
#else
#  define CM_DEMANGLED
#endif


#if defined (CM_SHARED) && !defined (CM_STATIC)
#  ifdef CM_EXPORTS
#    define CM_PUBLIC CM_EXPORT
#  else
#    define CM_PUBLIC CM_IMPORT
#  endif
#  define CM_PRIVATE CM_LOCAL
#elif !defined (CM_SHARED) && defined (CM_STATIC)
#  define CM_PUBLIC
#  define CM_PRIVATE
#elif defined (CM_SHARED) && defined (CM_STATIC)
#  ifdef CM_EXPORTS
#    error "Cannot build as shared and static simultaneously."
#  else
#    error "Cannot link against shared and static simultaneously."
#  endif
#else
#  ifdef CM_EXPORTS
#    error "Choose whether to build as shared or static."
#  else
#    error "Choose whether to link against shared or static."
#  endif
#endif
