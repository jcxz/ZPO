#ifndef POINT_H
#define POINT_H

#include <ostream>

template <typename T>
struct Point_
{
  T x;
  T y;

  Point_(void) { }
  Point_(T x, T y) : x(x), y (y) { }

  template <typename U>
  Point_(Point_<U> & pt) : x(pt.x), y(pt.y) { }

  friend std::ostream & operator<<(std::ostream & os, const Point_ & pt)
  {
    return os << "[" << pt.x << "," << pt.y << "]";
  }
};

typedef Point_<float> PointF;
typedef Point_<int> Point;

#endif // POINT_H
