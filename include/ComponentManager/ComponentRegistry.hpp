#pragma once

#include <QtCore/QMetaObject>
#include <QtCore/QMetaType>
#include <QtCore/QString>
#include <QtCore/QHash>

#include "Export.hpp"

namespace ComponentManager {

/// Interface for accessing a QMetaObject
class RegistryItem
{
public:
  virtual
  ~RegistryItem() {}

  virtual
  QMetaObject const*
  getQMetaObject() const = 0;

  /// Returns class name
  virtual
  QString
  className() const = 0;
};

/// Class stores concrete type of registered QObject derivative
template <class T>
class ConcreteRegistryItem : public RegistryItem
{
public:
  virtual
  ~ConcreteRegistryItem() {}

  virtual
  QMetaObject const*
  getQMetaObject() const
  {
    return &T::staticMetaObject;
  }

  virtual
  QString
  className() const
  {
    return T::staticMetaObject.className();
  }
};

/// Singleton class storing Qt meta-object instances
/// associated with registered classes.
class CM_PUBLIC ComponentRegistry
{
public:

  template <class T>
  static
  void
  insert(QString const& name)
  {
    auto registryItem = new ConcreteRegistryItem<T>();

    insert(name, registryItem);
  }

  static
  void
  insert(QString const& name, RegistryItem const* registryItem);

  static
  QMetaObject const*
  find(QString const& name);

private:

  static QHash<QString, RegistryItem const*>&
  registry();
};

/// Object registers a MetaType statically upon loading the dll.

/**
   Loading the dll (shared library) instantiates all
   static objects defined in the compilatino unit.
   That automatically registers a Type into the
   ComponentRegistry.
 */
template <typename Type>
class MetaTypeRegistration
{
public:
  inline
  MetaTypeRegistration(char const* name)
  {
    ComponentRegistry::insert<Type>(name);
  }
};
} // namespace

#define REGISTER_TYPE(Type) \
  { ComponentManager::ComponentRegistry::insert<Type>(#Type); }

#define REGISTER_TYPE_CPP(Type) \
  static ComponentManager::MetaTypeRegistration<Type> type##Type(#Type);
