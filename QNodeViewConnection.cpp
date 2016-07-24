/*!
  @file    QNodeViewConnection.cpp

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

#include "QNodeViewConnection.h"
#include "QNodeViewPort.h"

//------------------------------------------------------------------------------
QNodeViewConnection::QNodeViewConnection(QGraphicsItem* parent)
    : QGraphicsPathItem(parent)
{
  setCacheMode(DeviceCoordinateCache);
  setPen(QPen(QColor(170, 170, 170), 2)); // GW-TODO: Expose to QStyle
  setBrush(Qt::NoBrush);
  setZValue(-1);
}

//------------------------------------------------------------------------------
QNodeViewConnection::~QNodeViewConnection()
{
  if (_startPort && _endPort)
  {
    _startPort->connections().remove(_startPort->connections().indexOf(this));
    _endPort->connections().remove(_endPort->connections().indexOf(this));
  }
}

//------------------------------------------------------------------------------
void QNodeViewConnection::setStartPosition(const QPointF& position)
{
  _startPosition = position;
}

//------------------------------------------------------------------------------
void QNodeViewConnection::setEndPosition(const QPointF& position)
{
  _endPosition = position;
}

//------------------------------------------------------------------------------
void QNodeViewConnection::setStartPort(QNodeViewPort* port)
{
  _startPort = port;
}

//------------------------------------------------------------------------------
void QNodeViewConnection::setEndPort(QNodeViewPort* port)
{
  _endPort = port;
}

//------------------------------------------------------------------------------
void QNodeViewConnection::updatePosition()
{
  _startPosition = _startPort->scenePos();
  _endPosition = _endPort->scenePos();
}

//------------------------------------------------------------------------------
void QNodeViewConnection::updatePath()
{
  QPainterPath path;

  QVector<QPointF> curvePoints;
  curvePoints.append(_startPosition);
  curvePoints.append(_endPosition);

  for (qint32 index = 0; index < curvePoints.size() - 1; ++index)
  {
    const QPointF& startPosition = curvePoints[index + 0];
    const QPointF& endPosition = curvePoints[index + 1];

    const qreal deltaX = endPosition.x() - startPosition.x();
    const qreal deltaY = endPosition.y() - startPosition.y();

    QPointF anchor1(startPosition.x() + deltaX * 0.25, startPosition.y() + deltaY * 0.1);
    QPointF anchor2(startPosition.x() + deltaX * 0.75, startPosition.y() + deltaY * 0.9);

    path.moveTo(startPosition);
    path.cubicTo(anchor1, anchor2, endPosition);
  }

  setPath(path);
}

//------------------------------------------------------------------------------
QNodeViewPort* QNodeViewConnection::startPort() const
{
  return _startPort;
}

//------------------------------------------------------------------------------
QNodeViewPort* QNodeViewConnection::endPort() const
{
  return _endPort;
}

//------------------------------------------------------------------------------
QPointF QNodeViewConnection::startPosition() const
{
  return _startPosition;
}

//------------------------------------------------------------------------------
QPointF QNodeViewConnection::endPosition() const
{
  return _endPosition;
}

//------------------------------------------------------------------------------
void QNodeViewConnection::finalize()
{
  // make sure start pos points to the output node
  if (!_startPort || !_endPort)
    return;

  if (!_startPort->isOutput())
  {
    std::swap(_startPort, _endPort);
    std::swap(_startPosition, _endPosition);
  }

  _startPort->connections().append(this);
  _endPort->connections().append(this);

  updatePath();
}

//------------------------------------------------------------------------------
int QNodeViewConnection::type() const
{
  return QNodeViewType_Connection;
}
