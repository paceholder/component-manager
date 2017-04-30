#pragma once

#include "IComponentCreator.hpp"

#include "FunctionSignature.hpp"

namespace ComponentManager
{


/// Class produces a Creator responsible for Components instantiated by a Factory.
/**
  An example JSON description for a factory-instantiated Component:

  @code{.json}
  {
    "components" : [
      {
        "name" : "ComponentFromFactory",

        "construction" : "factory",

        "factory" : "SingletonObjectFactory",

        "function" : "createObject"

        "parameters" : [
          { "component" : "SomeParentObject" },
          { "int" : "7" }
        ]
      }
    ]
  }
  @endcode

  This creator knows what factory to use and what class-function to invoke.
*/
class FactoryComponentCreator : public IComponentCreator
{
public:

  FactoryComponentCreator(QJsonObject const &json);

public:

  QString
  name() const override;

  QObject*
  create() const override;

private:

  QString _name;

  QString _factoryName;

  QString _functionName;

  FunctionSignatureWithArguments _functionSignature;
};
}
