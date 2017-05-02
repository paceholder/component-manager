#pragma once

#include <QtCore/QJsonObject>

#include "FunctionSignature.hpp"

namespace ComponentManager
{

using FunctionCall = std::pair<QString,
                               FunctionSignatureWithArguments>;

void
loadFunctionCalls(QJsonObject const & moduleJson);

}
