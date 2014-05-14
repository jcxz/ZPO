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
    void on_pushButton_clicked();

    void loadSourceImage(void);
    void loadDestinationImage(void);

  private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
