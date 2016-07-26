#pragma once
#include "block_loader.hpp"

//------------------------------------------------------------------------------
struct BlockListWidget : public QListWidget
{
  using QListWidget::QListWidget;
  virtual QMimeData* mimeData(const QList<QListWidgetItem*>& items) const;
  virtual QMimeData* mimeData(const QList<QListWidgetItem*> items) const;

  QSize minimumSizeHint() const;
  QSize sizeHint() const;
};

//------------------------------------------------------------------------------
struct PropertyWidget : public QDialog
{
  PropertyWidget(QWidget* parent = nullptr);

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  void clearLayout();
  void addWidget(QWidget* widget);

  vector<QWidget*> _widgets;
  QVBoxLayout* _layout;
};

//------------------------------------------------------------------------------
struct RenderArea : public QWidget
{
  RenderArea(QWidget *parent = 0);

  QSize RenderArea::minimumSizeHint() const;
  QSize RenderArea::sizeHint() const;

  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

  QColor _color;
};

//------------------------------------------------------------------------------
struct FloatProperty : public QWidget
{
  FloatProperty(BlockDef::Param* _param, QWidget* parent = nullptr);

  BlockDef::Param* _param;
  QString _name;
  float _curValue, _minValue, _maxValue;
  QSlider* _slider;
  QLabel* _label;
};

//------------------------------------------------------------------------------
struct ColorProperty : public QWidget
{
  ColorProperty(BlockDef::Param* param, QWidget* parent = nullptr);
  BlockDef::Param* _param;
  RenderArea* _renderArea;
  QColor _color;
};
