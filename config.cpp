#include "config.h"
#include "ui_config.h"
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QtGui>
#include <QMessageBox>
#include <QSettings>

const cv::Size CommonResolutions[] = {
    cv::Size(220,176),
    cv::Size(160,256),
    cv::Size(208,208),
    cv::Size(256,192),
    cv::Size(280,192),
    cv::Size(432,128),
    cv::Size(240,240),
    cv::Size(320,192),
    cv::Size(320,200),
    cv::Size(256,256),
    cv::Size(320,208),
    cv::Size(320,224),
    cv::Size(320,240),
    cv::Size(320,256),
    cv::Size(376,240),
    cv::Size(272,340),
    cv::Size(400,240),
    cv::Size(320,320),
    cv::Size(432,240),
    cv::Size(560,192),
    cv::Size(400,270),
    cv::Size(480,234),
    cv::Size(400,300),
    cv::Size(480,250),
    cv::Size(312,390),
    cv::Size(640,200),
    cv::Size(480,272),
    cv::Size(512,256),
    cv::Size(416,352),
    cv::Size(480,320),
    cv::Size(640,240),
    cv::Size(640,256),
    cv::Size(512,342),
    cv::Size(800,240),
    cv::Size(512,384),
    cv::Size(640,320),
    cv::Size(640,350),
    cv::Size(640,360),
    cv::Size(480,500),
    cv::Size(720,348),
    cv::Size(720,350),
    cv::Size(640,400),
    cv::Size(720,364),
    cv::Size(800,352),
    cv::Size(600,480),
    cv::Size(640,480),
    cv::Size(640,512),
    cv::Size(768,480),
    cv::Size(800,480),
    cv::Size(848,480),
    cv::Size(854,480),
    cv::Size(800,600),
    cv::Size(960,540),
    cv::Size(832,624),
    cv::Size(960,544),
    cv::Size(1024,576),
    cv::Size(960,640),
    cv::Size(1024,600),
    cv::Size(1024,640),
    cv::Size(960,720),
    cv::Size(1136,640),
    cv::Size(1024,768),
    cv::Size(1024,800),
    cv::Size(1152,720),
    cv::Size(1152,768),
    cv::Size(1280,720),
    cv::Size(1120,832),
    cv::Size(1280,768),
    cv::Size(1152,864),
    cv::Size(1334,750),
    cv::Size(1280,800),
    cv::Size(1152,900),
    cv::Size(1024,1024),
    cv::Size(1366,768),
    cv::Size(1280,854),
    cv::Size(1280,960),
    cv::Size(1600,768),
    cv::Size(1440,900),
    cv::Size(1280,1024),
    cv::Size(1440,960),
    cv::Size(1600,900),
    cv::Size(1400,1050),
    cv::Size(1440,1024),
    cv::Size(1440,1080),
    cv::Size(1600,1024),
    cv::Size(1680,1050),
    cv::Size(1600,1200),
    cv::Size(1920,1080),
    cv::Size(1920,1200),
    cv::Size(2048,1152),
    cv::Size(1792,1344),
    cv::Size(1856,1392),
    cv::Size(1800,1440),
    cv::Size(2880,900),
    cv::Size(2048,1280),
    cv::Size(1920,1400),
    cv::Size(2538,1080),
    cv::Size(1920,1440),
    cv::Size(2160,1440),
    cv::Size(2048,1536),
    cv::Size(2304,1440)
};

//******************Save settings

void config::SaveSettings(QString value, QString key, QString varible)
{
QSettings settings("JoshConwaySoft", "uWho");
settings.beginGroup(value);
settings.setValue(key, varible);
settings.endGroup();
}

//******************Resolution Query per webcam

QVector<int> config::getSupportedVideoResolutions(int camera)
{
    cv::VideoCapture cameraDevice = cv::VideoCapture(camera);
    QVector<int> supportedVideoResolutions;  // This refers to the index of CommonResolutions
    for (int i = 0; i < 100 ; i++){  //There are 101 elements. -1 for index, that leaves 100
        cv::waitKey(100);
        cv::Size test = CommonResolutions[i];
        cameraDevice.set(CV_CAP_PROP_FRAME_WIDTH, test.width);
        cameraDevice.set(CV_CAP_PROP_FRAME_HEIGHT, test.height);
        double width = cameraDevice.get(CV_CAP_PROP_FRAME_WIDTH);
        double height = cameraDevice.get(CV_CAP_PROP_FRAME_HEIGHT);
        if (test.width == width && test.height == height) {
            qDebug() << "PASS" << QString::number(test.width) << QString::number(test.height) ;
            supportedVideoResolutions.append( i );
        }
    }
    return supportedVideoResolutions;
}

//******************Webcam Query

QVector<int> getSupportedWebcams(void)
{
    QVector<int> webcamNumbers ;
    for (int i = 0 ; i < 10 ; i++){
        cv::VideoCapture camera(i) ;
        cv::waitKey(100); // We wait to let the system catch up. Elsewise, V4L errors. :/
        if(camera.isOpened()){
           webcamNumbers.append(i);
           qDebug() << "webcam#" << i ;
        }else{
            return webcamNumbers;
        }
    }
    return webcamNumbers;
}



//******************main

config::config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::config)
{
    ui->setupUi(this);

    QVector<int> webcamIndexList = getSupportedWebcams() ;
    if (webcamIndexList.size() == 0){
        QMessageBox errorBox;
        errorBox.setText("There are no valid cameras on this system. Please plug one in and try again.");
        errorBox.exec();
    }else {
        config::SaveSettings( "Video", "Device", "0" );
        for(int i = 0; i < webcamIndexList.size(); i++ ){
            ui->cameraCombobox->addItem( QString::number(webcamIndexList.at(i)) ); //Populates camera combobox with camera #s
            QVector<int> resolutions = config::getSupportedVideoResolutions(i);
            for(int j = 0; j < resolutions.size(); j++ ){
                QString resolutionString = QString::number(CommonResolutions[resolutions.at(j)].width) + "X" + QString::number(CommonResolutions[resolutions.at(j)].height);
                qDebug() << resolutionString ;
                ui->resolutionCombobox->addItem(resolutionString);
            }
        }
    }
}

//******************Saving logic

void config::on_cameraCombobox_currentIndexChanged(int index)
{
    config::SaveSettings( "Video", "Device", QString::number(index) );
}

void config::on_resolutionCombobox_currentIndexChanged(const QString &arg1)
{
    QString resolution = arg1 ;
    QString resX = resolution.section('X', 0, 0); // Because handling the resolutions directly is so much easier.
    QString resY = resolution.section('X', 1, 1); // Wish I could pass a cv::Size() . No handling needed then.

    config::SaveSettings( "Video", "Resolution_X", resX );
    config::SaveSettings( "Video", "Resolution_Y", resY );
}


//******************config destructor
config::~config()
{
    delete ui;
}
