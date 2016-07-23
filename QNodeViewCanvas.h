/*!
  @file    QNodeViewBlock.h

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

class QNodeViewCanvas : public QGraphicsView
{
public:
  QNodeViewCanvas(QGraphicsScene* scene, QWidget* parent = NULL);
  virtual ~QNodeViewCanvas();

  void contextMenuEvent(QContextMenuEvent* event);
  void drawBackground(QPainter* painter, const QRectF& rect);

  virtual void dragEnterEvent(QDragEnterEvent* event) Q_DECL_OVERRIDE;
  virtual void dragLeaveEvent(QDragLeaveEvent* event) Q_DECL_OVERRIDE;
  virtual void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;
  virtual void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;

protected:
  virtual void wheelEvent(QWheelEvent* event);


};
