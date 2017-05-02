#pragma once

#include <vector>

#include <QtCore/QStringList>

#include "Export.hpp"

namespace ComponentManager
{

CM_PUBLIC
void
loadModules(std::vector<QString> const &moduleJsonFiles);

CM_PUBLIC
void
loadModule(QString const &moduleJsonFile);

}
