#pragma once

#include <vector>

#include <QtCore/QString>
#include <QtCore/QGenericArgument>
#include <QtCore/QStringList>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

namespace ComponentManager
{

using FunctionSignatureWithArguments =
        std::vector<std::pair<QString, QString> >;

using FunctionSignatureTypes = std::vector<QString>;

/**
   Function expects an array of type

   @code{.json}
   "parameters" : [
    { "component" : "MainWindow" }
   ]
   @endcode

   The result is a vector of pairs <type, value>
 */
FunctionSignatureWithArguments
parseJsonArrayToFunctionSignature(QJsonArray const& jsonArray);

/**
   Overloaded version taking array name as parameter
 */
FunctionSignatureWithArguments
parseJsonArrayToFunctionSignature(QJsonObject const& jsonObject,
                                  QString const & arrayName);

/**
   Overloaded version taking array name as parameter
 */
FunctionSignatureTypes
parseJsonArrayToFunctionSignatureTypes(QJsonObject const& jsonObject,
                                       QString const & arrayName);

/**
   Function creates QObjects required for passing as parameters
   to the other component constructors and wraps them
   with the macro Q_ARG() producing a vector of QGenericArgutment
 */
std::pair<std::vector<QGenericArgument>,
          std::vector<QObject*> >
createArgumentsForFunctionSignature(FunctionSignatureWithArguments const &functionSignature);
}
