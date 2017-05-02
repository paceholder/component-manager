#pragma once

#if                                                               \
  defined (__MINGW32__)                                        || \
  defined (__MINGW64__)
#  define CM_COMPILER "MinGW"
#  define CM_COMPILER_MINGW
#elif \
  defined (__GNUC__)
#  define CM_COMPILER "GNU"
#  define CM_COMPILER_GNU
#  define CM_COMPILER_GNU_VERSION_MAJOR __GNUC__
#  define CM_COMPILER_GNU_VERSION_MINOR __GNUC_MINOR__
#  define CM_COMPILER_GNU_VERSION_PATCH __GNUC_PATCHLEVEL__
#elif \
  defined (__clang__)
#  define CM_COMPILER "Clang"
#  define CM_COMPILER_CLANG
#elif \
  defined (_MSC_VER)
#  define CM_COMPILER "Microsoft Visual C++"
#  define CM_COMPILER_MICROSOFT
#elif \
  defined (__BORLANDC__)
#  define CM_COMPILER "Borland C++ Builder"
#  define CM_COMPILER_BORLAND
#elif \
  defined (__CODEGEARC__)
#  define CM_COMPILER "CodeGear C++ Builder"
#  define CM_COMPILER_CODEGEAR
#elif                                                             \
  defined (__INTEL_COMPILER)                                   || \
  defined (__ICL)
#  define CM_COMPILER "Intel C++"
#  define CM_COMPILER_INTEL
#elif                                                             \
  defined (__xlC__)                                            || \
  defined (__IBMCPP__)
#  define CM_COMPILER "IBM XL C++"
#  define CM_COMPILER_IBM
#elif \
  defined (__HP_aCC)
#  define CM_COMPILER "HP aC++"
#  define CM_COMPILER_HP
#elif \
  defined (__WATCOMC__)
#  define CM_COMPILER "Watcom C++"
#  define CM_COMPILER_WATCOM
#endif

#ifndef CM_COMPILER
#  error "Current compiler is not supported."
#endif
