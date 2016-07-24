/*!
  @file    Example.cpp

  Copyright (c) 2014 Graham Wihlidal

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  @author  Graham Wihlidal
  @date    January 19, 2014
*/

#include "Example.h"
#include "QNodeViewBlock.h"
#include "QNodeViewCanvas.h"
#include "QNodeViewEditor.h"
#include "block_loader.hpp"
#include "block_widgets.hpp"

ExampleMainWindow* g_mainWindow;

int main(int argc, char* argv[])
{
  QApplication application(argc, argv);
  application.setStyle(QStyleFactory::create("Fusion"));
  g_mainWindow = new ExampleMainWindow();
  g_mainWindow->resize(640, 480);
  g_mainWindow->show();
  int res = application.exec();
  delete g_mainWindow;
  return res;
}

ExampleMainWindow::ExampleMainWindow(QWidget* parent) : QMainWindow(parent)
{
  loadBlockDesc("config/block1.json");

  setWindowTitle(tr("QNodeView Example"));

  _scene = new QGraphicsScene();
  _view = new QNodeViewCanvas(_scene, this);
  _view->setAcceptDrops(true);
  _view->setMinimumSize(400, 400);

  QSplitter* splitter = new QSplitter();
  QListWidget* listWidget = new BlockListWidget(this);
  listWidget->setDragEnabled(true);

  for (auto& kv : blockDefs)
  {
    new QListWidgetItem(tr(kv.first.c_str()), listWidget);
  }

  splitter->addWidget(listWidget);
  splitter->addWidget(_view);
  _propertyWidget = new PropertyWidget();
  splitter->addWidget(_propertyWidget);

  setCentralWidget(splitter);

  _editor = new QNodeViewEditor(_scene, this);
}
