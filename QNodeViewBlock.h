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
#include "precompiled.hpp"
#include "QNodeViewCommon.h"
#include "block_loader.hpp"


class QNodeViewPort;

class QNodeViewBlock : public QGraphicsPathItem
{
public:
  QNodeViewBlock(const BlockDef& blockDef, int blockId, QGraphicsItem* parent = NULL);

  void init();

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

  int type() const;

  void updatePropertyWidget();

  void save(QJsonArray* root);
  int blockId() const;

  QNodeViewPort* findPort(int idx);

private:

  QNodeViewPort* addPort(const QString& name, bool isOutput, int type, size_t idx);

  BlockDef _blockDef;
  int _blockId;

  QGraphicsTextItem* _label;
  QGraphicsDropShadowEffect _dropShadow;
  qint32 _width;
  qint32 _height;
  qint32 _horizontalMargin;
  qint32 _verticalMargin;

  unordered_map<int, QNodeViewPort*> _ports;
};
