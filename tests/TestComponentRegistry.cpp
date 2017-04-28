#include <catch/catch.hpp>

#include <QtCore/QObject>

#include <ComponentManager/Components/ComponentRegistry>

class TestObject : public QObject
{
  Q_OBJECT
public:
  Q_INVOKABLE
  TestObject(QObject* parent)
    : QObject(parent)
  {}
};

class TestObjectTwo : public QObject
{
  Q_OBJECT
public:
  Q_INVOKABLE
  TestObjectTwo(QObject* parent)
    : QObject(parent)
  {}
};

#include "TestComponentRegistry.moc"

REGISTER_TYPE_CPP(TestObject)

/// There are two ways of registering user-defined QObjects:
/// 1) Calling a static function ComponentRegistry::insert<Type>;
/// 2) Creaing a static object ComponentManager::MetaTypeRegistration<Type>
///
/// Two tests below are testing two ways of registering Qt's meta-objects.


//------------------------------------------------------------

TEST_CASE("Register QObject with static object", "[registry]")
{
  QMetaObject const* registeredObject =
    ComponentManager::ComponentRegistry::find("TestObject");

  REQUIRE (registeredObject != nullptr);
}

//------------------------------------------------------------

TEST_CASE("Register QObject with function call", "[registry]")
{
  REGISTER_TYPE(TestObjectTwo);

  QMetaObject const* registeredObject =
    ComponentManager::ComponentRegistry::find("TestObjectTwo");

  REQUIRE (registeredObject != nullptr);
}
