#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

namespace Ui {
class config;
}

class config : public QDialog
{
    Q_OBJECT

public:
    explicit config(QWidget *parent = 0);
    ~config();

private slots:
    void on_cameraCombobox_currentIndexChanged(int index);


    void on_resolutionCombobox_currentIndexChanged(const QString &arg1);

private:
    Ui::config *ui;
    QVector<int> getSupportedVideoResolutions(int);
    void SaveSettings(QString, QString, QString);
};

#endif // CONFIG_H
