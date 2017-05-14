#pragma once

#include <QtCore/QMetaMethod>
#include <QtCore/QString>
#include <QtCore/QDebug>

#include "FunctionSignature.hpp"

namespace ComponentManager
{

namespace Function
{

inline
std::vector<QMetaMethod>
findFunctions(QMetaObject const *metaObject,
              QString const &functionName,
              unsigned int parameterCount)
{
  std::vector<QMetaMethod> result;

  for (int i = 0; i < metaObject->methodCount(); ++i)
  {
    QMetaMethod method = metaObject->method(i);

    unsigned int declaredParameterCount =
      static_cast<unsigned int>(method.parameterCount());

    if (functionName == method.name() &&
        parameterCount == declaredParameterCount)
    {
      result.push_back(std::move(method));
    }
  }

  return result;
}

#if 0
inline
std::vector<QMetaMethod>
findConstructors(QMetaObject const * metaObject,
                 unsigned int parameterCount)
{
  std::vector<QMetaMethod> result;

  QString constructorName = metaObject->className();

  for (int i = 0; i < metaObject->constructorCount(); ++i)
  {
    QMetaMethod constructor = metaObject->constructor(i);

    unsigned int declaredParameterCount =
      static_cast<unsigned int>(constructor.parameterCount());

    if (parameterCount == declaredParameterCount)
    {
      result.push_back(std::move(constructor));
    }
  }

  return result;
}
#endif


inline
QMetaMethod
findFunction(std::vector<QMetaMethod> candidateMethods,
             Function::SignatureWithArguments const &arguments,
             std::vector<QObject*> const & qObjectArguments)
{
  bool functionIsOkToUse = true;

  for (QMetaMethod const &m : candidateMethods)
  {
    functionIsOkToUse = true;

    unsigned int c = 0;

    // check each parameter
    // given object must inherit the expected class
    for (int i = 0; i < m.parameterCount(); ++i)
    {
      if (arguments[i].first == QStringLiteral("component"))
      {
        int parameterType = m.parameterType(i);

        QMetaObject const * expectedMetaObject =
          QMetaType::metaObjectForType(parameterType);

        QMetaObject const * givenMetaObject =
          qObjectArguments[c]->metaObject();;

        if (givenMetaObject->inherits(expectedMetaObject))
        {
          // it is OK
        }
        else
        {
          functionIsOkToUse = false;
          break;
        }

        ++c;
      }

      if (!functionIsOkToUse)
        break;
    }

    if (functionIsOkToUse)
    {
      // target function has been found -- return immediately
      return m;
    }
  }

  return QMetaMethod();
}


//
}
}
