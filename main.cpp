#include <QCoreApplication>


#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QString>


#include <httplistener.h>
#include <httprequesthandler.h>


#include "cameracontroller.h"
#include "mytcpserver.h"
#include "cameramainlooper.h"
#include "requestmapper.h"


/**
 * @brief 搜索配置文件
 * @return 返回配置文件的完整路径
 */
QString searchConfigFile() {
    QString binDir = QCoreApplication::applicationDirPath();
    QString appName = QCoreApplication::applicationName();
    QFile   file;

    file.setFileName(binDir + "/webapp.ini");

    if (!file.exists()) {
        file.setFileName(binDir + "/etc/webapp.ini");

        if (!file.exists()) {
            file.setFileName(binDir + "/../etc/webapp.ini");

            if (!file.exists()) {
                file.setFileName(binDir + "/../" + appName + "/etc/webapp.ini");

                if (!file.exists()) {
                    file.setFileName(
                        binDir + "/../../" + appName + "/etc/webapp.ini");

                    if (!file.exists()) {
                        file.setFileName(
                            binDir + "/../../../../../" + appName +
                            "/etc/webapp.ini");

                        if (!file.exists()) {
                            file.setFileName(QDir::rootPath() + "etc/webapp.ini");
                        }
                    }
                }
            }
        }
    }

    if (file.exists()) {
        QString configFileName = QDir(file.fileName()).canonicalPath(); // 将相对路径换成绝对路径
        qDebug("using config file %s", qPrintable(configFileName));
        return configFileName;
    }

    qFatal("config file not found"); // 找不到该文件，则应用程序将输出一条错误消息，并中止程序
    return "";
}

// void GetAllFiles(QString dirPath, QVector<QString>& files)

// {
//    QDir dir(dirPath);

//    dir.setFilter(QDir::Files);

//    foreach(QFileInfo fi, dir.entryInfoList())
//    {
//        qDebug() << fi.absoluteFilePath();
//        files << fi.absoluteFilePath();
//    }
// }

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QVector<QString> files;

    //    GetAllFiles("G:\\bad_apple\\png360", files);

    MyTcpServer *tcpServer = new MyTcpServer();

    tcpServer->listen();


    CameraMainLooper *mainLooper = new CameraMainLooper();

    mainLooper->start();


    // Load the configuration file
    QString configFileName = searchConfigFile();
    QSettings *listenerSettings = new QSettings(configFileName,
                                                QSettings::IniFormat,
                                                &app);


    // Start the HTTP server
    listenerSettings->beginGroup("listener");

    auto mapper = new RequestMapper(&app);

    new stefanfrings::HttpListener(
        listenerSettings,
        mapper,
        &app);

    qDebug() << "server started...";

    QObject::connect(mapper->getCameraController(),
                     &CameraController::startLiveview,
                     mainLooper,
                     &CameraMainLooper::startLiveview);
    QObject::connect(mapper->getCameraController(),
                     &CameraController::stopLiveview,
                     mainLooper,
                     &CameraMainLooper::stopLiveview);
    QObject::connect(mapper->getCameraController(),
                     &CameraController::captureOne,
                     mainLooper,
                     &CameraMainLooper::captureOne);


    QObject::connect(mainLooper,
                     &CameraMainLooper::onLiveview,
                     tcpServer,
                     &MyTcpServer::onLiveview);
    return app.exec();
}
