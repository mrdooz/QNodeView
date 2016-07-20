#pragma once

#include <QBrush>
#include <QEvent>
#include <QFileDialog>
#include <QFontMetrics>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGuiApplication>
#include <QJsonDocument.h>
#include <QMenu>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QtWidgets>

#include <algorithm>
#include <deque>
#include <string>
#include <vector>
#include <unordered_map>

#include <stdint.h>

using std::string;
using std::deque;
using std::vector;
using std::unordered_map;
using std::min;
using std::max;

typedef uint32_t u32;