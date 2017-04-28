#pragma once

#include <ComponentManager/Components/Export>

#ifdef CORE_EXPORTS
  #define CORE_PUBLIC CM_EXPORT
#else
  #define CORE_PUBLIC CM_IMPORT
#endif
