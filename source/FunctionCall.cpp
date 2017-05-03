#include "FunctionCall.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QDebug>

#include "InvokeMethodVectorBinding.hpp"
#include "Creator.hpp"

namespace ComponentManager
{

std::vector<FunctionCall>
loadFunctionCalls(QJsonObject const & moduleJson)
{
  std::vector<FunctionCall> result;

  QJsonArray callJsonArray = moduleJson["calls"].toArray();

  for (int i = 0; i < callJsonArray.size(); ++i)
  {
    QJsonObject functionJson =
      callJsonArray[i].toObject();

    //---

    QString const component =
      functionJson["component"].toString();

    QString const function =
      functionJson["function"].toString();

    Function::SignatureWithArguments signatureWithArguments =
      Function::parseJsonArrayToFunctionSignature(functionJson, "parameters");

    result.emplace_back(component,
                        function,
                        std::move(signatureWithArguments));
  }

  return result;
}


void
invokeFunctionCalls(std::vector<FunctionCall> const &functionCalls)
{
  for (auto const & functionCall : functionCalls)
  {
    QString const & componentName = std::get<0>(functionCall);

    QString const & functionName = std::get<1>(functionCall);

    Function::SignatureWithArguments const & arguments =
      std::get<2>(functionCall);

    //--------------

    auto component = ComponentManager::create(componentName);

    auto f =
      [&](QGenericArgument g1, QGenericArgument g2, QGenericArgument g3,
          QGenericArgument g4, QGenericArgument g5, QGenericArgument g6,
          QGenericArgument g7, QGenericArgument g8, QGenericArgument g9)
      {
        QMetaObject::invokeMethod(component,
                                  functionName.toUtf8().constData(),
                                  g1, g2, g3,
                                  g4, g5, g6,
                                  g7, g8, g9);
      };

    auto const aa = Function::createArgumentsFromSignature(arguments);

    // a copy of the argument vector
    std::vector<QGenericArgument> genericArguments = aa.first;

    // pad vector to get full set of arguments
    while (genericArguments.size() < Arguments::numberOfQtArguments)
      genericArguments.push_back(QGenericArgument());

    Arguments::bindVector(f, genericArguments);
  }
}
}
