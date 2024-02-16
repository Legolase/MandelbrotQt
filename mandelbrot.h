#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <QMainWindow>
#include <QWidget>
#include <QPointF>
#include "renderer.h"

class mandelbrot : public QWidget {
  Q_OBJECT

  static constexpr floating_t DEFAULT_SCALE = 1/200.;
  static constexpr short DEFAULT_BOX_SIZE = 64;
public:
  static constexpr int DEFAULT_WIDTH = 420;
  static constexpr int DEFAULT_HEIGHT = 300;

  mandelbrot(QWidget *parent = nullptr);

  image_info const& get_info() const noexcept;
  void set_info(image_info const&) noexcept;

public slots:
  void update_image() noexcept;

signals:
  void info_changed();

protected:
  void paintEvent(QPaintEvent* event) noexcept override;
  void mouseMoveEvent(QMouseEvent *event) noexcept override;
  void mousePressEvent(QMouseEvent *event) noexcept override;
  void mouseReleaseEvent(QMouseEvent *event) noexcept override;
  void wheelEvent(QWheelEvent *event) noexcept override;
  void resizeEvent(QResizeEvent *event) noexcept override;

private:
  image_info info;
  QImage image;

  bool pressed_mouse = false;
  QPointF last_mouse_pos;
  renderer tasker;
};
#endif // MANDELBROT_H
