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
#include "QNodeViewCommon.h"

class QNodeViewPort;

class QNodeViewBlock : public QGraphicsPathItem
{
public:
  QNodeViewBlock(const QString& name, QGraphicsItem* parent = NULL);
  virtual ~QNodeViewBlock();

  QNodeViewPort* addPort(const QString& name, bool isOutput);
  void addInputPort(const QString& name, int type);
  void addOutputPort(const QString& name, int type);

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

  QNodeViewBlock* clone();
  QVector<QNodeViewPort*> ports();

  int type() const
  {
    return QNodeViewType_Block;
  }

private:

  QGraphicsTextItem* _label;
  QGraphicsDropShadowEffect _dropShadow;
  qint32 _width;
  qint32 _height;
  qint32 _horizontalMargin;
  qint32 _verticalMargin;
};
