/*!
  @file    Example.h

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

#pragma once

#include <QMainWindow>
#include <QtWidgets>

class QNodeViewEditor;
struct PropertyWidget;

class ExampleMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit ExampleMainWindow(QWidget* parent = NULL);

  PropertyWidget* _propertyWidget;

private:
  void createMenus();
  void loadFile();
  void saveFile();

  QNodeViewEditor* _editor;
  QMenu* _fileMenu;
  QGraphicsView* _view;
  QGraphicsScene* _scene;
};

extern ExampleMainWindow* g_mainWindow;
