#ifndef MOVIEPLAYERWINDOW_H
#define MOVIEPLAYERWINDOW_H

#include "Movie.h"

#include <QWidget>
#include <memory>

namespace Ui { class MoviePlayerWindow; }


class MoviePlayerWindow : public QWidget
{
    Q_OBJECT
    
  public:
    explicit MoviePlayerWindow(Movie *movie = nullptr, QWidget *parent = 0);
    ~MoviePlayerWindow(void);

    void setMovie(Movie *movie) { m_movie_ptr.reset(movie); prepareUI(); }

  protected:
    virtual void timerEvent(QTimerEvent * ) override;

  private slots:
    void playStop(void);
    void pauseResume(void);
    void save(void);
    void saveAsImages(void);

  private:
    void resetUI(void);
    void prepareUI(void);

    static bool isMovieValid(Movie *p_movie)
    { return ((p_movie != nullptr) && (p_movie->frameCount() > 0)); }

  private:
    typedef std::unique_ptr<Movie> tMoviePtr;

  private:
    Ui::MoviePlayerWindow *ui;
    tMoviePtr m_movie_ptr;
    int m_timer_id;
    int m_cur_frame;
    bool m_paused;

};

#endif // MOVIEPLAYERWINDOW_H
