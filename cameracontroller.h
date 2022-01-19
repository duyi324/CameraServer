#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <httprequesthandler.h>
#include <QObject>


using namespace stefanfrings;

typedef struct RequestInfo {
    QString applyId;
    QString attachId;
    QString attachUrl;
    int     pageNo;
    QString docPicId;
    QString docPicUrl;
    QString preStampPicId;
    QString preStampPicUrl;
    QString sealDocPicId;
    QString sealDocPicUrl;
    int     markPicFlag = 0;

    QString host;
    QString username;
    QString passwordHash;
    QString companyCode;
    QString paramJson;

    const int Info_Size = 16;
} RequestInfo;

/**
 * @brief Web服务器中相机相关的接口处理类
 */
class CameraController : public HttpRequestHandler {
    Q_OBJECT

private:

    struct ResponseInfo
    {
        int     returnCode;
        QString returnData;
        QString returnMsg;
    };

    QString strRequestInfo[16] =
    {
        "applyId",
        "attachId",
        "attachUrl",
        "pageNo",
        "docPicId",
        "docPicUrl",
        "preStampPicId",
        "preStampPicUrl",
        "sealDocPicId",
        "sealDocPicUrl",
        "markPicFlag",
        "host",
        "username",
        "passwordHash",
        "companyCode",
        "paramJson"
    };

    void writeResponse(ResponseInfo& responseInfo,
                       HttpResponse& response,
                       bool          lastPart = false);

    void parse_json(QJsonObject  obj,
                    RequestInfo  request_info,
                    ResponseInfo response_info);

public:

    explicit CameraController(QObject *parent = nullptr);

    void service(HttpRequest & request,
                 HttpResponse& response);
    void parseRequest(const QJsonObject& rootObj,
                      RequestInfo      & info,
                      ResponseInfo     & response_info);

signals:

    void startLiveview();
    void stopLiveview();
    void captureOne();
};

#endif // CAMERACONTROLLER_H
