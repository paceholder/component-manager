#include "SimpleMainWindow.hpp"

#include <QtCore/QDebug>

#include <ComponentManager/ComponentRegistry>

#include "CoreExport.hpp"

#include "SimpleMainWindow.hpp"

extern "C"
{

CORE_PUBLIC void
registerComponent()
{
  qDebug() << "Core Component registered";

  REGISTER_TYPE(SimpleMainWindow);
}
}

// alternatively
//REGISTER_TYPE_CPP(SimpleMainWindow);
