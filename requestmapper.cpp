#include "requestmapper.h"

RequestMapper::RequestMapper(QObject *parent)
    : HttpRequestHandler{parent}
    , cameraController(new CameraController())
{}

void RequestMapper::service(HttpRequest & request,
                            HttpResponse& response) {
    QByteArray path = request.getPath();

    qDebug("RequestMapper: path=%s", path.data());

    if ((path == "/") || (path == "/api")) {
        response.setStatus(200, "Ok");
        response.write("Hello, camera.", true);
    }
    else if (path.startsWith("/api/camera")) {
        cameraController->service(request, response);
    }
    else {
        response.setStatus(404, "Not found");
        response.write("The URL is wrong, no such document.", true);
    }
    qDebug("RequestMapper: finished request");
}
