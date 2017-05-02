#pragma once

#include <unordered_map>
#include <memory>

#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QHash>

#include "QStringStdHash.hpp"

#include "IComponentCreator.hpp"
#include "Export.hpp"

namespace ComponentManager
{

/// Stores Component Creators parsed in JSON.

/**
   JSON-based component descriptor contains an array of component
   declarations. Some of components could be created directly
   by calling the constructor, another are created by a factory component:

   @code{.json}
    "components" : [
      {
        "name" : "Button",
        "construction" : "prototype",

        "constructor_parameters" : [
            {"component" : "MainWindow" }
        ]
      }
    ]
   @endcode

   ComponentCreatorSet collects all the types of componenent creators
   and stores them as pointers to abstract class IComponentCreator.
 */

using ComponentCreatorSet =
        std::unordered_map<QString, std::unique_ptr<IComponentCreator> >;

CM_PUBLIC
ComponentCreatorSet
createComponentCreatorSet(QJsonObject jsonObject);

CM_PUBLIC
ComponentCreatorSet
createComponentCreatorSet(QString jsonObjectUtf8);
}
