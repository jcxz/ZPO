#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui { class MainWindow; }


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow(void);
    
  private slots:
    void morph(void);
    void loadProject(void);
    void saveProject(void);
    void loadSourceImage(void);
    void loadDestinationImage(void);
    void scaleImages(void);

  private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
