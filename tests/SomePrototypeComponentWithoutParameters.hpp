#include <QtCore/QObject>

class SomePrototypeComponentWithoutParameters : public QObject
{
  Q_OBJECT
public:
  Q_INVOKABLE
  SomePrototypeComponentWithoutParameters(QObject* parent = nullptr)
    : QObject(parent)
  {}
};
