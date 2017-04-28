#include <QtWidgets/QAction>
#include <QtCore/QDebug>

#include <ComponentManager/Components/ComponentRegistry>

#include "AdditionExport.hpp"

//------------------------------------------------------------------------------

class ADDITION_PUBLIC CustomAction : public QAction
{
  Q_OBJECT

public:
  Q_INVOKABLE
  CustomAction(QObject* parent)
    : QAction("Custom Action", parent)
  {
    qDebug() << "CustomAction created";
  }

  virtual
  ~CustomAction()
  {
    qDebug() << "CustomAction destroyed";
  }
};

//------------------------------------------------------------------------------

class ADDITION_PUBLIC CustomActionFromFactory : public QAction
{
  Q_OBJECT

public:
  Q_INVOKABLE
  CustomActionFromFactory(QObject* parent)
    : QAction("Custom Action From Factory", parent)
  {
    qDebug() << "CustomActionFromFactory created";
  }

  virtual
  ~CustomActionFromFactory()
  {
    qDebug() << "CustomActionFromFactory destroyed";
  }
};

//------------------------------------------------------------------------------

class ADDITION_PUBLIC CustomActionFactory : public QObject
{
  Q_OBJECT

public:
  Q_INVOKABLE
  CustomActionFactory()
  {
    qDebug() << "CustomActionFactory created";
  }

  virtual
  ~CustomActionFactory()
  {
    qDebug() << "CustomActionFactory destroyed";
  }

  Q_INVOKABLE 
  QObject*
  createCustomAction(QObject* parent)
  {
    qDebug() << "Function 'createCustomAction' is being invoked";
    return new CustomActionFromFactory(parent);
  }
};

#include "Addition.moc"

//------------------------------------------------------------------------------

extern "C"
{

// The only required exported function in the module
void
ADDITION_PUBLIC
registerComponent()
{
  qDebug() << "Addition Component registered";

  REGISTER_TYPE(CustomAction);
  REGISTER_TYPE(CustomActionFromFactory);
  REGISTER_TYPE(CustomActionFactory);
}
}
