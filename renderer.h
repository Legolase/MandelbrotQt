#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <condition_variable>
#include "image_package.h"

class renderer : public QObject
{
  Q_OBJECT

  using image_ptr = std::shared_ptr<image_package>;
  using version_t = int;
  static const int IDEAL_THREAD_NUMBER;
  static const version_t STOP_REQUEST = 0;

  std::mutex m;
  image_info info;
  image_ptr current_request{nullptr};
  QImage image;

  std::atomic<version_t> input_version{0};
  std::condition_variable input_changed;
  std::vector<std::thread> workers;
public:
  renderer() noexcept;
  ~renderer() noexcept;
  void request_stop();

  void thread_main(int id) noexcept;
  bool thread_calc_image(int id, image_info const&, uchar* begin, auto&, const int) noexcept;

  void send_request(image_info const&) noexcept;
  QImage get_image() noexcept;
private slots:
  void send_image();
signals:
  void output_ready();
};

#endif // RENDERER_H
