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
