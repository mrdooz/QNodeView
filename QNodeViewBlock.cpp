/*!
  @file    QNodeViewBlock.cpp

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
#include "QNodeViewPort.h"

QNodeViewBlock::QNodeViewBlock(const QString& name, QGraphicsItem* parent)
    : QGraphicsPathItem(parent)
    , _width(100)
    , _height(5)
    , _horizontalMargin(20)
    , _verticalMargin(5)
{
  setCacheMode(DeviceCoordinateCache);

  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemIsSelectable);

  _label = new QGraphicsTextItem(this);
  _label->setCacheMode(DeviceCoordinateCache);
  _label->setPlainText(name);

  QPainterPath path;
  path.addRoundedRect(-50, -15, 100, 30, 5, 5);
  setPath(path);
  setPen(QPen(QColor(30, 30, 30))); // GW-TODO: Expose to QStyle
  setBrush(QColor(50, 50, 50));     // GW-TODO: Expose to QStyle

  _dropShadow.setBlurRadius(16);
  _dropShadow.setXOffset(0.0);
  _dropShadow.setYOffset(5.0);

  setGraphicsEffect(&_dropShadow);
}

QNodeViewBlock::~QNodeViewBlock()
{
}

QNodeViewPort* QNodeViewBlock::addPort(const QString& name, bool isOutput)
{
  QNodeViewPort* port = new QNodeViewPort(this);
  port->setName(name);
  port->setIsOutput(isOutput);
  port->setBlock(this);

  QFontMetrics fontMetrics(scene()->font());
  const qint32 width = fontMetrics.width(name);
  const qint32 height = fontMetrics.height();

  if (width > _width - _horizontalMargin)
    _width = width + _horizontalMargin;

  _height += height;

  // update block size to accomodate port
  QPainterPath path;
  path.addRoundedRect(-(_width >> 1), -(_height >> 1), _width, _height, 5, 5);
  setPath(path);

  qint32 y = -(_height >> 1) + _verticalMargin + port->radius();

  for (QGraphicsItem* childItem : childItems())
  {
    if (childItem->type() != QNodeViewType_Port)
      continue;

    QNodeViewPort* childPort = static_cast<QNodeViewPort*>(childItem);

    if (childPort->isOutput())
      childPort->setPos((_width >> 1) + childPort->radius(), y);
    else
      childPort->setPos(-(_width >> 1) - childPort->radius(), y);

    y += height;
  }

  return port;
}

void QNodeViewBlock::addInputPort(const QString& name, int /*type*/)
{
  addPort(name, false);
}

void QNodeViewBlock::addOutputPort(const QString& name, int /*type*/)
{
  addPort(name, true);
}

void QNodeViewBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  Q_UNUSED(widget);

  // Only paint dirty regions for increased performance
  painter->setClipRect(option->exposedRect);

  if (isSelected())
  {
    painter->setPen(QPen(QColor(30, 30, 30))); // GW-TODO: Expose to QStyle
    painter->setBrush(QColor(100, 100, 100));  // GW-TODO: Expose to QStyle
  }
  else
  {
    painter->setPen(QPen(QColor(30, 30, 30))); // GW-TODO: Expose to QStyle
    painter->setBrush(QColor(80, 80, 80));     // GW-TODO: Expose to QStyle
  }

  painter->drawPath(path());
}

QNodeViewBlock* QNodeViewBlock::clone()
{
  QNodeViewBlock* block = new QNodeViewBlock(NULL);
  scene()->addItem(block);

  for (QGraphicsItem* childPort : childItems())
  {
    if (childPort->type() == QNodeViewType_Port)
    {
      QNodeViewPort* clonePort = static_cast<QNodeViewPort*>(childPort);
      block->addPort(clonePort->portName(), clonePort->isOutput());
    }
  }

  return block;
}

QVector<QNodeViewPort*> QNodeViewBlock::ports()
{
  QVector<QNodeViewPort*> result;

  for (QGraphicsItem* childItem : childItems())
  {
    if (childItem->type() == QNodeViewType_Port)
      result.append(static_cast<QNodeViewPort*>(childItem));
  }

  return result;
}
