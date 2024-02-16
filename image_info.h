#ifndef IMAGE_INFO_H
#define IMAGE_INFO_H
#include <type_traits>
//#include <QDebug>

template<typename NumericType>
concept numeric = std::is_arithmetic<NumericType>::value;

template<numeric T>
struct position {
  position() = default;
  position(position const&) = default;
  position(T, T) noexcept(std::is_nothrow_move_constructible_v<T>);
  ~position() = default;

  position& operator=(position const&) = default;
  position operator+(position const&) const noexcept;
  position operator-(position const&) const noexcept;
  position& operator+=(position const&) noexcept;
  position& operator-=(position const&) noexcept;

  bool operator==(position const&) const noexcept;
  bool operator!=(position const&) const noexcept;

//  template<numeric T_>
//  friend QDebug operator<<(QDebug, position<T_> const&);

  T x;
  T y;
};

using floating_t = long double;

struct image_info
{
  image_info() = default;
  image_info(position<floating_t> const&, position<int> const&, floating_t, short) noexcept;
  image_info(floating_t stx, floating_t sty, int width, int height, floating_t sc, short bx) noexcept;
  image_info(image_info const&) = default;
  ~image_info() = default;

  image_info& operator=(image_info const&) = default;
  image_info operator+(image_info const&) const;
  image_info& operator+=(image_info const&);

//  friend QDebug operator<<(QDebug, image_info const&);

  position<floating_t> start;
  position<int> size;
  floating_t scale;
  short box_size;
};

#endif // IMAGE_INFO_H
