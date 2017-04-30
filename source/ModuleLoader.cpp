#include "ModuleLoader.hpp"

#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QLibrary>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>

#include <QtCore/QDebug>

#include <QtWidgets/QApplication>

#include "Creator.hpp"
#include "ComponentCreatorSet.hpp"

namespace ComponentManager {

void
loadModules(std::vector<QString> const &moduleJsonFiles)
{
  for (auto const & s : moduleJsonFiles)
  {
    loadModule(s);
  }
}


inline
QString
constructSharedLibraryPath(QString const &moduleName,
                           QString const &modulePath)
{
  QDir appPath(QCoreApplication::applicationDirPath());

  appPath.cd(appPath.absoluteFilePath(modulePath));

  return appPath.filePath(moduleName);
}


void
loadModule(QString const &moduleJsonFile)
{
  // 1. Read json file

  if (!QFileInfo::exists(moduleJsonFile))
  {
    qDebug() << "File "
             << moduleJsonFile
             << " does not exist.";

    return;
  }

  QFile file(moduleJsonFile);

  if (!file.open(QIODevice::ReadOnly))
    return;

  QByteArray wholeFileData = file.readAll();

  QJsonParseError parseError;

  QJsonObject const jsonDocument =
    QJsonDocument::fromJson(wholeFileData, &parseError).object();

  Q_ASSERT_X(parseError.error == QJsonParseError::NoError,
             "ComponentManager::loadModule",
             parseError.errorString().toUtf8().constData());

  // 2. Read json structure

  // 2.1 Module name

  QJsonObject moduleJson = jsonDocument["module"].toObject();

  Q_ASSERT_X(!jsonDocument.isEmpty(),
             "ComponentManager::loadModule",
             "Empty module json is loaded.");

  QString moduleName = moduleJson["name"].toString();

  Q_ASSERT_X(!moduleName.isEmpty(),
             "ComponentManager::loadModule",
             "Module name is empty");

  // 2.1 Module's shared library path (relative to executable)

  QString modulePath = moduleJson["path"].toString();

  Q_ASSERT_X(!modulePath.isEmpty(),
             "ComponentManager::loadModule",
             "Module path is empty");

  // 3. Path to the shared library

  QString sharedLibPath = constructSharedLibraryPath(moduleName, modulePath);

  Q_ASSERT_X(QFileInfo::exists(sharedLibPath),
             "ComponentManager::loadModule",
             "Shared library does not exist.");

  // 4. Load shared library

  QLibrary module(sharedLibPath);

  QFunctionPointer componentRegistratorFunction = module.resolve("registerComponent");

  QString cantResolve = QString("Cannot resolve function 'registerComponent' in library:\n %1").arg(sharedLibPath);

  Q_ASSERT_X(componentRegistratorFunction != nullptr,
             "ModuleLoader",
             sharedLibPath.toUtf8().constData());

  // Register!
  componentRegistratorFunction();

  // 5. Parse declared Components and add them to the global creator

  Creator::merge(ComponentManager::createComponentCreatorSet(moduleJson));
}
}
