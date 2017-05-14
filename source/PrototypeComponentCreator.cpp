#include "PrototypeComponentCreator.hpp"

#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <QtCore/QMetaMethod>

#include "ComponentRegistry.hpp"
#include "Creator.hpp"
#include "InvokeMethodVectorBinding.hpp"

#include "FunctionUtilities.hpp"

#include <QDebug>
#include <iostream>

namespace ComponentManager
{

PrototypeComponentCreator::
PrototypeComponentCreator(QJsonObject const &json)
{
  _name = json["name"].toString();

  QString cl = json["class"].toString();

  _class = cl.isEmpty() ? _name : cl;

  _constructor =
    Function::parseJsonArrayToFunctionSignature(json, "constructor_parameters");
}


QString
PrototypeComponentCreator::
name() const
{
  return _name;
}


QObject*
PrototypeComponentCreator::
create() const
{
  QObject* result = nullptr;

  // call with parameters

  using ComponentManager::ComponentRegistry;
  QMetaObject const* metaObject = ComponentRegistry::find(_class);

  // find available constructors

  auto candidateConstructors =
    ComponentManager::Function::findConstructors(metaObject, 1);

  // the Qt's macro Q_ARG("QObject*", b) expects the parameter `b` to be
  // a pointer `QObject *`. Then, internally the pointer to a pointer is
  // taken: `QObject**`. Thus, the original pointer `QObject*` must be
  // stored in the scope until the function is invoked.
  auto const aa = Function::createArgumentsFromSignature(_constructor);

  // a copy of the argument vector
  std::vector<QGenericArgument> genericArguments = aa.first;

  // lambda wrapper around QMetaObject::invokeMethod
  // it takes just nine QGenericArgument parameters
  auto f =
    [&](QGenericArgument g1, QGenericArgument g2, QGenericArgument g3,
        QGenericArgument g4, QGenericArgument g5, QGenericArgument g6,
        QGenericArgument g7, QGenericArgument g8, QGenericArgument g9)
    {
      result = metaObject->newInstance(g1, g2, g3,
                                       g4, g5, g6,
                                       g7, g8, g9);
    };

  // pad vector to get full set of arguments
  while (genericArguments.size() < Arguments::numberOfQtArguments)
    genericArguments.push_back(QGenericArgument());

  Arguments::bindVector(f, genericArguments);

  Q_ASSERT_X(result != nullptr,
             "PrototypeComponentCreator",
             "Created Component is nullptr");

  return result;
}
}
