#include "mandelbrot.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QSize>
#include <QKeyEvent>

mandelbrot::mandelbrot(QWidget *parent)
    : QWidget(parent), info(-((DEFAULT_WIDTH)/2. * DEFAULT_SCALE),
                                -((DEFAULT_HEIGHT)/2. * DEFAULT_SCALE),
                                DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_SCALE, 1)
{
  resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    connect(&tasker, &renderer::output_ready, this, &mandelbrot::update_image);
}

const image_info &mandelbrot::get_info() const noexcept
{
    return info;
}

void mandelbrot::set_info(const image_info &inf) noexcept
{
    info = inf;
    info.box_size = DEFAULT_BOX_SIZE;
    tasker.send_request(info);
}

void mandelbrot::update_image() noexcept
{
    emit info_changed();
    update();
}

void mandelbrot::paintEvent(QPaintEvent *event) noexcept
{
  QPainter(this).drawImage(0, 0, tasker.get_image());
}

void mandelbrot::mouseMoveEvent(QMouseEvent *event) noexcept
{
  if (!pressed_mouse) {
    event->ignore();
  }
  QPointF move = last_mouse_pos - event->position();
  floating_t scale = info.scale;
  last_mouse_pos = event->position();
  info += image_info(move.x() * scale, move.y() * scale, 0, 0, 0, DEFAULT_BOX_SIZE);
  tasker.send_request(info);
  event->accept();
}

void mandelbrot::mousePressEvent(QMouseEvent *event) noexcept
{
  if (event->button() == Qt::LeftButton) {
    pressed_mouse = true;
    last_mouse_pos = event->position();
    event->accept();
    return;
  }
  event->ignore();
}

void mandelbrot::mouseReleaseEvent(QMouseEvent *event) noexcept
{
  if (event->button() == Qt::LeftButton) {
    pressed_mouse = false;
    event->accept();
    return;
  }
  event->ignore();
}

void mandelbrot::wheelEvent(QWheelEvent *event) noexcept
{
  QPointF real_pos = event->position() * info.scale;
  floating_t next_scale;
  if (event->angleDelta().y() > 0) {
    next_scale = info.scale * std::pow(0.9, event->angleDelta().y() / 180.);
  } else {
    next_scale = info.scale / std::pow(0.9, -event->angleDelta().y() / 180.);
  }
  real_pos -= event->position() * next_scale;
  info += image_info(real_pos.x(), real_pos.y(), 0, 0, next_scale - info.scale, DEFAULT_BOX_SIZE);
  tasker.send_request(info);
  event->accept();
}

void mandelbrot::resizeEvent(QResizeEvent *event) noexcept
{
  QSize next = event->size();
  info.size.x = next.width();
  info.size.y = next.height();
  info.box_size = DEFAULT_BOX_SIZE;
  tasker.send_request(info);
  QWidget::resize(next);
  event->accept();
}

