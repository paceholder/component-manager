#include <QtCore/QObject>

#include <QDebug>

class SomePrototypeComponentWithParent : public QObject
{
  Q_OBJECT

public:

  Q_INVOKABLE
  SomePrototypeComponentWithParent(QObject* parent)
    : QObject(parent)
  { }
};
