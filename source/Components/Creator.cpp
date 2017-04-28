#include "Creator.hpp"

#include <iterator>

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
             "Component Creator with given name does not exist");

  return it->second->create();
}


IComponentCreator const*
Creator::
has(QString const & name)
{
  auto it = Creator::instance().find(name);

  Q_ASSERT_X(it != Creator::instance().end(),
             "ComponentManager",
             "Component Creator with given name does not exist");

  return it->second.get();
}
}
