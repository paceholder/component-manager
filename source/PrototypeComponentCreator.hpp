#pragma once

#include "IComponentCreator.hpp"

#include "FunctionSignature.hpp"

#include <vector>

class QJsonObject;

namespace ComponentManager
{

/// Manages the prototype-based Component creation.

/**
   A typical JSON description of a prototype Component looks as follows:

   @code{.json}
   {
    "name" : "Button",
    "construction" : "prototype",

    "constructor_parameters" : [
        { "component" : "MainWindow" },
        { "int", 4 },
        { "double", 4.4 },
        { "QString", "Hello" }
    ]
   }
   @endcode

   The array "constructor_parameters" is optional and can be empty
   or do not exist at all.
 */
class PrototypeComponentCreator : public IComponentCreator
{
public:

  PrototypeComponentCreator(QJsonObject const &json);

public:

  QString
  name() const override;

  QObject*
  create() const override;

private:

  std::vector<std::pair<QString, QString> >
  parseFunctionParameters(QJsonArray const& json);

private:

  QString _name;

  QString _class;

  Function::SignatureWithArguments _constructor;
};
}
