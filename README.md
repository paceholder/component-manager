Linux (x64, gcc-4.8, clang-3.6): [![Build Status](https://travis-ci.org/paceholder/component-manager.svg?branch=master)](https://travis-ci.org/paceholder/component-manager)

Windows (Win32, x64, msvc2015, MinGW 5.3): [![Build status](https://ci.appveyor.com/api/projects/status/ol3wwt948rjslwpo/branch/master?svg=true)](https://ci.appveyor.com/project/paceholder/component-manager/branch/master)

# Qt Component Manager

The library helps to build modular applications based on the Qt framework
and particularly [Qt Meta-Object Sytem](http://doc.qt.io/qt-5/metaobjects.html).


## Qt Component Manager in a nutshell

The Component Manager helps to instantiate `QObject` derivatives by their identifier
without explicitly linking to the library where this class is defined.
It is also possible to cast the resulting pointer `QObject*` to some pre-defined interface
(abstract class).

The instantiation is as easy as:

```cpp
auto someComponent = ComponentManager::create<IComponent>("FancyComponent");
```

## Example

### 1. Module (Shared Library). Define your objects and register them


```cpp

// CustomModule.cpp

class CPP_EXPORT_MACRO CustomAction : public QAction
{
  Q_OBJECT // <- important

public:
  Q_INVOKABLE // <- important
  CustomAction(QObject* parent)
    : QAction("Custom Action", parent) {}

  virtual
  ~CustomAction() = default;
};


#include <ComponentManager/ComponentRegistry>

// The only required public function

extern "C"
{
  void
  CPP_EXPORT_MACRO
  registerComponent()
  {
    REGISTER_TYPE(CustomAction);
  }
}
```

### 2. Prepare JSON-based module description

`CustomModule.json`:

```json
{
  "module" : {
    "name" : "CustomModule",

    "path" : "../lib",

    "components" : [
      {
        "name" : "CustomAction",

        "construction" : "prototype",
      }
    ]
  }
}
```



### 3. Write main application with using modules

```cpp
#include <ComponentManager/ModuleLoader>
#include <ComponentManager/Creator>

int
main(int argc, char* argv[])
{
  // Create Qt application
  QApplication application(argc, argv);

  // We assume that CustomModule.json is located next to the executable

  QDir directory(QCoreApplication::applicationDirPath());

  QString jsonFile = directory.absoluteFilePath("CustomModule.json");

  // This line loads shared libraries and registers available components

  ComponentManager::loadModule(jsonFile);

  // Create components at any place in your program

  CustomAction* customAction =
    ComponentManager::create<CustomAction*>("CustomAction");

  // Use customAction

  return application.exec();
}
```
