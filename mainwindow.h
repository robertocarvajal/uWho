#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <opencv2/objdetect/objdetect.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_videofileButton_clicked();

    void on_webcamButton_clicked();

    void on_dirpicButton_clicked();

    void on_ipButton_clicked();

    void on_configureButton_clicked();

private:
    Ui::MainWindow *ui;
    void SaveSettings(QString, QString);
};

#endif // MAINWINDOW_H
