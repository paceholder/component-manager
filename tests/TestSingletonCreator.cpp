#include <catch/catch.hpp>

#include <ComponentManager/Components/ComponentRegistry>
#include <ComponentManager/Components/ComponentCreatorSet>
#include <ComponentManager/Components/IComponentCreator>
#include <ComponentManager/Components/Creator>

class TestSingleton : public QObject
{
  Q_OBJECT
public:

  Q_INVOKABLE
  TestSingleton(QObject* parent = nullptr)
    : QObject(parent)
  {}
};

#include "TestSingletonCreator.moc"

REGISTER_TYPE_CPP(TestSingleton)

/// Test parses a JSON component array and instantiates a SingletonComponentCreator.
/// Then the test checks if two instances of a QObject have the same address.
TEST_CASE("Create singleton Component", "[singleton]")
{

  //--------------------------------------

  QMetaObject const* registeredObject =
    ComponentManager::ComponentRegistry::find("TestSingleton");

  REQUIRE(registeredObject != nullptr);

  //--------------------------------------

  QString json =
    R"(
  {
    "components" : [
      {
        "name" : "TestSingleton",
        "construction" : "singleton",

        "functions" : [
          "blahBlah"
        ]
      }
    ]
  }
  )";

  using ComponentManager::Creator;

  Creator::merge(ComponentManager::createComponentCreatorSet(json));

  ComponentManager::IComponentCreator const* testSingletonCreator =
    Creator::has("TestSingleton");

  REQUIRE(testSingletonCreator != nullptr);

  //--------------------------------------

  auto createdObject1 = testSingletonCreator->create();

  REQUIRE(createdObject1 != nullptr);

  auto createdObject2 = testSingletonCreator->create();

  REQUIRE(createdObject1 == createdObject2);
}
