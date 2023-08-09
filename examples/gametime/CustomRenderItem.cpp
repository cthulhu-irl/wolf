#include "CustomRenderItem.hpp"

#include <QPainter>

static std::size_t s_sdf_render(std::size_t x, std::size_t y, std::size_t t) noexcept
{
    return (x & y) | t;
}

CustomRenderItem::CustomRenderItem(QQuickPaintedItem *p_parent)
    : QQuickPaintedItem(p_parent), _image(width(), height(), QImage::Format_RGBX8888)
{
    _gametime.reset();
    QObject::connect(&_timer, &QTimer::timeout, this, &QQuickItem::update);
    _timer.start();
}

void CustomRenderItem::paint(QPainter *p_painter)
{
    if (width() != _image.width() || height() != _image.height())
    {
        _image = QImage(width(), height(), QImage::Format_RGBX8888);
    }

    for (std::size_t y = 0; y < _image.height(); ++y)
    {
        for (std::size_t x = 0; x < _image.width(); ++x)
        {
            _image.setPixel(x, y, s_sdf_render(x, y, _gametime.get_total_secs() * 1000));
        }
    }

    p_painter->drawImage(boundingRect(), _image);

    _gametime.tick();
    emit fpsChanged(_gametime.get_fps());
    emit totalTimeChanged(_gametime.get_total_secs());
}

uint32_t CustomRenderItem::getFps() const
{
    return _gametime.get_fps();
}

uint32_t CustomRenderItem::getTotalTime() const
{
    return _gametime.get_total_secs();
}
