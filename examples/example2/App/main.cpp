#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QAction>

#include <ComponentManager/ModuleLoader>
#include <ComponentManager/Creator>

#include <SimpleMainWindow.hpp>

int
main(int argc, char* argv[])
{
  // Create Qt application
  QApplication application(argc, argv);

  qDebug() << "Starting application";

  // Set filter to json files
  QStringList jsonFilter("*.json");

  // Set directory for looking for json files
  // to 'resource' directory in the root application directory
  QDir directory(QString("%1/../resource/").arg(QCoreApplication::applicationDirPath()));

  // Apply json filter
  QStringList jsonFiles = directory.entryList(jsonFilter);

  std::vector<QString> jsonFilesVector;
  for(auto const & jf : jsonFiles)
  {
    jsonFilesVector.push_back(directory.absoluteFilePath(jf));
  }


  ComponentManager::loadModules(jsonFilesVector);


  auto & componentCreatorSet = ComponentManager::Creator::instance();


  qDebug() << "Registered creators:";
  for (auto const & p : componentCreatorSet)
  {
    qDebug() << " - " << p.first;
  }

  auto mainWindow = ComponentManager::create<SimpleMainWindow*>("SimpleMainWindow");

  auto a = ComponentManager::create<QAction*>("CustomAction");

  a->setText("CustomAction (Created Manually)");

  mainWindow->addToToolBar(a);

  mainWindow->show();

  // Start Qt application
  return application.exec();
}
