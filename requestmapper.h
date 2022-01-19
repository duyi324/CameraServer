#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include <httprequesthandler.h>
#include <QObject>


#include "cameracontroller.h"

/**
 * @brief Http请求映射器
 */
class RequestMapper : public HttpRequestHandler {
    Q_OBJECT

public:

    explicit RequestMapper(QObject *parent = nullptr);


    void service(HttpRequest & request,
                 HttpResponse& response);

private:

    CameraController *cameraController;

public:

    CameraController* getCameraController() {
        return cameraController;
    }
};

#endif // REQUESTMAPPER_H
