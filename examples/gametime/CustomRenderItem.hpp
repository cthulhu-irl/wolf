#pragma once

#include <QQuickPaintedItem>
#include <QQmlEngine>
#include <QTimer>
#include <QImage>

#include <wolf/system/w_gametime.hpp>

class CustomRenderItem : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(unsigned int fps READ getFps NOTIFY fpsChanged CONSTANT)
    Q_PROPERTY(double total_time READ getTotalTime NOTIFY totalTimeChanged)

public:
    explicit CustomRenderItem(QQuickPaintedItem* p_parent = nullptr);
    void paint(QPainter* p_painter) override;

    Q_INVOKABLE unsigned int getFps() const;
    Q_INVOKABLE unsigned int getTotalTime() const;

signals:
    void fpsChanged(unsigned int fps);
    void totalTimeChanged(double total_time);

private:
    wolf::system::w_gametime gametime_;
    QTimer timer_;
    QImage image_;
};
