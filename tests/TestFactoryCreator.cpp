#include <catch/catch.hpp>

#include <ComponentManager/ComponentRegistry>
#include <ComponentManager/ComponentCreatorSet>
#include <ComponentManager/IComponentCreator>
#include <ComponentManager/Creator>

#include <QtCore/QDebug>

#include <iostream>

class DummyParameterObject : public QObject
{
  Q_OBJECT
public:

  Q_INVOKABLE
  DummyParameterObject()
  {}
};

class ObjectCreatedByFactory : public QObject
{
  Q_OBJECT
public:

  Q_INVOKABLE
  ObjectCreatedByFactory(QObject * parent)
    : QObject(parent)
  {}
};

/// This is an object that will work as a factory
/// producing another objects with special declared functions
/// (in this case `createObject()`).
class SingletonObjectFactory : public QObject
{
  Q_OBJECT
public:

  Q_INVOKABLE
  SingletonObjectFactory(QObject* parent = nullptr)
    : QObject(parent)
  {}

  Q_INVOKABLE
  QObject*
  createObject(QObject *parent, int a)
  {
    qDebug() << "Parameter " << a;
    return new ObjectCreatedByFactory(parent);
  }
};

REGISTER_TYPE_CPP(DummyParameterObject)
REGISTER_TYPE_CPP(ObjectCreatedByFactory)
REGISTER_TYPE_CPP(SingletonObjectFactory)

#include "TestFactoryCreator.moc"

/// Test parses a JSON component array and instantiates a SingletonComponentCreator.
/// Then the test checks if two instances of a QObject have the same address.
TEST_CASE("Component created by factory", "[components]")
{
  QString json = R"(
  {
    "components" : [
      {
        "name" : "DummyParameterObject",

        "construction" : "singleton"
      },

      {
        "name" : "ComponentFromFactory",

        "construction" : "factory",

        "factory" : "SingletonObjectFactory",

        "function" : "createObject",

        "parameters" : [

          { "component" : "DummyParameterObject" },

          { "int" : "5" }

        ]
      },

      {
        "name" : "SingletonObjectFactory",

        "construction" : "singleton",

        "functions" : [
          {
            "name" : "createObject",

            "signature" : [ "component", "int" ]
          },
          {
            "here we can have" : "something else"
          }
        ]
      }
    ]
  }
  )";

  // 1. Test registered creators

  auto parsedComponents = ComponentManager::createComponentCreatorSet(json);

  using ComponentManager::Creator;

  Creator::reset(std::move(parsedComponents));

  ComponentManager::IComponentCreator const* dummyParentSingleton =
    Creator::has("DummyParameterObject");

  REQUIRE(dummyParentSingleton != nullptr);

  ComponentManager::IComponentCreator const* testSingletonCreator =
    Creator::has("SingletonObjectFactory");

  REQUIRE(testSingletonCreator != nullptr);

  ComponentManager::IComponentCreator const* componentFromFactoryCreator =
    Creator::has("ComponentFromFactory");

  REQUIRE(componentFromFactoryCreator != nullptr);

  // 2. Test singleton

  auto createdSingleton1 = testSingletonCreator->create();

  REQUIRE(createdSingleton1 != nullptr);

  auto createdSingleton2 = testSingletonCreator->create();

  REQUIRE(createdSingleton1 == createdSingleton2);

  // 3. Test "parent" singleton

  auto parentSingleton1 = dummyParentSingleton->create();

  REQUIRE(parentSingleton1 != nullptr);

  auto parentSingleton2 = dummyParentSingleton->create();

  REQUIRE(parentSingleton1 == parentSingleton2);

  // 4. Reset global singleton object with newly parsed JSON
  // ComponentManager's state is used by internal creation of
  // dependent components

  //ComponentManager::Creator::reset(std::move(parsedComponents));

  // 5. Test "object from factory"

  auto componentFromFactory = componentFromFactoryCreator->create();

  REQUIRE(componentFromFactory != nullptr);

  // 6. Another way of creating component from factory

  auto c = ComponentManager::Creator::create("ComponentFromFactory");

  REQUIRE(c != nullptr);

  REQUIRE(c->parent() != nullptr);

  REQUIRE(c->parent()->metaObject()->className() ==
          QStringLiteral("DummyParameterObject"));
}
