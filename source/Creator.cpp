#include "Creator.hpp"

#include <iterator>

#include <QtCore/QDebug>

namespace ComponentManager
{

ComponentCreatorSet &
Creator::
instance()
{
  static ComponentCreatorSet componentCreatorSet;

  return componentCreatorSet;
}


void
Creator::
reset(ComponentCreatorSet && set)
{
  Creator::instance() = std::move(set);
}


void
Creator::
merge(ComponentCreatorSet && set)
{
  auto & s = Creator::instance();

  s.insert(std::make_move_iterator(std::begin(set)),
           std::make_move_iterator(std::end(set)));
}


QObject*
Creator::
create(QString const &name)
{
  auto it = Creator::instance().find(name);

  Q_ASSERT_X(it != Creator::instance().end(),
             "ComponentManager",
             QString("Component Creator name '%1' does not exist").arg(name).toUtf8().constData());

  return it->second->create();
}


IComponentCreator const*
Creator::
has(QString const & name)
{
  auto it = Creator::instance().find(name);

  Q_ASSERT_X(it != Creator::instance().end(),
             "ComponentManager",
             QString("Component Creator name '%1' does not exist").arg(name).toUtf8().constData());

  return it->second.get();
}
}
