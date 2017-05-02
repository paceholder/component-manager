#pragma once

#if                                                                            \
  defined (__CYGWIN__)                                                      || \
  defined (__CYGWIN32__)
#  define CM_PLATFORM "Cygwin"
#  define CM_PLATFORM_CYGWIN
#  define CM_PLATFORM_UNIX
#  define CM_PLATFORM_WINDOWS
#elif                                                                          \
  defined (_WIN16)                                                          || \
  defined (_WIN32)                                                          || \
  defined (_WIN64)                                                          || \
  defined (__WIN32__)                                                       || \
  defined (__TOS_WIN__)                                                     || \
  defined (__WINDOWS__)
#  define CM_PLATFORM "Windows"
#  define CM_PLATFORM_WINDOWS
#elif                                                                          \
  defined (macintosh)                                                       || \
  defined (Macintosh)                                                       || \
  defined (__TOS_MACOS__)                                                   || \
  (defined (__APPLE__) && defined (__MACH__))
#  define CM_PLATFORM "Mac"
#  define CM_PLATFORM_MAC
#  define CM_PLATFORM_UNIX
#elif                                                                          \
  defined (linux)                                                           || \
  defined (__linux)                                                         || \
  defined (__linux__)                                                       || \
  defined (__TOS_LINUX__)
#  define CM_PLATFORM "Linux"
#  define CM_PLATFORM_LINUX
#  define CM_PLATFORM_UNIX
#elif                                                                          \
  defined (__FreeBSD__)                                                     || \
  defined (__OpenBSD__)                                                     || \
  defined (__NetBSD__)                                                      || \
  defined (__bsdi__)                                                        || \
  defined (__DragonFly__)
#  define CM_PLATFORM "BSD"
#  define CM_PLATFORM_BSD
#  define CM_PLATFORM_UNIX
#elif                                                                          \
  defined (sun)                                                             || \
  defined (__sun)
#  define CM_PLATFORM "Solaris"
#  define CM_PLATFORM_SOLARIS
#  define CM_PLATFORM_UNIX
#elif                                                                          \
  defined (_AIX)                                                            || \
  defined (__TOS_AIX__)
#  define CM_PLATFORM "AIX"
#  define CM_PLATFORM_AIX
#  define CM_PLATFORM_UNIX
#elif                                                                          \
  defined (hpux)                                                            || \
  defined (_hpux)                                                           || \
  defined (__hpux)
#  define CM_PLATFORM "HPUX"
#  define CM_PLATFORM_HPUX
#  define CM_PLATFORM_UNIX
#elif \
  defined (__QNX__)
#  define CM_PLATFORM "QNX"
#  define CM_PLATFORM_QNX
#  define CM_PLATFORM_UNIX
#elif                                                                          \
  defined (unix)                                                            || \
  defined (__unix)                                                          || \
  defined (__unix__)
#  define CM_PLATFORM "Unix"
#  define CM_PLATFORM_UNIX
#endif

#ifndef CM_PLATFORM
#  error "Current platform is not supported."
#endif
