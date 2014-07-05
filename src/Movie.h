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

#ifndef MOVIE_H
#define MOVIE_H

#include "debug.h"

#include <QVector>
#include <QImage>


// podla vsetkeho (na zaklade standardu) toto nemoze byt definovane
// ako member funkcia (ani staticka)
constexpr inline int makeFourcc(char c1, char c2, char c3, char c4)
{
  return ((((int) (c4)) << 24) | (((int) (c3)) << 16) | (((int) (c2)) <<  8) | ((int) (c1)));
}

// Tato trieda reprezentuje video vygenerovane
// morphovacim algoritmom
class Movie
{
  private:
    static constexpr int DEFAULT_FPS = 24;
    static constexpr int DEFAULT_FOURCC = makeFourcc('D', 'I', 'V', 'X');
    static constexpr int INVALID_FRAME_LENGTH = -1;

  public:
    explicit Movie(int fps = DEFAULT_FPS)
      : m_frames()
      , m_fps(fps)
      , m_frame_w(INVALID_FRAME_LENGTH)
      , m_frame_h(INVALID_FRAME_LENGTH)
    { }

    int fps(void) const { return m_fps; }
    void setFPS(int fps) { m_fps = fps; }

    int frameCount(void) const { return m_frames.size(); }
    int frameWidth(void) const { return m_frame_w; }
    int frameHeigth(void) const { return m_frame_h; }
    const QImage & frame(int idx) const { return m_frames[idx]; }

    bool addFrame(const QImage & frame)
    {
      int frame_w = frame.width();
      int frame_h = frame.height();

      if (m_frames.empty())
      {
        m_frame_w = frame_w;
        m_frame_h = frame_h;
      }
      else
      {
        if ((frame_w != m_frame_w) || (frame_h != m_frame_h))
        {
          ERRORM("Invalid frame size");
          return false;
        }
      }

      m_frames.push_back(frame);

      return true;
    }

    void clear(void)
    {
      m_frames.clear();
      m_frame_w = INVALID_FRAME_LENGTH;
      m_frame_h = INVALID_FRAME_LENGTH;
    }

    bool saveAsAVI(const std::string & filename, int fourcc = DEFAULT_FOURCC) const;

    // The filename is the base name without the extension
    bool saveAsImageSequence(const std::string & filename = "frame") const;

  public:
    QVector<QImage> m_frames;
    int m_fps;
    int m_frame_w;
    int m_frame_h;
};

#endif
