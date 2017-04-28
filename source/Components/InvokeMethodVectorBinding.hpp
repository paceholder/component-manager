#pragma once

/**
   @file InvokeMethodVectorBinding.hpp

   File contains methods for binding a vector of QGenericArgument
   to QMetaObject::invokeMethod function

   @code{.cpp}

   // In some QObject-derived class
   Q_INVOCABLE
   void functionNameToInvoke(float d);

   ...

   auto f =
    [](QGenericArgument g1, QGenericArgument g2, QGenericArgument g3,
        QGenericArgument g4, QGenericArgument g5, QGenericArgument g6,
        QGenericArgument g7, QGenericArgument g8, QGenericArgument g9)
    {
      QMetaObject::invokeMethod(pointerToObjectWithFunction
                                "functionNameToInvoke",
                                 g1, g2, g3,
                                 g4, g5, g6,
                                 g7, g8, g9);
    };

    std::vector<QGenericArgument> vv;
    vv.push_back(Q_ARG(4.0));

    bindVector(f, vv);

   @endcode
 */



namespace ComponentManager
{

/// This is a maximum number of arguments taken by QMetaObject::invokeMethod
// According to the Qt 5.x Documentation
static constexpr std::size_t numberOfQtArguments = 9;

template <typename F, typename V, typename ... Args>
auto
bindVector(F f, const V&, Args&& ... args)
->typename std::enable_if<sizeof ... (Args) == numberOfQtArguments, void>::type
{
  f(std::forward<Args>(args) ...);
}


template <typename F, typename V, typename ... Args>
auto
bindVector(F f, const V& v, Args&& ... args)
->typename std::enable_if < (sizeof ... (Args) < numberOfQtArguments), void > ::type
{
  bindVector(f, v, std::forward<Args>(args) ..., v.at(sizeof ... (Args)));
}
}
