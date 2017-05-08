#pragma once

#include "IComponentCreator.hpp"

#include <unordered_map>

#include "QStringStdHash.hpp"
#include "FunctionMixture.hpp"

namespace ComponentManager
{

class SingletonComponentCreator
  : public IComponentCreator
  , public FunctionMixture
{
public:

  SingletonComponentCreator(QJsonObject const &doc);

public:

  QString
  name() const override;

  QObject*
  create() const override;

private:

  QString _name;

  QString _class;

  mutable QObject* _instance = nullptr;
};
}
