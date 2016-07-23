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

QNodeViewConnection::QNodeViewConnection(QGraphicsItem* parent)
    : QGraphicsPathItem(parent), m_startPort(NULL), m_endPort(NULL)
{
  setCacheMode(DeviceCoordinateCache);
  setPen(QPen(QColor(170, 170, 170), 2)); // GW-TODO: Expose to QStyle
  setBrush(Qt::NoBrush);
  setZValue(-1);
}

QNodeViewConnection::~QNodeViewConnection()
{
  if (m_startPort)
    m_startPort->connections().remove(m_startPort->connections().indexOf(this));

  if (m_endPort)
    m_endPort->connections().remove(m_endPort->connections().indexOf(this));
}

void QNodeViewConnection::setStartPosition(const QPointF& position)
{
  m_startPosition = position;
}

void QNodeViewConnection::setEndPosition(const QPointF& position)
{
  m_endPosition = position;
}

void QNodeViewConnection::setStartPort(QNodeViewPort* port)
{
  m_startPort = port;
  m_startPort->connections().append(this);
}

void QNodeViewConnection::setEndPort(QNodeViewPort* port)
{
  m_endPort = port;
  m_endPort->connections().append(this);
}

void QNodeViewConnection::updatePosition()
{
  m_startPosition = m_startPort->scenePos();
  m_endPosition = m_endPort->scenePos();
}

void QNodeViewConnection::updatePath()
{
  QPainterPath path;

  QVector<QPointF> curvePoints;
  curvePoints.append(m_startPosition);

  curvePoints.append(m_endPosition);

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

QNodeViewPort* QNodeViewConnection::startPort() const
{
  return m_startPort;
}

QNodeViewPort* QNodeViewConnection::endPort() const
{
  return m_endPort;
}
