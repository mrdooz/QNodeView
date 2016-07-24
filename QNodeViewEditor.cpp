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

#include "QNodeViewBlock.h"
#include "QNodeViewCommon.h"
#include "QNodeViewConnection.h"
#include "QNodeViewEditor.h"
#include "QNodeViewPort.h"

//------------------------------------------------------------------------------
QNodeViewEditor::QNodeViewEditor(QGraphicsScene* scene, QObject* parent)
    : QObject(parent), _scene(scene), _activeConnection(NULL)
{
  scene->installEventFilter(this);
}

//------------------------------------------------------------------------------
bool QNodeViewEditor::validEndPort(QGraphicsItem* item)
{
  if (!_activeConnection)
    return false;

  if (!item || item->type() != QNodeViewType_Port)
    return false;

  QNodeViewPort* startPort = _activeConnection->startPort();
  QNodeViewPort* endPort = (QNodeViewPort*)(item);

  if (startPort == endPort || startPort->parameterType() != endPort->parameterType())
    return false;

  // make sure startPort is always the output port
  if (!startPort->isOutput())
    std::swap(startPort, endPort);

  return startPort->block() != endPort->block() && startPort->isOutput() && !endPort->isOutput()
         && endPort->connections().empty();
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
            _activeConnection->startPort()->setState(QNodeViewPort::State::DragStart);

            return true;
          }
          else if (item->type() == QNodeViewType_Block)
          {
            QNodeViewBlock* block = (QNodeViewBlock*)item;
            block->propertyWidget();
          }

          break;
        }

        case Qt::RightButton:
        {
          QGraphicsItem* item = itemAt(mouseEvent->scenePos());
          if (!item)
          {
            break;
          }

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
        if (_lastPort)
          _lastPort->setState(QNodeViewPort::State::None);

        _activeConnection->setEndPosition(mouseEvent->scenePos());
        _activeConnection->updatePath();

        QGraphicsItem* item = itemAt(mouseEvent->scenePos());
        if (validEndPort(item))
        {
          ((QNodeViewPort*)item)->setState(QNodeViewPort::State::DragValid);
          _lastPort = (QNodeViewPort*)item;
        }
        else if (item && item->type() == QNodeViewType_Port && item != _activeConnection->startPort())
        {
          ((QNodeViewPort*)item)->setState(QNodeViewPort::State::DragInvalid);
          _lastPort = (QNodeViewPort*)item;
        }

        return true;
      }

      break;
    }

    case QEvent::GraphicsSceneMouseRelease:
    {
      if (_activeConnection && mouseEvent->button() == Qt::LeftButton)
      {
        _activeConnection->startPort()->setState(QNodeViewPort::State::None);

        QGraphicsItem* item = itemAt(mouseEvent->scenePos());
        if (item && item->type() == QNodeViewType_Port)
        {
          QNodeViewPort* endPort = (QNodeViewPort*)(item);
          endPort->setState(QNodeViewPort::State::None);

          if (validEndPort(endPort))
          {
            _activeConnection->setEndPosition(endPort->scenePos());
            _activeConnection->setEndPort(endPort);
            _activeConnection->finalize();
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

  for (QGraphicsItem* item : _scene->items(QRectF(point - QPointF(1, 1), QSize(3, 3))))
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
