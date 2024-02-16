#ifndef IMAGE_PACKAGE_H
#define IMAGE_PACKAGE_H

#include "image_info.h"
#include <QImage>
#include <QObject>
#include <atomic>

class image_package
{
public:
  using ready_t = int;
private:
  QImage result;

  std::atomic<ready_t> ready_threads{0};
public:
  image_package() noexcept;
  image_package(image_package const&) = delete;
  image_package(image_info const&) noexcept(noexcept(QImage(0, 0, std::declval<QImage::Format>())));

  ~image_package() = default;

  QImage& image() noexcept;
  ready_t get_finished_threads() const noexcept;
  void finish() noexcept;
};

#define USE_RELAXED_ATOMIC 1

#endif // IMAGE_PACKAGE_H
