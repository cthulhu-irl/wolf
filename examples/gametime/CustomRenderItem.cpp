#include "CustomRenderItem.hpp"

#include <QPainter>

static std::size_t sdf_render(std::size_t x, std::size_t y, std::size_t t) noexcept
{
    return (x & y) | t;
}

CustomRenderItem::CustomRenderItem(QQuickPaintedItem* p_parent)
    : QQuickPaintedItem(p_parent)
    , image_(width(), height(), QImage::Format_RGBX8888)
{
    gametime_.reset();
    QObject::connect(&timer_, &QTimer::timeout, this, &QQuickItem::update);
    timer_.start();
}

void CustomRenderItem::paint(QPainter* p_painter)
{
    if (width() != image_.width() || height() != image_.height()) {
        image_ = QImage(width(), height(), QImage::Format_RGBX8888);
    }

    for (std::size_t y = 0; y < image_.height(); ++y) {
        for (std::size_t x = 0; x < image_.width(); ++x) {
            image_.setPixel(x, y, sdf_render(x, y, gametime_.get_total_secs() * 1000));
        }
    }

    p_painter->drawImage(boundingRect(), image_);

    gametime_.tick();
    emit fpsChanged(gametime_.get_fps());
    emit totalTimeChanged(gametime_.get_total_secs());
}

unsigned int CustomRenderItem::getFps() const
{
    return gametime_.get_fps();
}

unsigned int CustomRenderItem::getTotalTime() const
{
    return gametime_.get_total_secs();
}
