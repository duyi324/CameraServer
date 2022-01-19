#include "cameracontroller.h"


#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtCore>
#include <QString>
#include <QtGlobal>


#include <iostream>

#define MSG_SUCCESS "success"
#define MSG_FAILED  "failed"


CameraController::CameraController(QObject *parent)
    : HttpRequestHandler{parent}
{}

void CameraController::service(HttpRequest& request, HttpResponse& response)
{
    ResponseInfo response_info;

    //   /api/camera/liveview/start

    QString path = request.getPath();
    QStringList pathList = path.split('/', QString::SkipEmptyParts);

    if (pathList.length() < 3) { // 少于3个参数
        response_info.returnCode = 200;
        response_info.returnMsg = MSG_SUCCESS;
        response_info.returnData = "Hello, camera.";
    } else if (pathList.length() == 4) { // 一共4个参数
        QString ability = pathList[2];
        QString param = pathList[3];

        if (ability == "liveview") {
            // 实时预览
            if (param == "start") { // 开始实时预览
                emit startLiveview();
                response_info.returnCode = 200;
                response_info.returnMsg = MSG_SUCCESS;
                response_info.returnData = "";
            }
            else if (param == "stop") // 停止实时预览
            {
                emit stopLiveview();
                response_info.returnCode = 200;
                response_info.returnMsg = MSG_SUCCESS;
                response_info.returnData = "";
            } else { // 未知操作
                response_info.returnCode = 201;
                response_info.returnMsg = MSG_FAILED;
                response_info.returnData = "Undefined.";
            }
        }
        else if (ability == "capture") {
            // 拍照
            if (param == "one") {
                emit captureOne();
                response_info.returnCode = 200;
                response_info.returnMsg = MSG_SUCCESS;
                response_info.returnData = "";
            }
        }
        else {
            response_info.returnCode = 201;
            response_info.returnMsg = MSG_SUCCESS;
            response_info.returnData = "Undefined.";
        }
    } else
    {
        //
    }


    writeResponse(response_info, response, true);


    //    QJsonParseError jsonError;
    //    QJsonDocument jsonDoc(QJsonDocument::fromJson(body, &jsonError));

    //    if (jsonError.error != QJsonParseError::NoError)
    //    {
    //        qDebug() << "Json Error: " << jsonError.errorString();

    //        response_info.returnCode = 201;
    //        response_info.returnMsg = "Request params wrong.";
    //        response_info.returnData = "";
    //        writeResponse(response_info, response);

    //        return;
    //    }
    //    QJsonObject rootObj = jsonDoc.object();
    //    RequestInfo request_info;


    //    parseRequest(rootObj, request_info, response_info);
    //    std::vector<std::string> str;


    //    str.push_back(request_info.applyId.toStdString());
    //    str.push_back(request_info.attachId.toStdString());
    //    str.push_back(request_info.attachUrl.toStdString());
    //    str.push_back(std::to_string(request_info.pageNo));
    //    str.push_back(request_info.docPicId.toStdString());
    //    str.push_back(request_info.docPicUrl.toStdString());
    //    str.push_back(request_info.preStampPicId.toStdString());
    //    str.push_back(request_info.preStampPicUrl.toStdString());
    //    str.push_back(request_info.sealDocPicId.toStdString());
    //    str.push_back(request_info.sealDocPicUrl.toStdString());
    //    str.push_back(std::to_string(request_info.markPicFlag));

    //    str.push_back(request_info.host.toStdString());
    //    str.push_back(request_info.username.toStdString());
    //    str.push_back(request_info.passwordHash.toStdString());
    //    str.push_back(request_info.companyCode.toStdString());
    //    str.push_back(request_info.paramJson.toStdString());
    //    str.push_back("false");


    //    if (response_info.returnCode == 200)
    //    {
    //        qDebug() << "response_info.returnCode = 200" << endl;
    //    }
    //    else
    //    {
    //        qDebug() << "response_info.returnCode " <<
    // response_info.returnCode <<
    //            "    " << "response_info.returnData " <<
    // response_info.returnMsg <<
    //            endl;
    //    }

    //    std::vector<std::string>().swap(str);


    //    writeResponse(response_info, response);
}

