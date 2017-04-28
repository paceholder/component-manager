#pragma once

#include <ComponentManager/Components/Export>

#ifdef ADDITION_EXPORTS
  #define ADDITION_PUBLIC CM_EXPORT
#else
  #define ADDITION_PUBLIC CM_IMPORT
#endif
