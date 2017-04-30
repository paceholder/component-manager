# Qt Component Manager

The library helps to build modular applications based on the Qt framework
and particularly [Qt Meta-Object Sytem](http://doc.qt.io/qt-5/metaobjects.html).

## Qt Component Manager in a nutshell

The Component Manager helps to instantiate `QObject` derivatives by their identifier
without explicitly linking to the library where this class is defined.
It is also possible to cast the resulting pointer `QObject*` to some pre-defined interface
(abstract class).

The instantiation is as easy as:

@code{.cpp}
  auto someComponent = ComponentManager::create<IComponent>("FancyComponent");
@endcode

## Example

### 1. Module (Shared Library). Define your objects and register them


@code{.cpp}

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


  #include <ComponentManager/Components/ComponentRegistry>

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
@endcode

### 2. Prepare JSON-based module description

`CustomModule.json`:

@code{.json}
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
@endcode



### 3. Write main application with using modules

@code{.cpp}
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
@endcode
