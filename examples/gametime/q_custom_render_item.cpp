#include "q_custom_render_item.hpp"

#include <QPainter>

static std::size_t s_sdf_render(std::size_t x, std::size_t y,
                                std::size_t t) noexcept {
  return (x & y) | t;
}

q_custom_render_item::q_custom_render_item(QQuickPaintedItem *p_parent)
    : QQuickPaintedItem(p_parent),
      _image(width(), height(), QImage::Format_RGBX8888) {
  _gametime.reset();
  QObject::connect(&_timer, &QTimer::timeout, this, &QQuickItem::update);
  _timer.start();
}

void q_custom_render_item::paint(QPainter *p_painter) {
  const auto _w = gsl::narrow_cast<int>(width());
  const auto _h = gsl::narrow_cast<int>(height());

  if (_w != _image.width() || _h != _image.height()) {
    _image = QImage(_w, _h, QImage::Format_RGBX8888);
  }

  for (std::size_t y = 0; y < _image.height(); ++y) {
    for (std::size_t x = 0; x < _image.width(); ++x) {
      _image.setPixel(x, y,
                      s_sdf_render(x, y,
                                   gsl::narrow_cast<size_t>(
                                       _gametime.get_total_secs() * 1000)));
    }
  }

  p_painter->drawImage(boundingRect(), _image);

  _gametime.tick();
  emit fps_changed(_gametime.get_fps());
  emit total_time_changed(_gametime.get_total_secs());
}

uint32_t q_custom_render_item::get_fps() const {
  return this->_gametime.get_fps();
}

uint32_t q_custom_render_item::get_total_time() const {
  return this->_gametime.get_total_secs();
}
