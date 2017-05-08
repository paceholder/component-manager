#pragma once

#include <QtCore/QString>

#include <vector>

#include "Export.hpp"

namespace ComponentManager
{

CM_PUBLIC
void
loadModules(std::vector<QString> const &moduleJsonFiles);

CM_PUBLIC
void
loadModules(QString const &moduleJsonFile);

}
