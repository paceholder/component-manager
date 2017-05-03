#pragma once

#include <tuple>

#include <QtCore/QJsonObject>

#include "FunctionSignature.hpp"

namespace ComponentManager
{

using FunctionCall = std::tuple<QString,
                                QString,
                                Function::SignatureWithArguments>;

/**
   Function fetches the array of function call descriptors
   from the given QJsonObject using the keyword 'calls'.

   @code{.json}
   "calls" : [
     {
       "component" : "SimpleMainWindow",

       "function" : "addToToolBar",

       "parameters" : [
         { "component" : "CustomAction"}
       ]
     },
     {
       ... another call ...
     }
   ]
   @endcode

   A function call descriptor is a tuple of three types:
   - `QString` for QObject class name
   - `QString` for function name
   - `Function::SignatureWithArguments` list of arguments
 */
std::vector<FunctionCall>
loadFunctionCalls(QJsonObject const & moduleJson);

/**
   Function creates each QObject using the `ComponentManager::Creator`
   class and invokes a function with the given vector of
   type-argument pairs
 */
void
invokeFunctionCalls(std::vector<FunctionCall> const &functionCalls);
}
