/*
 * Copyright (C) 2014 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>
 *
 * This file is part of ZPO.
 *
 * ZPO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ZPO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ZPO. If not, see <http://www.gnu.org/licenses/>.
 */

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
