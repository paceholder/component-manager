#include "FactoryComponentCreator.hpp"

#include <QtCore/QJsonObject>
#include <QtCore/QDebug>

#include "Creator.hpp"
#include "FactoryComponentCreator.hpp"
#include "FunctionMixture.hpp"
#include "FunctionUtilities.hpp"
#include "InvokeMethodVectorBinding.hpp"

#include <functional>

namespace ComponentManager
{

FactoryComponentCreator::
FactoryComponentCreator(QJsonObject const &json)
{
  _name = json["name"].toString();

  _factoryName = json["factory"].toString();

  // creating function
  _functionName = json["function"].toString();

  _functionSignature = Function::parseJsonArrayToFunctionSignature(json,
                                                                   "parameters");
}


QString
FactoryComponentCreator::
name() const
{
  return _name;
}


QObject*
FactoryComponentCreator::
create() const
{
  QObject* result = nullptr;

  // Creator for factory (as a rule for singleton-factory)
  IComponentCreator const* factoryCreator =
    ComponentManager::Creator::has(_factoryName);

  QObject* factory = factoryCreator->create();

  FunctionMixture const* functionMixture =
    dynamic_cast<FunctionMixture const*>(factoryCreator);

  Q_ASSERT_X(functionMixture != nullptr,
             "FactoryComponentCreator",
             "Factory has no functions");

  auto it = functionMixture->functions().find(_functionName);

  Q_ASSERT_X(it != functionMixture->functions().end(),
             "FactoryComponentCreator",
             qUtf8Printable(QString("Function '%1' was not defined in factory").arg(_functionName)));

  auto const declaredFunctionSignature = it->second;

  Q_ASSERT_X(declaredFunctionSignature.size() == _functionSignature.size(),
             "FactoryComponentCreator",
             qUtf8Printable(QString("Declared and required function '%1' signatures differ in size").arg(_functionName)));

  for (std::size_t i = 0; i < declaredFunctionSignature.size(); ++i)
  {
    Q_ASSERT_X(declaredFunctionSignature[i] == _functionSignature[i].first,
               "FactoryComponentCreator",
               "Different argutment types");
  }

  //--------------------------

  auto const genericArguments = Function::createArgumentsFromSignature(_functionSignature);

  auto candidateMethods = Function::findFunctions(factory->metaObject(),
                                                  _functionName,
                                                  _functionSignature.size());

  // method to call
  QMetaMethod method = Function::findFunction(candidateMethods,
                                              _functionSignature,
                                              genericArguments.second);

  // a wrapper for the return variable
  QGenericReturnArgument returnArgument(method.typeName(),
                                        static_cast<void*>(&result));


  // lambda wrapper around QMetaObject::invokeMethod
  // it takes just nine QGenericArgument parameters
  auto f =
    [&](QGenericArgument g1, QGenericArgument g2, QGenericArgument g3,
        QGenericArgument g4, QGenericArgument g5, QGenericArgument g6,
        QGenericArgument g7, QGenericArgument g8, QGenericArgument g9)
    {
      method.invoke(factory,
                    returnArgument,
                    g1, g2, g3,
                    g4, g5, g6,
                    g7, g8, g9);
    };

  // a copy of the argument vector
  std::vector<QGenericArgument> generics = genericArguments.first;

  while (generics.size() < Arguments::numberOfQtArguments)
    generics.push_back(QGenericArgument());

  Arguments::bindVector(f, generics);

  return result;
}
}
