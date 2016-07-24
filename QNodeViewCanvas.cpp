/*!
  @file    QNodeViewCanvas.cpp

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
#include "QNodeViewCanvas.h"
#include "block_loader.hpp"

//------------------------------------------------------------------------------
QNodeViewCanvas::QNodeViewCanvas(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent)
{
  setRenderHint(QPainter::Antialiasing, true);
  setAcceptDrops(true);
}

//------------------------------------------------------------------------------
void QNodeViewCanvas::drawBackground(QPainter* painter, const QRectF& rect)
{
  // GW-TODO: Expose this to QStyle
  painter->fillRect(rect, QBrush(QColor(50, 50, 50)));

  const qint32 gridInterval = 50;
  painter->setWorldMatrixEnabled(true);

  // GW-TODO: Expose this to QStyle
  QPen linePen(QColor(80, 80, 80), 1, Qt::DotLine, Qt::FlatCap, Qt::RoundJoin);
  linePen.setCosmetic(true); // Performance optimization
  painter->setPen(linePen);

  const qreal left = qint32(rect.left()) - (qint32(rect.left()) % gridInterval);
  const qreal top = qint32(rect.top()) - (qint32(rect.top()) % gridInterval);

  QVarLengthArray<QLineF, 100> linesX;
  for (qreal x = left; x < rect.right(); x += gridInterval)
    linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

  QVarLengthArray<QLineF, 100> linesY;
  for (qreal y = top; y < rect.bottom(); y += gridInterval)
    linesY.append(QLineF(rect.left(), y, rect.right(), y));

  painter->drawLines(linesX.data(), linesX.size());
  painter->drawLines(linesY.data(), linesY.size());
}

//------------------------------------------------------------------------------
void QNodeViewCanvas::dragEnterEvent(QDragEnterEvent* event)
{
  if (event->mimeData()->hasText())
    event->acceptProposedAction();
}

//------------------------------------------------------------------------------
void QNodeViewCanvas::dragMoveEvent(QDragMoveEvent* event)
{
  if (event->mimeData()->hasText())
    event->acceptProposedAction();
}

//------------------------------------------------------------------------------
void QNodeViewCanvas::dropEvent(QDropEvent* event)
{
  QString blockType = event->mimeData()->text();

  // find block def, and create it
  auto it = blockDefs.find(blockType.toStdString());
  if (it == blockDefs.end())
  {
    qWarning() << "Unable to find block def for: " << blockType;
    return;
  }

  const BlockDef& blockDef = it->second;

  QNodeViewBlock* block = new QNodeViewBlock(blockDef);
  scene()->addItem(block);
  block->init();
  block->setPos(mapToScene(event->pos()));
}

//------------------------------------------------------------------------------
void QNodeViewCanvas::wheelEvent(QWheelEvent* event)
{
  setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  double scaleFactor = 1.15;

  if (event->delta() > 0)
  {
    scale(scaleFactor, scaleFactor);
  }
  else
  {
    scale(1.0 / scaleFactor, 1.0 / scaleFactor);
  }
}
