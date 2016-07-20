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

int main(int argc, char* argv[])
{
  QApplication application(argc, argv);
  ExampleMainWindow window;
  window.resize(640, 480);
  window.show();
  return application.exec();
}

struct BlockListWidget : public QListWidget
{
  using QListWidget::QListWidget;
  virtual QMimeData* mimeData(const QList<QListWidgetItem*>& items) const
  {
    QListWidgetItem* item = items.front();
    QMimeData* data = new QMimeData();
    data->setText(item->text());
    return data;
  }

  virtual QMimeData* mimeData(const QList<QListWidgetItem*> items) const
  {
    QListWidgetItem* item = items.front();
    QMimeData* data = new QMimeData();
    data->setText(item->text());
    return data;
  }
};

ExampleMainWindow::ExampleMainWindow(QWidget* parent) : QMainWindow(parent)
{
  loadBlockDesc("config/block1.json");

  setWindowTitle(tr("QNodeView Example"));

  createMenus();

  _scene = new QGraphicsScene();
  _view = new QNodeViewCanvas(_scene, this);
  _view->setAcceptDrops(true);

  QSplitter* splitter = new QSplitter();
  QListWidget* listWidget = new BlockListWidget(this);
  listWidget->setDragEnabled(true);

  for (auto& kv : blockDefs)
  {
    new QListWidgetItem(tr(kv.first.c_str()), listWidget);
  }

  splitter->addWidget(listWidget);
  splitter->addWidget(_view);
  splitter->addWidget(new QPlainTextEdit());

  setCentralWidget(splitter);

  _editor = new QNodeViewEditor(this);
  _editor->install(_scene);

  addBlockInternal(QPointF(0, 0));
  addBlockInternal(QPointF(150, 0));
  addBlockInternal(QPointF(150, 150));
}

ExampleMainWindow::~ExampleMainWindow()
{
}

void ExampleMainWindow::addBlock()
{
  addBlockInternal(_view->sceneRect().center().toPoint());
}

void ExampleMainWindow::saveFile()
{
  QString fileName = QFileDialog::getSaveFileName();
  if (fileName.isEmpty())
    return;

  QFile file(fileName);
  file.open(QFile::WriteOnly);
  QDataStream stream(&file);
  _editor->save(stream);
}

void ExampleMainWindow::loadFile()
{
  QString fileName = QFileDialog::getOpenFileName();
  if (fileName.isEmpty())
    return;

  QFile file(fileName);
  file.open(QFile::ReadOnly);
  QDataStream stream(&file);
  _editor->load(stream);
}

void ExampleMainWindow::createMenus()
{
  QAction* quitAction = new QAction(tr("&Quit"), this);
  quitAction->setShortcuts(QKeySequence::Quit);
  quitAction->setStatusTip(tr("Quit the example"));
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  QAction* loadAction = new QAction(tr("&Load"), this);
  loadAction->setShortcuts(QKeySequence::Open);
  loadAction->setStatusTip(tr("Load node view from file"));
  connect(loadAction, SIGNAL(triggered()), this, SLOT(loadFile()));

  QAction* saveAction = new QAction(tr("&Save"), this);
  saveAction->setShortcuts(QKeySequence::Save);
  saveAction->setStatusTip(tr("Save node view to file"));
  connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

  QAction* addAction = new QAction(tr("&Add"), this);
  addAction->setStatusTip(tr("Add new block"));
  connect(addAction, SIGNAL(triggered()), this, SLOT(addBlock()));

  _fileMenu = menuBar()->addMenu(tr("&File"));
  _fileMenu->addAction(addAction);
  _fileMenu->addAction(loadAction);
  _fileMenu->addAction(saveAction);
  _fileMenu->addSeparator();
  _fileMenu->addAction(quitAction);
}

void ExampleMainWindow::addBlockInternal(const QPointF& position)
{
  QNodeViewBlock* block = new QNodeViewBlock(NULL);
  _scene->addItem(block);

  static qint32 index = 1;
  QString blockName = QString("myTest%1").arg(index++);

  block->addPort(blockName, 0, QNodeViewPortLabel_Name);
  block->addPort("TestEntity", 0, QNodeViewPortLabel_Type);

  block->addInputPort("Input 1");
  block->addInputPort("Input 2");
  block->addInputPort("Input 3");

  block->addOutputPort("Output 1");
  block->addOutputPort("Output 2");
  block->addOutputPort("Output 3");
  block->addOutputPort("Output 4");
  block->setPos(position);
}
