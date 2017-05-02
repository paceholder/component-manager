#include "FunctionCall.hpp"

#include <QtCore/QJsonArray>

void
loadFunctionCalls(QJsonObject const & moduleJson)
{
  QJsonArray callJsonArray = moduleJson["calls"].toArray();
}
