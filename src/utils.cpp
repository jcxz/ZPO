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
    default: return "Uknown format";
  }

  #undef CASE

  return "Uknown format";
}

} // end of namespace utils
