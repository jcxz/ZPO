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

#include "Movie.h"
#include "utils.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



static bool qImage2CvMat(const QImage & img, cv::Mat & mat)
{
  // the const cast is here because the function called is constBits which returns a const pointer
  // and constBits() in turn is called because it does not detach the pixel buffer from QImage,
  // which is the desired behaviour in this case.
  // The cost_cast should also be safe here, since cvtColor method should not write to the
  // pixels provided

  QImage::Format img_fmt = img.format();
  if ((img_fmt == QImage::Format_ARGB32) || (img_fmt == QImage::Format_RGB32))
  {
    cv::Mat tmp(img.height(), img.width(), CV_8UC4, const_cast<uchar*>(img.constBits()), img.bytesPerLine());
    cv::cvtColor(tmp, mat, CV_BGRA2BGR);
  }
  else if (img_fmt == QImage::Format_RGB888)
  {
    cv::Mat tmp(img.height(), img.width(), CV_8UC3, const_cast<uchar*>(img.constBits()), img.bytesPerLine());
    cv::cvtColor(tmp, mat, CV_RGB2BGR);
  }
  else
  {
    return false;
  }

  return true;
}


bool Movie::saveAsAVI(const std::string & filename, int fourcc) const
{
  if (m_frames.empty())
  {
    WARNM("There are no frames in the movie");
    return true;
  }

  cv::Size frame_size(m_frame_w, m_frame_h);
  cv::VideoWriter writer(filename, fourcc, m_fps, frame_size, true);
  if (!writer.isOpened())
  {
    ERRORM("Failed to create video writer");
    return false;
  }

  cv::Mat frame;

  for (int i = 0; i < m_frames.size(); ++i)
  {
    if (!qImage2CvMat(m_frames[i], frame))
    {
      ERRORM("Unsupported conversion from "
             << utils::qImageFormatToString(m_frames[i].format())
             << " pixel format");
      return false;
    }

    writer.write(frame);
  }

  return true;
}



#if 0
bool Movie::saveAsAVI(const std::string & filename, int fourcc)
{
  if (m_frames.empty())
  {
    WARNM("There are no frames in the movie");
    return true;
  }

  cv::Size frame_size(m_frame_w, m_frame_h);
  cv::VideoWriter writer(filename, fourcc, m_fps, frame_size, true);
  if (!writer.isOpened())
  {
    ERRORM("Failed to create video writer");
    return false;
  }

  //cv::Mat cv_frame(m_frame_h, m_frame_w, CV_8UC3, nullptr);
  cv::Mat cv_frame(m_frame_h, m_frame_w, CV_8UC4, nullptr);

  for (int i = 0; i < m_frames.size(); ++i)
  {
    QImage & frame = m_frames[i];

    if (frame.format() != QImage::Format_RGB888)
    {
      INFOM("Converting frame #" << i << " from "
            << utils::qImageFormatToString(frame.format())
            << " to QImage::Format_RGB888");

      frame = frame.convertToFormat(QImage::Format_RGB888);
      if (frame.isNull())
      {
        ERRORM("Failed to convert pixel format of frame #" << i << " to QImage::Format_RGB888");
        return false;
      }
    }

    // the const cast is here because the function called is constBits which returns a const pointer
    // and constBits() in turn is called because it does not detach the pixel buffer from QImage,
    // which is the desired behaviour in this case.
    // The cost_cast should also be safe here, since write method of VideoWriter should write to the
    // pixels provided
    cv_frame.data = const_cast<uchar*>(frame.constBits());
    writer.write(cv_frame);
  }

  return true;
}
#endif


bool Movie::saveAsImageSequence(const std::string & filename) const
{
  QString name((filename + "_%1.jpg").c_str());

  bool ret = true;

  for (int i = 0; i < m_frames.size(); ++i)
  {
    if (!m_frames[i].save(name.arg(i)))
    {
      WARNM("Failed to save frame #" << i << " to file " << name.toStdString());
      ret = false;
    }
  }

  return ret;
}
