#include <catch/catch.hpp>

#include <ComponentManager/ComponentRegistry>
#include <ComponentManager/ComponentCreatorSet>
#include <ComponentManager/IComponentCreator>
#include <ComponentManager/Creator>

#include "FunctionCall.hpp"

#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>

#include <iostream>

class DummyObject : public QObject
{
  Q_OBJECT
public:

  Q_INVOKABLE
  DummyObject()
  {}
};

class ObjectWithFunction : public QObject
{
  Q_OBJECT
public:

  Q_INVOKABLE
  ObjectWithFunction() {}

  Q_INVOKABLE
  void
  functionWithParameter(DummyObject *obj) { ++number; }

  int
  getNumber() const { return number; }

private:

  int number = 41;
};

REGISTER_TYPE_CPP(DummyObject)
REGISTER_TYPE_CPP(ObjectWithFunction)

#include "TestFunctionCall.moc"

/// Test parses a JSON component array and instantiates a SingletonComponentCreator.
/// Then the test checks if two instances of a QObject have the same address.
TEST_CASE("Call function of an object", "[function]")
{
  QString json = R"(
  {
    "components" : [
      {
        "name" : "DummyObject",

        "construction" : "prototype"
      },

      {
        "name" : "ObjectWithFunction",

        "construction" : "singleton",

        "functions" : [
          {
            "name" : "functionWithParameter",

            "parameters" : [ "component" ]
          }
        ]
      }
    ],

    "calls" : [
      {
        "component" : "ObjectWithFunction",

        "function" : "functionWithParameter",

        "parameters" : [
          { "component" : "DummyObject" }
        ]
      }
    ]
  }
  )";

  // 1. Test registered creators

  QJsonObject jsonObject =
    QJsonDocument::fromJson(json.toUtf8()).object();

  using ComponentManager::Creator;

  auto ccs = ComponentManager::createComponentCreatorSet(jsonObject);

  REQUIRE(ccs.size() == 2);

  // 2.

  Creator::merge(std::move(ccs));

  // 3.

  auto fc = ComponentManager::loadFunctionCalls(jsonObject);

  REQUIRE(fc.size() == 1);

  ComponentManager::invokeFunctionCalls(fc);

  auto obj = ComponentManager::create<ObjectWithFunction*>("ObjectWithFunction");

  REQUIRE(42 == obj->getNumber());
}
