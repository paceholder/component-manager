#include <catch/catch.hpp>

#include <ComponentManager/ComponentRegistry>
#include <ComponentManager/Creator>
#include <ComponentManager/ComponentCreatorSet>
#include <ComponentManager/IComponentCreator>

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/// Test parses a JSON component array and checks if the corresponding
/// instances of Creators were instantiated in ComponentCreatorSet
TEST_CASE("Parse component JSON array", "[components]")
{
  QString json =
    R"(
  {
    "components" : [
      {
        "name" : "Button",

        "construction" : "prototype",

        "constructor_parameters" : [
            {"component" : "MainWindow" }
        ]
      },

      {
        "name" : "ButtonFromFactory",

        "construction" : "factory",

        "factory" : "ButtonFactory",

        "parameters" : [
          { "component" : "MainWindow" }
        ]
      },

      {
        "name" : "ButtonFactory",

        "construction" : "singleton",

        "functions" : [
          "createButton"
        ]
      },

      {
        "name" : "SimpleMainWindow",
        "construction" : "prototype",

        "functions" : [
          "addToToolBar"
        ]
      }
    ]
  }
  )";

  auto parsedComponents = ComponentManager::createComponentCreatorSet(json);

  using ComponentManager::Creator;

  Creator::reset(std::move(parsedComponents));

  SECTION("Testing prototype creator")
  {
    REQUIRE(Creator::has("Button") != nullptr);
  }

  SECTION("Testing factory-based Component creator")
  {
    REQUIRE(Creator::has("ButtonFromFactory") != nullptr);
  }

  SECTION("Tesing singleton creator")
  {
    REQUIRE(Creator::has("ButtonFactory") != nullptr);
  }
}
