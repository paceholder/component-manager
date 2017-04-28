#include "SimpleMainWindow.hpp"

#include <QtWidgets/QMdiArea>
#include <QtWidgets/QToolBar>

SimpleMainWindow::
SimpleMainWindow()
{
  _mdiArea = new QMdiArea(this);
  _toolBar = addToolBar("SimpleMainWindow ToolBar");

  setCentralWidget(_mdiArea);

  setDockOptions(AnimatedDocks |
                 AllowNestedDocks |
                 AllowTabbedDocks);
}


SimpleMainWindow::
~SimpleMainWindow()
{
//
}


void
SimpleMainWindow::
addToToolBar(QAction* action)
{
  _toolBar->addAction(action);
}
