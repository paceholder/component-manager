#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QString>

//#include "IFunction.hpp"

namespace ComponentManager
{

/// The object is created based on parsed JSON. It is responsible for making QObjects.
/**
  There might be different implementations for this interface, depending on
  specific configuration in JSON, for example: factory method creator,
  singleton creator, prototype (use class constructor) etc.
  Some returned QObjects could have described in JSON callable functions.
  Such functions a wrapped in IFunction interface.
 */
class IComponentCreator
{
public: 

  virtual QString
  name() const = 0;

public:

  /// The main job of this class is to create QObjects.
  virtual
  QObject*
  create() const = 0;

  //virtual
  //IFunction*
  //function(QString const &name) = 0;
};
}
