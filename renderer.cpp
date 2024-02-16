#include "renderer.h"
#include <complex>
#include <cmath>
#include <QThread>

const int renderer::IDEAL_THREAD_NUMBER = (QThread::idealThreadCount() == 1) ? 1 : QThread::idealThreadCount() - 1;
//const int renderer::IDEAL_THREAD_NUMBER = 1;

renderer::renderer() noexcept
{
  workers.reserve(IDEAL_THREAD_NUMBER);
  for (int id = 0; id < IDEAL_THREAD_NUMBER; ++id) {
    workers.emplace_back([id, this]{
      thread_main(id);
    });
  }
}

renderer::~renderer() noexcept
{
  request_stop();
}

void renderer::request_stop()
{
#if USE_RELAXED_ATOMIC
  input_version.store(0, std::memory_order_acquire);
#else
  input_version = 0;
#endif
  input_changed.notify_all();
  for (auto& thread : workers) {
    thread.join();
  }
}

void renderer::thread_main(int id) noexcept
{
  version_t last_input_version = 0;
  auto check_cancel = [&, id, this] {
#if USE_RELAXED_ATOMIC
    if (last_input_version != input_version.load(std::memory_order_relaxed)) {
#else
    if (last_input_version != input_version) {
#endif
      //qDebug() << "Thread" << id << "interrapted";
      return true;
    }
    return false;
  };
  image_ptr last_request;
  uchar* begin;
  image_info inf;
  int bpl;
  //qDebug() << "Thread" << id << "started";
  while(true) {
    {
      //qDebug() << "Thread" << id << "ready";
      std::unique_lock ulk(m);
      input_changed.wait(ulk, check_cancel);
#if USE_RELAXED_ATOMIC
    last_input_version = input_version.load(std::memory_order_release);
#else
    last_input_version = input_version;
#endif
      //qDebug() << "Thread" << id << "updated_version";
      if (last_input_version == STOP_REQUEST) {
        return;
      }
      last_request = current_request;
      inf = info;
      begin = last_request->image().bits();
      bpl = last_request->image().bytesPerLine();
      //qDebug() << "Thread" << id << "updated_info" << inf;
    }
    if (check_cancel()) {
      continue;
    }
    //qDebug() << "Thread" << id << "before_calc";
    if (thread_calc_image(id, inf, begin, check_cancel, bpl)) {
      //qDebug() << "Thread" << id << "after_calc";
      if (check_cancel()) {
        continue;
      }
      last_request->finish();
      if (last_request->get_finished_threads() == IDEAL_THREAD_NUMBER) {
        {
            std::lock_guard lg(m);
            image.swap(last_request->image());
            //qDebug() << "\tThread" << id << "updated_image";
        }
        QMetaObject::invokeMethod(this, "send_image");
        //emit output_ready();
        //qDebug() << "\tThread" << id << "sended_image";

        if (inf.box_size > 1) {
          inf.box_size /= 2;
          send_request(inf);
          //qDebug() << "\tThread" << id << "sended_improve";
        }
      }
    }
  }
}

namespace {
floating_t calculate(floating_t x, floating_t y) {
  static const int max_step = 2000;
  std::complex<floating_t> c(x, y), z = 0;

  for(int step = 0;; ++step) {
    if ((z.real() * z.real() + z.imag()*z.imag()) >= 4.) {
      return (step % 251) / 250.;
    }
    if (step == max_step) {
      return 0;
    }
    z = z * z + c;
  }
}
}

double f(double x) {
  double res = 4. * x * x + 1;
  return (res >= 0) ? res : 0;
}

bool renderer::thread_calc_image(int id, image_info const& inf, uchar* begin, auto& check_cancel, const int bpl) noexcept
{
  const int block_size = inf.size.y / IDEAL_THREAD_NUMBER;
  const int end_local = block_size + ((id + 1 == IDEAL_THREAD_NUMBER) ? inf.size.y % IDEAL_THREAD_NUMBER : 0);
  floating_t result;
  uchar* block_begin = begin + (id * bpl * block_size);
  uchar* p;
  int i;
  //qDebug() << "\tThread" << id << "before_loop";
  for (int j = id * block_size, j_local = 0;j_local < end_local; ++j, ++j_local) {
    p = block_begin + (j_local * bpl);
    if (check_cancel()) return false;
    for (i = 0; i < inf.size.x; ++i) {
      if (check_cancel()) return false;
      if (i % inf.box_size == 0) {
        if (j_local % inf.box_size == 0) {
          result = calculate(inf.start.x + ((i + inf.box_size / 2) * inf.scale), inf.start.y + ((j + inf.box_size / 2) * inf.scale));
          *p++ = static_cast<uchar>(result * result * 0xff);
          *p++ = static_cast<uchar>(result * result * 0xff);
          *p++ = static_cast<uchar>(result * 0xff);
        } else {
          *p++ = p[-bpl];
          *p++ = p[-bpl];
          *p++ = p[-bpl];
        }
      } else {
        *p++ = p[-3];
        *p++ = p[-3];
        *p++ = p[-3];
      }
    }
  }
  //qDebug() << "\tThread" << id << "after_loop";
  return true;
}

void renderer::send_request(const image_info & inf) noexcept
{
  {
    std::lock_guard lg(m);
    current_request = std::make_shared<image_package>(inf);
    info = inf;
    //qDebug() << input_version + 1;
  }
#if USE_RELAXED_ATOMIC
  input_version.fetch_add(1, std::memory_order_acq_rel);
#else
  ++input_version;
#endif
  input_changed.notify_all();
}

QImage renderer::get_image() noexcept
{
  std::lock_guard lg(m);
  return image;
}

void renderer::send_image()
{
  emit output_ready();
}
