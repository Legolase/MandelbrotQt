#include "image_info.h"
#include <utility>

image_info::image_info(const position<floating_t> & st, const position<int> & sz, floating_t sc, short bx) noexcept :
      start(st), size(sz), scale(sc), box_size(bx)
{

}

image_info::image_info(floating_t stx, floating_t sty, int width, int height, floating_t sc, short bx) noexcept :
  start(stx, sty), size(width, height), scale(sc), box_size(bx)
{

}

image_info image_info::operator+(const image_info & other) const
{
  return image_info(start + other.start, size + other.size, scale + other.scale, other.box_size);
}

image_info &image_info::operator+=(const image_info &other)
{
  *this = *this + other;
  return *this;
}

//template<numeric T>
//QDebug operator<<(QDebug out, position<T> const& that) {
//  return out << "{ x." << that.x << ", y." << that.y << "}";
//}

//QDebug operator<<(QDebug out, image_info const& that) {
//  return out << "start." << that.start << "size." << that.size << "scale." << that.scale << "box_size." << that.box_size;
//}

template<numeric T>
position<T>::position(T a, T b) noexcept(std::is_nothrow_move_constructible_v<T>) : x(std::move(a)), y(std::move(b))
{

}

template<numeric T>
position<T> position<T>::operator+(const position & other) const noexcept
{
  return position(x + other.x, y + other.y);
}

template<numeric T>
position<T> position<T>::operator-(const position & other) const noexcept
{
  return position(x - other.x, y - other.y);
}

template<numeric T>
position<T> &position<T>::operator+=(const position & other) noexcept
{
  *this = *this + other;
  return *this;
}

template<numeric T>
position<T> &position<T>::operator-=(const position & other) noexcept
{
  *this = *this - other;
  return *this;
}

template<numeric T>
bool position<T>::operator==(const position & other) const noexcept
{
  return (x == other.x) && (y == other.y);
}

template<numeric T>
bool position<T>::operator!=(const position & other) const noexcept
{
  return !(*this == other);
}
