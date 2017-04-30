#pragma once

#include <vector>

#include <QtCore/QStringList>

namespace ComponentManager
{

void
loadModules(std::vector<QString> const &moduleJsonFiles);

void
loadModule(QString const &moduleJsonFile);

}
