/*!
  @file    QNodeViewPort.h

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

class QNodeViewBlock;
class QNodeViewConnection;

class QNodeViewPort : public QGraphicsPathItem
{
public:
  QNodeViewPort(int parameterType, QGraphicsItem* parent = NULL);
  virtual ~QNodeViewPort();

  void setBlock(QNodeViewBlock* block);
  void setName(const QString& name);
  void setIsOutput(bool isOutput);

  bool isConnected(QNodeViewPort*);
  bool isOutput();

  qint32 radius();
  QVector<QNodeViewConnection*>& connections();
  QNodeViewBlock* block() const;

  const QString& portName() const;
  int type() const;

  int parameterType() const;

  enum class State
  {
    None,
    DragStart,
    DragValid,
    DragInvalid,
  };

  void setState(State state);

private:

  QVariant itemChange(GraphicsItemChange change, const QVariant& value);

  QVector<QNodeViewConnection*> _connections;
  QString _name;
  QNodeViewBlock* _block;
  QGraphicsTextItem* _label;

  qint32 _radius;
  qint32 _margin;

  bool _isOutput;
  int _parameterType;

  // if the port is part of any drag/drop operation, keep track of its state
  State _state;

};
