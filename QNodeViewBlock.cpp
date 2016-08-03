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
#include "block_widgets.hpp"
#include "Example.h"

//------------------------------------------------------------------------------
QNodeViewBlock::QNodeViewBlock(const BlockDef& blockDef, QGraphicsItem* parent)
    : QGraphicsPathItem(parent)
    , _blockDef(blockDef)
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
  _label->setPlainText(blockDef.name.c_str());

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

//------------------------------------------------------------------------------
void QNodeViewBlock::init()
{
  for (const BlockDef::Port& input : _blockDef.inputs)
  {
    addInputPort(input.name.c_str(), input.param.id);
  }

  for (const BlockDef::Port& output : _blockDef.outputs)
  {
    addOutputPort(output.name.c_str(), output.param.id);
  }
}

//------------------------------------------------------------------------------
QNodeViewPort* QNodeViewBlock::addPort(const QString& name, bool isOutput, int parameterType)
{
  QNodeViewPort* port = new QNodeViewPort(parameterType, this);
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

//------------------------------------------------------------------------------
void QNodeViewBlock::addInputPort(const QString& name, int parameterType)
{
  addPort(name, false, parameterType);
}

//------------------------------------------------------------------------------
void QNodeViewBlock::addOutputPort(const QString& name, int parameterType)
{
  addPort(name, true, parameterType);
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
int QNodeViewBlock::type() const
{
  return QNodeViewType_Block;
}

//------------------------------------------------------------------------------
void QNodeViewBlock::updatePropertyWidget()
{
  PropertyWidget* prop = g_mainWindow->_propertyWidget;
  prop->clearLayout();

  for (BlockDef::Param& param : _blockDef.params)
  {
    switch (param.type)
    {
      case BlockDef::Param::Bool:
      {
        break;
      }
      case BlockDef::Param::Int:
      {
        break;
      }
      case BlockDef::Param::Float:
      {
        prop->addWidget(new FloatProperty(&param));
        break;
      }
      case BlockDef::Param::Float2:
      {
        break;
      }
      case BlockDef::Param::Float3:
      {
        break;
      }
      case BlockDef::Param::Color:
      {
        prop->addWidget(new ColorProperty(&param));
        break;
      }
    }
  }
}

//------------------------------------------------------------------------------
void QNodeViewBlock::save(QJsonObject* root)
{
  QJsonObject obj;
  QString name = _blockDef.name.c_str();
  obj["name"] = name;

  QJsonObject objPos;
  objPos["x"] = pos().x();
  objPos["y"] = pos().y();
  obj["pos"] = objPos;

  QJsonArray params;
  for (const BlockDef::Param& param : _blockDef.params)
  {
    QJsonObject objParam;
    QJsonObject objValue;
    switch (param.type)
    {
      case BlockDef::Param::Unknown:
        break;
      case BlockDef::Param::Bool:
        objParam["type"] = "bool";
        objParam["value"] = param.value.toBool();
        break;
      case BlockDef::Param::Int:
        objParam["type"] = "int";
        objParam["value"] = param.value.toInt();
        break;
      case BlockDef::Param::Float:
        objParam["type"] = "float";
        objParam["value"] = param.value.toFloat();
        break;
      case BlockDef::Param::Float2:
      {
        // http://stackoverflow.com/questions/24362946/how-can-i-cast-a-qvariant-to-custom-class
        objParam["type"] = "float2";
        Float2 value = param.value.value<Float2>();
        objValue["x"] = value.x;
        objValue["y"] = value.y;
        objParam["value"] = objValue;
        break;
      }
      case BlockDef::Param::Float3:
      {
        objParam["type"] = "float3";
        Float3 value = param.value.value<Float3>();
        objValue["x"] = value.x;
        objValue["y"] = value.y;
        objValue["z"] = value.z;
        objParam["value"] = objValue;
        break;
      }
      case BlockDef::Param::Color:
      {
        objParam["type"] = "color";
        QColor value = param.value.value<QColor>();
        objValue["r"] = value.red();
        objValue["g"] = value.green();
        objValue["b"] = value.blue();
        objParam["value"] = objValue;
        break;
      }
      default:
        break;
    }

    params.append(objParam);
  }

  obj["params"] = params;
  (*root)[name] = obj;
}

//------------------------------------------------------------------------------
void QNodeViewBlock::load()
{
}

