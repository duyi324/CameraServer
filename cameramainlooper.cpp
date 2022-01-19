#include "cameramainlooper.h"


#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QImage>


CameraMainLooper::CameraMainLooper()
    : QThread()
    , isRunning(false)
    , isLiveview(true)
{
    //
}

CameraMainLooper::~CameraMainLooper() {
    isRunning = false;
}

void GetAllFiles(QString dirPath, QVector<QString>& files)

{
    QDir dir(dirPath);

    dir.setFilter(QDir::Files);

    foreach(QFileInfo fi, dir.entryInfoList())
    {
        qDebug() << fi.absoluteFilePath();
        files << fi.absoluteFilePath();
    }
}

void CameraMainLooper::run() {
    isRunning = true;


    //    QDir dir("G:\\bad_apple\\png360");

    QDir dir("/Users/lifayu/Desktop/qt/png360");

    dir.setFilter(QDir::Files);


    while (isRunning) {
        QThread::sleep(1);

        //        qDebug() << "test";

        //        if (isLiveview) {
        //            //
        //        }

        QT_TRY
        {
            auto cameraPair = gphoto2pp::autoDetectAll();
            std::cout << "camera count:" << cameraPair.count() << std::endl;
            auto name = cameraPair.getName(0);
            auto val = cameraPair.getValue(0);


            auto camera =
                gphoto2pp::CameraWrapper(name, val);

            //                gphoto2pp::CameraWrapper mCamera;

            while (true) {
                QT_TRY{
                    // QThread::sleep(1);
                    // camera.getSummary();
                    if (isLiveview) {
                        gphoto2pp::CameraFileWrapper preview =
                            camera.capturePreview();
                        std::vector<char>ds = preview.getDataAndSize();
                        unsigned long size = ds.size();
                        char *data = ds.data();

                        qDebug() << size;
                        qDebug() << data;
                        emit onLiveview(data, size);

                        //  std::fstream f1("",
                        //                  std::fstream::out |
                        //                  std::fstream::binary |
                        //                  std::fstream::trunc);

                        //  gphoto2pp::helper::capturePreview(camera, f1);
                        //  std::cout << f1.tellg();

                        //
                    }
                }
                QT_CATCH(...) {
                    break;
                }
            }
        }
        QT_CATCH(gphoto2pp::exceptions::NoCameraFoundError& e)
        {
            std::cout <<
                "GPhoto couldn't detect any cameras connected to the computer"
                      <<
                std::endl;
            std::cout << "Exception Message: " << e.what() << std::endl;
        }
        QT_CATCH(...) {
            qDebug() << "error...";
        }

        //            gphoto2pp::CameraListWrapper cameraList =
        // gphoto2pp::autoDetectAll();

        //            std::cout << mCamera.getSummary() << std::endl;
        //            qDebug() << cameraList.count();

        //            // 发送测试数据
        //            foreach(QFileInfo fi, dir.entryInfoList())
        //            {
        //                qDebug() << fi.absoluteFilePath();
        //                QImage image(fi.absoluteFilePath());

        //                QBuffer buffer;

        //                buffer.open(QIODevice::ReadWrite);
        //                image.save(&buffer, "jpg");

        //                quint32 len = buffer.data().size();

        //                qDebug() << "image size: " << len;


        //                //                image.save(&buffer, "jpg");
        //                //                imageData =
        // imageData.toBase64();
        //            }

        //            auto  fileList = dir.entryInfoList();
        //            auto  fi0 =  fileList.at(0);
        //            QFile file(fi0.absoluteFilePath());
        //            file.open(QIODevice::ReadOnly);


        //            QDataStream stream;
        //            stream << file.readAll();
    }
}

void CameraMainLooper::startLiveview() {
    isLiveview = true;
    qDebug() << ">>>>>>　start Liveview　in looper";
}

void CameraMainLooper::stopLiveview() {
    isLiveview = false;
    qDebug() << ">>>>>>　stop Liveview　in looper";
}

void CameraMainLooper::captureOne() {
    qDebug() << ">>>>>>　Capture one in looper";
}
