#ifndef CAMERAMAINLOOPER_H
#define CAMERAMAINLOOPER_H

#include <QThread>
#include <QObject>

#include <QByteArray>
#include <QDataStream>
#include <QBuffer>
#include <QString>


#include <iostream>
#include <fstream>

#include <gphoto2pp/helper_gphoto2.hpp>
#include <gphoto2pp/camera_list_wrapper.hpp>
#include <gphoto2pp/exceptions.hpp>

#include <gphoto2pp/camera_wrapper.hpp>

#include <gphoto2pp/exceptions.hpp>

#include <gphoto2pp/helper_gphoto2.hpp>
#include <gphoto2pp/helper_context.hpp>
#include <gphoto2pp/window_widget.hpp>
#include <gphoto2pp/camera_abilities_list_wrapper.hpp>
#include <gphoto2pp/camera_list_wrapper.hpp>
#include <gphoto2pp/gp_port_info_list_wrapper.hpp>

#include <gphoto2pp/camera_file_wrapper.hpp>
#include <gphoto2pp/camera_file_path_wrapper.hpp>
#include <gphoto2pp/camera_event_type_wrapper.hpp>
#include <gphoto2pp/camera_capture_type_wrapper.hpp>

#include <gphoto2pp/log.h>

#include <gphoto2pp/camera_wrapper.hpp>        // Header for CameraWrapper
#include <gphoto2pp/camera_file_wrapper.hpp>   // Header for CameraFileWrapper
#include <gphoto2pp/helper_camera_wrapper.hpp> // Used for helper::capture(...)
                                               // method

/**
 * @brief 主循环，检查相机连接等
 */
class CameraMainLooper : public QThread {
    Q_OBJECT

public:

    explicit CameraMainLooper();
    ~CameraMainLooper();

protected:

    // QThread的虚函数
    // 线程处理函数
    // 不能直接调用，通过start()间接调用
    void run();

private:

    bool isRunning;

    /**
     * @brief isLiveview 实时预览
     */
    bool isLiveview;

    //    gphoto2pp::CameraWrapper mCamera;

public slots:

    void startLiveview();
    void stopLiveview();
    void captureOne();

signals:

    void onLiveview(char         *data,
                    unsigned long size);
};

#endif // CAMERAMAINLOOPER_H
