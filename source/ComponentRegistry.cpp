#include "ComponentRegistry.hpp"

#include <QMetaType>

using namespace ComponentManager;

void
ComponentRegistry::
insert(QString const& typeName, RegistryItem const* metaObject)
{
  ComponentRegistry::registry().insert(typeName, metaObject);
}


QMetaObject const*
ComponentRegistry::
find(QString const& typeName)
{
  auto const &registry = ComponentRegistry::registry();

  auto it = registry.find(typeName);

  if (it == registry.end())
  {
    throw std::logic_error("No registered type with the name '" +
                           typeName.toStdString() + "'");
  }

  RegistryItem const* registryItem = *it;

  QMetaObject const* metaObject = registryItem->getQMetaObject();

  // no meta object means it Qt-known class?

  if (!metaObject)
  {
    int id = QMetaType::type(typeName.toUtf8().constData());

    if (id == QMetaType::UnknownType)
    {
      throw std::logic_error("Unknown Type");
    }

    metaObject = QMetaType::metaObjectForType(id);
  }

  return metaObject;
}


QHash<QString, RegistryItem const*>&
ComponentRegistry::
registry()
{
  static QHash<QString, RegistryItem const*> registry;

  return registry;
}
