#include "FunctionSignature.hpp"

#include "Creator.hpp"

#include <QtCore/QDebug>

namespace  ComponentManager
{

FunctionSignatureWithArguments
parseJsonArrayToFunctionSignature(QJsonArray const& jsonArray)
{
  std::vector<std::pair<QString, QString> > result;

  for (int i = 0; i < jsonArray.size(); ++i)
  {
    QJsonObject param = jsonArray[i].toObject();

    QStringList keys = param.keys();

    Q_ASSERT_X(keys.size() == 1,
               "Function",
               "Function parameter has multiple or no values");

    QString const & key = keys.front();

    result.push_back(std::make_pair(key,
                                    param[key].toString()));
  }

  return result;
}


//------------------------------------------------------------------------------

FunctionSignatureWithArguments
parseJsonArrayToFunctionSignature(QJsonObject const& jsonObject,
                                  QString const & arrayName)
{
  auto paramsJson = jsonObject[arrayName];

  if (paramsJson.isArray())
  {
    return parseJsonArrayToFunctionSignature(paramsJson.toArray());
  }

  return FunctionSignatureWithArguments();
}


//------------------------------------------------------------------------------

FunctionSignatureTypes
parseJsonArrayToFunctionSignatureTypes(QJsonObject const& jsonObject,
                                       QString const & arrayName)
{
  std::vector<QString> result;

  auto paramsJson = jsonObject[arrayName];

  if (paramsJson.isArray())
  {
    QJsonArray jsonArray = paramsJson.toArray();

    for (int i = 0; i < jsonArray.size(); ++i)
    {
      QString param = jsonArray[i].toString();

      Q_ASSERT_X(param.size() > 0,
                 "Function",
                 "Empty type name");

      result.push_back(param);
    }
  }

  return FunctionSignatureTypes();
}


//------------------------------------------------------------------------------

std::pair<std::vector<QGenericArgument>,
          std::vector<QObject*> >
createArgumentsForFunctionSignature(FunctionSignatureWithArguments const &functionSignature)
{
  std::vector<QGenericArgument> result;
  std::vector<QObject*> qobjectStorage;

  for (auto const & typeValue : functionSignature)
  {
    if (typeValue.first == QStringLiteral("component"))
    {
      QString const & className = typeValue.second;

      QObject* parameter = ComponentManager::Creator::create(className);

      qobjectStorage.push_back(parameter);

      Q_ASSERT_X(parameter != nullptr,
                 "createArgumentsForFunctionSignature",
                 "Component parameter is nullptr");

      result.push_back(Q_ARG(QObject *, qobjectStorage.back()));
    }
    else if (typeValue.first == QStringLiteral("int"))
    {
      bool ok     = false;
      int  number = typeValue.second.toInt(&ok);

      // TODO numbers should also their memory addresses
      // up to the function call

      Q_ASSERT_X(ok,
                 "createArgumentsForFunctionSignature",
                 "Impossible to convert string to int");

      result.push_back(Q_ARG(int, number));
    }
    else if (true)
    {
      // TODO: put here another conditions for int, string, double etc...
    }
  }

  // It's important to keep all QObject* pointers in a temporary storage
  // qobjectStorage untill Q_ARG macro is executed.
  // The macro takes the address of a pointer, i.e. QObject**

  return std::make_pair(std::move(result),
                        std::move(qobjectStorage));
}
}
