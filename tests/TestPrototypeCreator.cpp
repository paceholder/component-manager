#include <catch/catch.hpp>

#include <ComponentManager/ComponentRegistry>
#include <ComponentManager/ComponentCreatorSet>
#include <ComponentManager/IComponentCreator>
#include <ComponentManager/Creator>

#include <QDebug>

#include "SomePrototypeComponentWithParent.hpp"
#include "SomePrototypeComponentWithoutParameters.hpp"

REGISTER_TYPE_CPP(SomePrototypeComponentWithoutParameters)
REGISTER_TYPE_CPP(SomePrototypeComponentWithParent)

/// Here we test instantiation of components defined as "prototypes"
/// Some of them could have parent objects (constructor parameters),
/// some of them are created without parents


/// Test parses a JSON component array and checks if the corresponding
/// instances of Creators were intantiated in ComponentCreatorSet
TEST_CASE("Create prototype without parameters", "[prototype]")
{
  QString json =
    R"(
  {
    "components" : [
      {
        "name" : "SomePrototypeComponentWithoutParameters",
        "construction" : "prototype"
      }
    ]
  }
  )";

  // 1. Is parsing correct?

  auto parsedComponents = ComponentManager::createComponentCreatorSet(json);

  using ComponentManager::Creator;

  Creator::reset(std::move(parsedComponents));

  REQUIRE(Creator::has("SomePrototypeComponentWithoutParameters") != nullptr);

  // 2. Is QObject class registered?

  QMetaObject const* registeredObject =
    ComponentManager::ComponentRegistry::find("SomePrototypeComponentWithoutParameters");

  REQUIRE(registeredObject != nullptr);

  // 3. Is the new object being created correctly?

  ComponentManager::IComponentCreator const* creator =
    Creator::has("SomePrototypeComponentWithoutParameters");

  QObject* qObject = creator->create();

  REQUIRE(qObject != nullptr);

  REQUIRE(qObject->metaObject()->className() ==
          QStringLiteral("SomePrototypeComponentWithoutParameters"));
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/// We create one Component without parameters in constructor and then
/// create the second component using the latter one as a parent parameter.
TEST_CASE("Create prototype with parent object", "[prototype]")
{
  QString json =
    R"(
  {
    "components" : [
      {
        "name" : "SomePrototypeComponentWithParent",
        "construction" : "prototype",

        "constructor_parameters" : [
            {"component" : "SomePrototypeComponentWithoutParameters" }
        ]
      },

      {
        "name" : "SomePrototypeComponentWithoutParameters",
        "construction" : "prototype"
      }
    ]
  }
  )";

  // 1
  using ComponentManager::Creator;

  Creator::merge(ComponentManager::createComponentCreatorSet(json));

  REQUIRE(Creator::has("SomePrototypeComponentWithoutParameters") != nullptr);
  REQUIRE(Creator::has("SomePrototypeComponentWithParent") != nullptr);

  // 2. Check meta-object registration

  QMetaObject const* registeredObject =
    ComponentManager::ComponentRegistry::find("SomePrototypeComponentWithoutParameters");

  REQUIRE(registeredObject != nullptr);

  QMetaObject const* registeredObject2 =
    ComponentManager::ComponentRegistry::find("SomePrototypeComponentWithParent");

  REQUIRE(registeredObject2 != nullptr);

  // 3

  QObject* qObject = ComponentManager::create("SomePrototypeComponentWithParent");

  // Note: the parent object is passed to the SomePrototypeComponentWithParent's constructor
  // implicitly based on the JSON description. Applying the chain rule we create
  // a parameter-object (parent) by callign the same Creator::create function.

  REQUIRE(qObject != nullptr);

  REQUIRE(qObject->metaObject()->className() ==
          QStringLiteral("SomePrototypeComponentWithParent"));

  REQUIRE(qObject->parent() != nullptr);

  REQUIRE(qObject->parent()->metaObject()->className() ==
          QStringLiteral("SomePrototypeComponentWithoutParameters"));
}
