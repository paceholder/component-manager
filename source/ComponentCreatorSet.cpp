#include "ComponentCreatorSet.hpp"

#include "IComponentCreator.hpp"

#include <memory>

#include <iostream>

#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>

#include <QtCore/QDebug>

#include "PrototypeComponentCreator.hpp"
#include "SingletonComponentCreator.hpp"
#include "FactoryComponentCreator.hpp"

namespace ComponentManager
{

ComponentCreatorSet
createComponentCreatorSet(QJsonObject jsonObject)
{
  ComponentCreatorSet componentCreatorSet;

  if (jsonObject.isEmpty())
  {
    throw std::logic_error("Empty JSON object");
  }

  auto it = jsonObject.find("components");

  auto getString = [] (QJsonObject const & obj, QString str)
                   {
                     QJsonValue val = obj.value(str);

                     if (val.type() == QJsonValue::Undefined ||
                         val.type() == QJsonValue::Null)
                     {
                       qCritical() << "No value with the name " << str;
                     }

                     return val.toString();
                   };

  if (it != jsonObject.end() &&
      it->isArray())
  {
    QJsonArray array = it->toArray();

    for (QJsonValueRef const &ref : array)
    {
      QJsonObject componentObject = ref.toObject();

      QString name   = getString(componentObject, "name");
      QString constr = getString(componentObject, "construction");

      if (name.isEmpty())
        throw std::logic_error("Empty name property");

      if (constr.isEmpty())
        throw std::logic_error("Empty construction property");

      if (constr == "prototype")
      {
        auto creator = std::make_unique<PrototypeComponentCreator>(componentObject);

        componentCreatorSet[name] = std::move(creator);
      }
      else if (constr == "factory")
      {
        auto creator = std::make_unique<FactoryComponentCreator>(componentObject);

        componentCreatorSet[name] = std::move(creator);
      }
      else if (constr == "singleton")
      {
        auto creator = std::make_unique<SingletonComponentCreator>(componentObject);

        componentCreatorSet[name] = std::move(creator);
      }
    }
  }
  else
  {
    throw std::logic_error("No component array in JSON");
  }

  return componentCreatorSet;
}


ComponentCreatorSet
createComponentCreatorSet(QString jsonObjectUtf8)
{
  QJsonObject jsonObject =
    QJsonDocument::fromJson(jsonObjectUtf8.toUtf8()).object();

  return createComponentCreatorSet(jsonObject);
}

}
