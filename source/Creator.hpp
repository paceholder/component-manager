#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QString>

#include "ComponentCreatorSet.hpp"

namespace ComponentManager
{

class Creator
{
public:

  static
  ComponentCreatorSet &
  instance();

  static
  void
  reset(ComponentCreatorSet && set);

  static
  void
  merge(ComponentCreatorSet && set);

  static
  QObject*
  create(QString const & name);

  static
  IComponentCreator const*
  has(QString const & name);
};

inline
QObject*
create(QString const & name)
{
  return Creator::create(name);
}


template<typename T>
T
create(QString const & name)
{
  return static_cast<T>(create(name));
}
}
