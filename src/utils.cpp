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

#include "utils.h"

#include <QImage>


namespace utils {

const char *qImageFormatToString(int fmt)
{
  #define CASE(e) case e: return #e;

  switch (fmt)
  {
    CASE(QImage::Format_Invalid);
    CASE(QImage::Format_Mono);
    CASE(QImage::Format_MonoLSB);
    CASE(QImage::Format_Indexed8);
    CASE(QImage::Format_RGB32);
    CASE(QImage::Format_ARGB32);
    CASE(QImage::Format_ARGB32_Premultiplied);
    CASE(QImage::Format_RGB16);
    CASE(QImage::Format_ARGB8565_Premultiplied);
    CASE(QImage::Format_RGB666);
    CASE(QImage::Format_ARGB6666_Premultiplied);
    CASE(QImage::Format_RGB555);
    CASE(QImage::Format_ARGB8555_Premultiplied);
    CASE(QImage::Format_RGB888);
    CASE(QImage::Format_RGB444);
    CASE(QImage::Format_ARGB4444_Premultiplied);
    default: return "Unknown format";
  }

  #undef CASE

  return "Unknown format";
}

} // End of namespace utils
