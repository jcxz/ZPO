#include "MoviePlayerWindow.h"
#include "ui_MoviePlayerWindow.h"
#include "debug.h"

#include <QFileDialog>
#include <QMessageBox>



MoviePlayerWindow::MoviePlayerWindow(Movie *movie, QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::MoviePlayerWindow)
  , m_timer_id(0)
  , m_cur_frame(0)
  , m_paused(false)
{
  ui->setupUi(this);
  setMovie(movie);
}


MoviePlayerWindow::~MoviePlayerWindow(void)
{
  delete ui;
}


void MoviePlayerWindow::playStop(void)
{
  Movie *p_movie = m_movie_ptr.get();
  if (isMovieValid(p_movie))
  {
    if ((m_timer_id == 0) && (m_paused == false))  // isStopped -> play
    {
      m_cur_frame = 0;
      m_timer_id = startTimer(1000 / p_movie->fps());
      if (m_timer_id == 0)
      {
        WARNM("Failed to start timer for movie player");
      }
      ui->pbPlayStop->setText(tr("Stop"));
      ui->pbPauseResume->setEnabled(true);
    }
    else  // isPlaying/isPaused -> stop
    {
      killTimer(m_timer_id);
      m_timer_id = 0;
      m_paused = false;
      m_cur_frame = 0;
      resetUI();
    }
  }
  else
  {
    WARNM("No movie to be played");
  }
}


void MoviePlayerWindow::pauseResume(void)
{
  Movie *p_movie = m_movie_ptr.get();
  if (isMovieValid(p_movie))
  {
    if (m_timer_id == 0)  // isPaused -> play
    {
      m_timer_id = startTimer(1000 / p_movie->fps());
      if (m_timer_id == 0)
      {
        WARNM("Failed to start timer for movie player");
      }
      m_paused = false;
      ui->pbPauseResume->setText(tr("Pause"));
    }
    else  // isPlaying -> pause
    {
      killTimer(m_timer_id);
      m_timer_id = 0;
      m_paused = true;
      ui->pbPauseResume->setText(tr("Resume"));
    }
  }
  else
  {
    WARNM("No movie to be paused");
  }
}


void MoviePlayerWindow::resetUI(void)
{
  ui->pbPauseResume->setText(tr("Pause"));
  ui->pbPauseResume->setEnabled(false);
  ui->pbPlayStop->setText(tr("Play"));
  //ui->lblFrame->setPixmap(QPixmap());
  ui->lblFrame->setPixmap(QPixmap::fromImage(m_movie_ptr->frame(0)));
}


void MoviePlayerWindow::timerEvent(QTimerEvent * )
{
  // toto by tu malo byt uz vzdy v poriadku,
  // ale pre istotu to tu este kontrolujem
  Movie *p_movie = m_movie_ptr.get();
  if (!isMovieValid(p_movie))
  {
    WARNM("Invalid movie");
    return;
  }

  if (m_cur_frame >= p_movie->frameCount())
  {
    if (ui->cbLoopInfinitely->isChecked())
    {
      m_cur_frame = 0;
    }
    else
    {
      killTimer(m_timer_id);
      m_timer_id = 0;
      m_paused = false;
      resetUI();
      return;
    }
  }

  ui->lblFrame->setPixmap(QPixmap::fromImage(p_movie->frame(m_cur_frame)));
  ++m_cur_frame;
}


void MoviePlayerWindow::save(void)
{
  Movie *p_movie = m_movie_ptr.get();
  if (!isMovieValid(p_movie))
  {
    WARNM("Invalid movie");
    return;
  }

  QString filename(QFileDialog::getSaveFileName(this, tr("Video filename"),
                                                "./morph.avi",
                                                tr("Video Files (*.avi);;All Files (*.*)")));
  if (filename.isNull()) return;

  if (!p_movie->saveAsAVI(filename.toStdString()))
  {
    QMessageBox::critical(this, tr("Error"), tr("Failed to save video as an AVI file"));
  }
}


void MoviePlayerWindow::saveAsImages(void)
{
  std::cerr << __FUNCTION__ << std::endl;

  Movie *p_movie = m_movie_ptr.get();
  if (!isMovieValid(p_movie))
  {
    WARNM("Invalid movie");
    return;
  }

  QString dir(QFileDialog::getExistingDirectory(this, tr("Directory to save the frame sequence")));
  if (dir.isNull()) return;

  std::cerr << "Selected directory: " << dir.toStdString() << std::endl;

  if (!p_movie->saveAsImageSequence(dir.toStdString() + "/frame"))
  {
    QMessageBox::critical(this, tr("Error"), tr("Failed to save video frame sequence"));
  }
}


void MoviePlayerWindow::prepareUI(void)
{
  Movie *p_movie = m_movie_ptr.get();

  if (isMovieValid(p_movie))
  {
    ui->pbPlayStop->setEnabled(true);
    ui->pbPauseResume->setEnabled(false);
    ui->pbSave->setEnabled(true);
    ui->pbSaveAsImages->setEnabled(true);
    ui->lblFrame->setPixmap(QPixmap::fromImage(p_movie->frame(0)));
  }
  else
  {
    ui->pbPlayStop->setEnabled(false);
    ui->pbPauseResume->setEnabled(false);
    ui->pbSave->setEnabled(false);
    ui->pbSaveAsImages->setEnabled(false);
    ui->lblFrame->setPixmap(QPixmap());
  }
}
