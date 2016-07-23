/*!
  @file    QNodeViewEditor.cpp

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

#include "QNodeViewEditor.h"
#include "QNodeViewCommon.h"
#include "QNodeViewConnection.h"
#include "QNodeViewPort.h"
#include "QNodeViewBlock.h"

//------------------------------------------------------------------------------
QNodeViewEditor::QNodeViewEditor(QObject* parent) : QObject(parent), _activeConnection(NULL)
{
}

//------------------------------------------------------------------------------
void QNodeViewEditor::install(QGraphicsScene* scene)
{
  Q_ASSERT(scene);
  scene->installEventFilter(this);
  _scene = scene;
}

//------------------------------------------------------------------------------
bool QNodeViewEditor::eventFilter(QObject* object, QEvent* event)
{
  QGraphicsSceneMouseEvent* mouseEvent = static_cast<QGraphicsSceneMouseEvent*>(event);

  switch (event->type())
  {
    case QEvent::GraphicsSceneMousePress:
    {
      switch (mouseEvent->button())
      {
        case Qt::LeftButton:
        {
          QGraphicsItem* item = itemAt(mouseEvent->scenePos());
          if (!item)
            break;

          if (item->type() == QNodeViewType_Port)
          {
            _activeConnection = new QNodeViewConnection(NULL);
            _scene->addItem(_activeConnection);
            _activeConnection->setStartPort(static_cast<QNodeViewPort*>(item));
            _activeConnection->setStartPosition(item->scenePos());
            _activeConnection->setEndPosition(mouseEvent->scenePos());
            _activeConnection->updatePath();
            return true;
          }
          else if (item->type() == QNodeViewType_Block)
          {
            // GW-TODO: Some form of property editor callback?
          }

          break;
        }

        case Qt::RightButton:
        {
          QGraphicsItem* item = itemAt(mouseEvent->scenePos());
          if (!item)
            break;

          const QPoint menuPosition = mouseEvent->screenPos();

          if (item->type() == QNodeViewType_Connection)
          {
            showConnectionMenu(menuPosition, static_cast<QNodeViewConnection*>(item));
            return true;
          }
          else if (item->type() == QNodeViewType_Block)
          {
            showBlockMenu(menuPosition, static_cast<QNodeViewBlock*>(item));
            return true;
          }
          break;
        }
      }
    }

    case QEvent::GraphicsSceneMouseMove:
    {
      if (_activeConnection)
      {
        _activeConnection->setEndPosition(mouseEvent->scenePos());
        _activeConnection->updatePath();
        return true;
      }

      break;
    }

    case QEvent::GraphicsSceneMouseRelease:
    {
      if (_activeConnection && mouseEvent->button() == Qt::LeftButton)
      {
        QGraphicsItem* item = itemAt(mouseEvent->scenePos());
        if (item && item->type() == QNodeViewType_Port)
        {
          QNodeViewPort* startPort = _activeConnection->startPort();
          QNodeViewPort* endPort = static_cast<QNodeViewPort*>(item);

          if (startPort->block() != endPort->block() && startPort->isOutput() != endPort->isOutput()
              && !startPort->isConnected(endPort))
          {
            _activeConnection->setEndPosition(endPort->scenePos());
            _activeConnection->setEndPort(endPort);
            _activeConnection->updatePath();
            _activeConnection = NULL;
            return true;
          }
        }

        _scene->removeItem(_activeConnection);
        delete _activeConnection;
        _activeConnection = NULL;
        return true;
      }

      break;
    }
  }

  return QObject::eventFilter(object, event);
}

//------------------------------------------------------------------------------
QGraphicsItem* QNodeViewEditor::itemAt(const QPointF& point)
{
  Q_ASSERT(_scene);

  QList<QGraphicsItem*> items = _scene->items(QRectF(point - QPointF(1, 1), QSize(3, 3)));

  Q_FOREACH (QGraphicsItem* item, items)
  {
    // Filter out non-user scene items
    if (item->type() > QGraphicsItem::UserType)
      return item;
  }

  // No user scene items found at point
  return NULL;
}

//------------------------------------------------------------------------------
void QNodeViewEditor::showBlockMenu(const QPoint& point, QNodeViewBlock* block)
{
  QMenu menu;
  QAction* deleteAction = menu.addAction("Delete");
  QAction* selection = menu.exec(point);
  if (selection == deleteAction)
  {
    delete block;
  }
}

//------------------------------------------------------------------------------
void QNodeViewEditor::showConnectionMenu(const QPoint& point, QNodeViewConnection* connection)
{
  QMenu menu;
  QAction* deleteAction = menu.addAction("Delete");
  QAction* selection = menu.exec(point);
  if (selection == deleteAction)
  {
    delete connection;
  }
}
