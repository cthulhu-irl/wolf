#pragma once

#include <QImage>
#include <QQmlEngine>
#include <QQuickPaintedItem>
#include <QTimer>
#include <wolf/system/w_gametime.hpp>

class q_custom_render_item : public QQuickPaintedItem {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(uint32_t fps READ get_fps NOTIFY fps_changed CONSTANT)
  Q_PROPERTY(double total_time READ get_total_time NOTIFY total_time_changed)

 public:
  explicit q_custom_render_item(QQuickPaintedItem *p_parent = nullptr);
  void paint(QPainter *p_painter) override;

  Q_INVOKABLE uint32_t get_fps() const;
  Q_INVOKABLE uint32_t get_total_time() const;

 signals:
  void fps_changed(unsigned int fps);
  void total_time_changed(double total_time);

 private:
  wolf::system::w_gametime _gametime;
  QTimer _timer;
  QImage _image;
};
