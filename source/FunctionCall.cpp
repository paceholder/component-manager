#include "FunctionCall.hpp"

#include <QtCore/QJsonArray>
#include <QtCore/QDebug>
#include <QtCore/QMetaMethod>

#include "InvokeMethodVectorBinding.hpp"
#include "Creator.hpp"
#include "FunctionSignature.hpp"

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


inline
std::vector<QMetaMethod>
findFunctions(QObject const *obj,
              QString const &functionName,
              unsigned int parameterCount)
{
  std::vector<QMetaMethod> result;

  QMetaObject const * metaObject = obj->metaObject();

  for (int i = 0; i < metaObject->methodCount(); ++i)
  {
    QMetaMethod method = metaObject->method(i);

    unsigned int declaredParameterCount =
      static_cast<unsigned int>(method.parameterCount());

    if (functionName == method.name() &&
        parameterCount == declaredParameterCount)
    {
      result.push_back(std::move(method));
    }
  }

  return result;
}


inline
QMetaMethod
findFunction(QObject const *obj,
             QString const & functionName,
             Function::SignatureWithArguments const &arguments,
             Function::GenericArumentsWithObjects const &generics
             )
{
  std::vector<QMetaMethod> candidateMethods =
    findFunctions(obj, functionName, arguments.size());

  bool functionIsOkToUse = true;

  for (QMetaMethod const &m : candidateMethods)
  {
    functionIsOkToUse = true;

    unsigned int c = 0;

    // check each parameter
    // give object must inherit the expected class
    for (int i = 0; i < m.parameterCount(); ++i)
    {
      if (arguments[i].first == QStringLiteral("component"))
      {
        int parameterType = m.parameterType(i);

        QMetaObject const * expectedMetaObject =
          QMetaType::metaObjectForType(parameterType);

        QMetaObject const * givenMetaObject =
          generics.second[c]->metaObject();;

        if (givenMetaObject->inherits(expectedMetaObject))
        {
          // it is OK
        }
        else
        {
          functionIsOkToUse = false;
          break;
        }

        ++c;
      }

      if (!functionIsOkToUse)
        break;
    }

    if (functionIsOkToUse)
    {
      // target function has been found -- return immediately
      return m;
    }
  }

  return QMetaMethod();
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

    auto const generics = Function::createArgumentsFromSignature(arguments);

    // method to call
    QMetaMethod method = findFunction(component,
                                      functionName,
                                      arguments,
                                      generics);

    // a copy of the argument vector
    std::vector<QGenericArgument> genericArguments = generics.first;

    auto f =
      [&](QGenericArgument g1, QGenericArgument g2, QGenericArgument g3,
          QGenericArgument g4, QGenericArgument g5, QGenericArgument g6,
          QGenericArgument g7, QGenericArgument g8, QGenericArgument g9)
      {
        method.invoke(component,
                      g1, g2, g3,
                      g4, g5, g6,
                      g7, g8, g9);
      };

    // pad vector to get full set of arguments
    while (genericArguments.size() < Arguments::numberOfQtArguments)
      genericArguments.push_back(QGenericArgument());

    Arguments::bindVector(f, genericArguments);
  }
}
}
