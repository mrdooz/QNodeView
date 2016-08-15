/*!
  @file    QNodeViewPort.cpp

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

#include "QNodeViewPort.h"
#include "QNodeViewConnection.h"

//------------------------------------------------------------------------------
QNodeViewPort::QNodeViewPort(int parameterType, size_t idx, QGraphicsItem* parent)
    : QGraphicsPathItem(parent), _idx(idx), _radius(5), _margin(2), _parameterType(parameterType), _state(State::None)
{
  setCacheMode(DeviceCoordinateCache);

  setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

  _label = new QGraphicsTextItem(this);
  _label->setCacheMode(DeviceCoordinateCache);

  QPainterPath path;
  path.addEllipse(-_radius, -_radius, _radius * 2, _radius * 2);
  setPath(path);

  setPen(QPen(QColor(100, 100, 100))); // GW-TODO: Expose to QStyle
  setBrush(QColor(155, 155, 155));     // GW-TODO: Expose to QStyle
}

//------------------------------------------------------------------------------
QNodeViewPort::~QNodeViewPort()
{
  Q_FOREACH (QNodeViewConnection* connection, _connections)
    delete connection;
}

//------------------------------------------------------------------------------
void QNodeViewPort::setBlock(QNodeViewBlock* block)
{
  _block = block;
}

//------------------------------------------------------------------------------
void QNodeViewPort::setName(const QString& name)
{
  _name = name;
  _label->setPlainText(name);
}

//------------------------------------------------------------------------------
void QNodeViewPort::setIsOutput(bool isOutput)
{
  _isOutput = isOutput;

  const qreal boundingWidth = _label->boundingRect().width();
  const qreal boundingHalfHeight = _label->boundingRect().height() / 2;

  if (_isOutput)
    _label->setPos(-_radius - _margin - boundingWidth, -boundingHalfHeight);
  else
    _label->setPos(_radius + _margin, -boundingHalfHeight);

  _label->setDefaultTextColor(QColor(155, 155, 155)); // GW-TODO: Expose to QStyle
}

//------------------------------------------------------------------------------
bool QNodeViewPort::isConnected(QNodeViewPort* other)
{
  for (QNodeViewConnection* connection : _connections)
  {
    if (connection->startPort() == other || connection->endPort() == other)
      return true;
  }

  return false;
}

//------------------------------------------------------------------------------
bool QNodeViewPort::isOutput()
{
  return _isOutput;
}

//------------------------------------------------------------------------------
qint32 QNodeViewPort::radius()
{
  return _radius;
}

//------------------------------------------------------------------------------
QVector<QNodeViewConnection*>& QNodeViewPort::connections()
{
  return _connections;
}

//------------------------------------------------------------------------------
QNodeViewBlock* QNodeViewPort::block() const
{
  return _block;
}

//------------------------------------------------------------------------------
QVariant QNodeViewPort::itemChange(GraphicsItemChange change, const QVariant& value)
{
  if (change == ItemScenePositionHasChanged)
  {
    for (QNodeViewConnection* connection : _connections)
    {
      connection->updatePosition();
      connection->updatePath();
    }
  }

  return value;
}

//------------------------------------------------------------------------------
void QNodeViewPort::setState(State state)
{
  // reset to default state
  setPen(QPen(QColor(100, 100, 100)));
  setBrush(QColor(155, 155, 155));

  QPainterPath path;
  path.addEllipse(-_radius, -_radius, _radius * 2, _radius * 2);
  setPath(path);

  if (state == State::DragStart)
  {
    setPen(QPen(QColor(100, 200, 100)));
    setBrush(QColor(155, 255, 155));
  }
  else if (state == State::DragValid)
  {
    setPen(QPen(QColor(100, 200, 100)));
    setBrush(QColor(155, 255, 155));

    QPainterPath path;
    path.addEllipse(-_radius, -_radius, _radius * 2.5, _radius * 2.5);
    setPath(path);
  }
  else if (state == State::DragInvalid)
  {
    setPen(QPen(QColor(200, 100, 100)));
    setBrush(QColor(255, 155, 155));
  }

  _state = state;
}

//------------------------------------------------------------------------------
int QNodeViewPort::parameterType() const
{
  return _parameterType;
}

//------------------------------------------------------------------------------
const QString& QNodeViewPort::portName() const
{
  return _name;
}

//------------------------------------------------------------------------------
int QNodeViewPort::type() const
{
  return QNodeViewType_Port;
}
