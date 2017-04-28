# Qt Component Manager

## Program Modularity

As a rule, complex projects are not compiled into a single executable.
Modularity is the key element for extensible and configurable software.

The target of the component manager is to abstract components of the system from
their real location (executable or shared library). Component manager allows to
avoid up-front linking of the executable against shared libraries.
Required plugins are loaded on demand.

In the code, we no longer create the instances of the classes implemented in
plugins, instead we request the component manager to create an instance of the
class registered with the given name and return a pointer to the class
interface.

@code{.cpp}
  auto someComponent = ComponentManager::create<IComponent>("FancyComponent");
@endcode

## Component Description

The executable must know what components are available in the system. In the
current implementation we scan some predefined folder and collect files
describing the components (*.json).

### Json Component Descriptors

Below you can see an example of the JSON-based component descriptor. It declares
the location of the plugin (the extension is added automatically depending on
OS), and an array of components. The only component in this listing has the name
"Button". The `construction` group describes the way an instance of such a
component is constructed. In this case we use the c++ class `Button` and pass
an instance of the component `MainWindow` to the constructor.

@code{.json}
{
  "context" : {
    "dll" : "../Extension",

    "components" : [
      {
        "name" : "Button",
        "construction" : "prototype",
        "constructor_parameters" : [
          {"component" : "MainWindow" }
        ]
      }
    ]
  }
}
@endcode


## Start-up time system construction

The modules make system flexible and extensible. But how to make the GUI
flexible as well? What if we need to swap two widgets or omit certain Widget in
the specific build? Would we be forced to adjust the source code for each
configuration?

The answer is -- not necessarily.

The Qt meta object system fills the gap in the scarce RTTI and introspection
features of C++. Using the meta-information collected from QObjects it is
possible to call function of the classes having their string-based names.



