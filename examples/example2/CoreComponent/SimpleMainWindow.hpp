#pragma once

#include <QtWidgets/QMainWindow>

#include <QtCore/QMetaType>
#include <QtCore/QObject>

class QMdiArea;
class QToolBar;

class SimpleMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  Q_INVOKABLE
  SimpleMainWindow();

  virtual
  ~SimpleMainWindow();

  Q_INVOKABLE virtual void
  addToToolBar(QAction* action);

private:

  QMdiArea* _mdiArea;

  QToolBar* _toolBar;
};