void CameraController::writeResponse(ResponseInfo& responseInfo,
                                     HttpResponse& response,
                                     bool          lastPart) {
    QJsonObject resultJsonObj;

    resultJsonObj.insert("code",    responseInfo.returnCode);
    resultJsonObj.insert("data",    responseInfo.returnData);
    resultJsonObj.insert("message", responseInfo.returnMsg);

    auto responseBody =
        QString(QJsonDocument(resultJsonObj).toJson(QJsonDocument::Compact));

    response.setStatus(responseInfo.returnCode);
    response.write(responseBody.toUtf8(), lastPart);
}

void CameraController::parseRequest(const QJsonObject& rootObj,
                                    RequestInfo      & info,
                                    ResponseInfo     & response_info)
{
    response_info.returnCode = 200;
    response_info.returnData = "";
    response_info.returnMsg = "OK";

    QStringList keys = rootObj.keys();

    for (int i = 0; i < keys.size(); i++)
    {
        QString qStr = keys.at(i);
        qDebug() << "qStr :" << qStr << endl;

        for (int idx = 0; idx < info.Info_Size; idx++) {
            if (qStr == strRequestInfo[idx])
            {
                QJsonValue ivalue = rootObj.value(qStr);
                bool bExist = false;
                qDebug() << strRequestInfo[idx] << "    ivalue" << ivalue << endl;

                switch (idx) {
                case 0:
                    bExist = true;
                    info.applyId = ivalue.toString();
                    break;

                case 1:
                    bExist = true;
                    info.attachId = ivalue.toString();
                    break;

                case 2:
                    bExist = true;
                    info.attachUrl = ivalue.toString();
                    break;

                case 3:
                    bExist = true;
                    info.pageNo = ivalue.toInt();
                    break;

                case 4:
                    bExist = true;
                    info.docPicId = ivalue.toString();
                    break;

                case 5:
                    bExist = true;
                    info.docPicUrl = ivalue.toString();
                    break;

                case 6:
                    bExist = true;
                    qDebug() << "info.preStampPicId" << info.preStampPicId <<
                        endl;
                    info.preStampPicId = ivalue.toString();
                    break;

                case 7:
                    bExist = true;
                    info.preStampPicUrl = ivalue.toString();
                    break;

                case 8:
                    bExist = true;
                    info.sealDocPicId = ivalue.toString();
                    break;

                case 9:
                    bExist = true;
                    info.sealDocPicUrl = ivalue.toString();
                    break;

                case 10:
                    bExist = true;
                    info.markPicFlag = ivalue.toInt();
                    break;

                case 11:
                    bExist = true;
                    info.host = ivalue.toInt();
                    break;

                case 12:
                    bExist = true;
                    info.username = ivalue.toInt();
                    break;

                case 13:
                    bExist = true;
                    info.passwordHash = ivalue.toInt();
                    break;

                case 14:
                    bExist = true;
                    info.companyCode = ivalue.toInt();
                    break;

                case 15:
                    bExist = true;
                    info.paramJson = ivalue.toInt();
                    break;
                }

                if (bExist) break;
            }
        }
    }


    qDebug() << "applyId :" << info.applyId << endl << " info.preStampPicId :" <<
        info.preStampPicId << endl;


    if (info.applyId.isEmpty() || info.preStampPicId.isEmpty()) {
        qDebug() <<
            "***************************************************************" <<
            endl;
        response_info.returnCode = 301;
        response_info.returnMsg = "";

        if (info.applyId.isEmpty()) response_info.returnMsg += "applyId is Null ";

        if (info.preStampPicId.isEmpty())
        {
            std::cout << "Run this img" << std::endl;
            std::cout.flush();
            response_info.returnMsg += "preStampPicId is Null ";
        }
    }
}
