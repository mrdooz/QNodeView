/*!
  @file    QNodeViewEditor.h

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
#include "precompiled.hpp"

class QPointF;
class QGraphicsScene;
class QGraphicsItem;
class QNodeViewBlock;
class QNodeViewConnection;
class QNodeViewPort;

class QNodeViewEditor : public QObject
{
  Q_OBJECT

public:
  explicit QNodeViewEditor(QGraphicsScene* scene, QObject* parent = NULL);

  void loadScene();
  void saveScene();

private:

  bool eventFilter(QObject* object, QEvent* event);
  bool validEndPort(QGraphicsItem* item);

  QGraphicsItem* itemAt(const QPointF& point);

  QGraphicsScene* _scene;
  QNodeViewConnection* _activeConnection;

  QNodeViewPort* _lastPort = nullptr;
};
