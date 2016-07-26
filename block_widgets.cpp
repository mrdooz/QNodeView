#include "block_widgets.hpp"

//------------------------------------------------------------------------------
QMimeData* BlockListWidget::mimeData(const QList<QListWidgetItem*>& items) const
{
  QListWidgetItem* item = items.front();
  QMimeData* data = new QMimeData();
  data->setText(item->text());
  return data;
}

//------------------------------------------------------------------------------
QMimeData* BlockListWidget::mimeData(const QList<QListWidgetItem*> items) const
{
  QListWidgetItem* item = items.front();
  QMimeData* data = new QMimeData();
  data->setText(item->text());
  return data;
}

//------------------------------------------------------------------------------
QSize BlockListWidget::minimumSizeHint() const
{
  return QSize(100, 200);
}

//------------------------------------------------------------------------------
QSize BlockListWidget::sizeHint() const
{
  return QSize(100, 200);
}

//------------------------------------------------------------------------------
PropertyWidget::PropertyWidget(QWidget* parent) : QDialog(parent)
{
  _layout = new QVBoxLayout();
  _layout->update();
  setLayout(_layout);
}

//------------------------------------------------------------------------------
void PropertyWidget::clearLayout()
{
  for (QWidget* widget : _widgets)
  {
    _layout->removeWidget(widget);
    delete widget;
  }

  _widgets.clear();
}

//------------------------------------------------------------------------------
void PropertyWidget::addWidget(QWidget* widget)
{
  _layout->addWidget(widget);
  _widgets.push_back(widget);
  _layout->update();
}

//------------------------------------------------------------------------------
QSize PropertyWidget::minimumSizeHint() const
{
  return QSize(200, 200);
}

//------------------------------------------------------------------------------
QSize PropertyWidget::sizeHint() const
{
  return QSize(200, 200);
}

//------------------------------------------------------------------------------
FloatProperty::FloatProperty(BlockDef::Param* param, QWidget* parent)
    : QWidget(parent)
    , _param(param)
    , _name(param->name.c_str())
    , _minValue(param->minValue.toFloat())
    , _maxValue(param->maxValue.toFloat())
    , _slider(new QSlider(Qt::Horizontal, this))
    , _label(new QLabel())
{
  connect(_slider, &QSlider::valueChanged, [this](int value) {
    _curValue = _minValue + (_maxValue - _minValue) * value / _slider->maximum();
    _param->value = _curValue;
    _label->setText(QString::asprintf("Value: %f", _curValue));
  });

  // create the layout
  QVBoxLayout* vLayout = new QVBoxLayout();
  QGroupBox* hGroupBox = new QGroupBox();
  QHBoxLayout* hLayout = new QHBoxLayout();
  hLayout->addWidget(new QLabel("hello"));
  hLayout->addWidget(_slider);
  hGroupBox->setLayout(hLayout);

  vLayout->addWidget(hGroupBox);
  vLayout->addWidget(_label);

  // set starting position (either middle or use the default value)
  int pos = 50;
  if (!param->value.isNull())
    pos = 100 * (param->value.toFloat() - _minValue) * (_maxValue - _minValue);
  else if (!param->defaultValue.isNull())
    pos = 100 * (param->defaultValue.toFloat() - _minValue) * (_maxValue - _minValue);
  _slider->setSliderPosition(pos);

  setLayout(vLayout);
}

//------------------------------------------------------------------------------
RenderArea::RenderArea(QWidget* parent) : QWidget(parent)
{
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
  setMaximumSize(50, 50);
}

//------------------------------------------------------------------------------
QSize RenderArea::minimumSizeHint() const
{
  return QSize(50, 50);
}

//------------------------------------------------------------------------------
QSize RenderArea::sizeHint() const
{
  return QSize(50, 50);
}

//------------------------------------------------------------------------------
void RenderArea::paintEvent(QPaintEvent* /* event */)
{
  QRect rect(0, 0, 50, 50);
  QPainter painter(this);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QBrush(_color));
  painter.drawRect(rect);

  painter.setPen(palette().dark().color());
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

//------------------------------------------------------------------------------
int ColorIndex(const QColor& col, size_t idx)
{
  switch (idx)
  {
    case 0: return col.red();
    case 1: return col.green();
    case 2: return col.blue();
    case 3: return col.alpha();
  }

  return -1;
}

//------------------------------------------------------------------------------
ColorProperty::ColorProperty(BlockDef::Param* param, QWidget* parent)
    : QWidget(parent), _param(param), _color(param->defaultValue.value<QColor>())
{
  QGridLayout* controlsLayout = new QGridLayout();

  _renderArea = new RenderArea();
  _renderArea->_color = _color;
  controlsLayout->addWidget(_renderArea, 0, 0, 4, 1);

  int vals[] = {255, 255, 255, 255};
  for (size_t i = 0; i < 4; ++i)
  {
    if (!_param->value.isNull())
      vals[i] = ColorIndex(_param->value.value<QColor>(), i);
    else if (!_param->defaultValue.isNull())
      vals[i] = ColorIndex(_param->defaultValue.value<QColor>(), i);
  }

  const static QString labels[] = {"R", "G", "B", "A"};
  for (size_t i = 0; i < 4; ++i)
  {
    QSlider* slider = new QSlider(Qt::Horizontal, this);
    slider->setMaximum(255);

    QSpinBox* spinBox = new QSpinBox(this);
    spinBox->setMaximum(255);

    connect(slider, &QSlider::valueChanged, spinBox, &QSpinBox::setValue);
    // QOverload needed to disambiguate between overloads,
    // see: http://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), slider, &QSlider::setValue);
    connect(slider, &QSlider::valueChanged, [this, i](int value) {
      switch (i)
      {
        case 0: _color.setRed(value); break;
        case 1: _color.setGreen(value); break;
        case 2: _color.setBlue(value); break;
        case 3: _color.setAlpha(value); break;
      }
      _renderArea->_color = _color;
      _param->value = _color;
      _renderArea->update();
    });

    int row = (int)i;
    int col = 1;
    controlsLayout->addWidget(new QLabel(labels[i]), row, col + 0);
    controlsLayout->addWidget(slider, row, col + 1);
    controlsLayout->addWidget(spinBox, row, col + 2);

    slider->setValue(vals[i]);

  }

  controlsLayout->setRowStretch(4, 1);

  setLayout(controlsLayout);
}
