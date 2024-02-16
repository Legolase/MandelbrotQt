#include "image_package.h"

image_package::image_package() noexcept
{

}

image_package::image_package(const image_info & info) noexcept(noexcept(QImage(0, 0, std::declval<QImage::Format>())))
    : result(info.size.x, info.size.y, QImage::Format_RGB888)
{

}

QImage &image_package::image() noexcept
{
  return result;
}

image_package::ready_t image_package::get_finished_threads() const noexcept
{
#if USE_RELAXED_ATOMIC
  return ready_threads.load(std::memory_order_release);
#else
  return ready_threads;
#endif
}

void image_package::finish() noexcept
{
#if USE_RELAXED_ATOMIC
  ready_threads.fetch_add(1, std::memory_order_acq_rel);
#else
  ++ready_threads;
#endif
}
