#pragma once
#include "block_loader.hpp"

//------------------------------------------------------------------------------
struct BlockListWidget : public QListWidget
{
  using QListWidget::QListWidget;
  virtual QMimeData* mimeData(const QList<QListWidgetItem*>& items) const;
  virtual QMimeData* mimeData(const QList<QListWidgetItem*> items) const;
};

//------------------------------------------------------------------------------
struct PropertyWidget : public QDialog
{
  PropertyWidget(QWidget* parent = nullptr);

  void clearLayout();
  void addWidget(QWidget* widget);

  vector<QWidget*> _widgets;
  QVBoxLayout* _layout;
};

//------------------------------------------------------------------------------
class RenderArea : public QWidget
{
public:

  QSize RenderArea::minimumSizeHint() const;
  QSize RenderArea::sizeHint() const;

  RenderArea(QWidget *parent = 0);

  QColor _color;

private:
  void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

//------------------------------------------------------------------------------
struct FloatProperty : public QWidget
{
  FloatProperty(const BlockDef::Param& param, QWidget* parent = nullptr);
  void setValue(int value);

  QString _name;
  float _curValue, _minValue, _maxValue;
  QSlider* _slider;
  QLabel* _label;
};

//------------------------------------------------------------------------------
struct ColorProperty : public QWidget
{
  ColorProperty(const BlockDef::Param& param, QWidget* parent = nullptr);
  RenderArea* _renderArea;
  QColor _color;
};
