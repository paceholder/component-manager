#include "SingletonComponentCreator.hpp"

#include <QtCore/QJsonObject>

#include "ComponentRegistry.hpp"

namespace ComponentManager
{

SingletonComponentCreator::
SingletonComponentCreator(QJsonObject const &doc)
  : FunctionMixture(doc)
{
  _name = doc["name"].toString();

  QString const cl = doc["class"].toString();

  _class = cl.isEmpty() ? _name : cl;
}


QString
SingletonComponentCreator::
name() const
{
  return _name;
}


QObject*
SingletonComponentCreator::
create() const
{
  QObject *result = nullptr;

  if (_instance == nullptr)
  {
    using ComponentManager::ComponentRegistry;
    QMetaObject const* metaObject = ComponentRegistry::find(_class);

    result = metaObject->newInstance();

    _instance = result;
  }
  else
  {
    result = _instance;
  }

  return result;
}
}
