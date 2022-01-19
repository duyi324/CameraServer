#include "cameracontroller.h"


#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtCore>


#include <iostream>


CameraController::CameraController(QObject *parent)
    : HttpRequestHandler{parent}
{}

void CameraController::service(HttpRequest& request, HttpResponse& response)
{
    QByteArray body = request.getBody();

    QJsonParseError jsonError;

    QJsonDocument jsonDoc(QJsonDocument::fromJson(body, &jsonError));

    ResponseInfo response_info;

    if (jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "Json Error: " << jsonError.errorString();

        response_info.returnCode = 201;
        response_info.returnMsg = "Request params wrong.";
        response_info.returnData = "";
        writeResponse(response_info, response);

        return;
    }
    QJsonObject rootObj = jsonDoc.object();
    RequestInfo request_info;


    parseRequest(rootObj, request_info, response_info);
    std::vector<std::string> str;


    str.push_back(request_info.applyId.toStdString());
    str.push_back(request_info.attachId.toStdString());
    str.push_back(request_info.attachUrl.toStdString());
    str.push_back(std::to_string(request_info.pageNo));
    str.push_back(request_info.docPicId.toStdString());
    str.push_back(request_info.docPicUrl.toStdString());
    str.push_back(request_info.preStampPicId.toStdString());
    str.push_back(request_info.preStampPicUrl.toStdString());
    str.push_back(request_info.sealDocPicId.toStdString());
    str.push_back(request_info.sealDocPicUrl.toStdString());
    str.push_back(std::to_string(request_info.markPicFlag));

    str.push_back(request_info.host.toStdString());
    str.push_back(request_info.username.toStdString());
    str.push_back(request_info.passwordHash.toStdString());
    str.push_back(request_info.companyCode.toStdString());
    str.push_back(request_info.paramJson.toStdString());
    str.push_back("false");

    //    std::cout << "Print the received str :" << endl;

    /*for (int ix = 0; ix < str.size(); ix++) {


            std::cout << "ix= " << ix << "  " << str[ix] << std::endl;


       }*/

    if (response_info.returnCode == 200)
    {
        qDebug() << "response_info.returnCode = 200" << endl;

        //        RunServer.InsertServer(RunServer.serverInfo, str);

        // InsertScontext(str);
    }
    else
    {
        qDebug() << "response_info.returnCode " << response_info.returnCode <<
            "    " << "response_info.returnData " << response_info.returnMsg <<
            endl;
    }

    std::vector<std::string>().swap(str);

    //    QByteArray byteArray;
    //    auto content = reinterpret_cast<char *>(&response_info);
    //    auto size = sizeof(response_info);
    //    byteArray.append(content, size);
    writeResponse(response_info, response);
}

void CameraController::writeResponse(ResponseInfo& responseInfo,
                                     HttpResponse& response) {
    QJsonObject resultJsonObj;

    resultJsonObj.insert("code",    responseInfo.returnCode);
    resultJsonObj.insert("data",    responseInfo.returnData);
    resultJsonObj.insert("message", responseInfo.returnMsg);

    auto responseBody =
        QString(QJsonDocument(resultJsonObj).toJson(QJsonDocument::Compact));

    response.setStatus(responseInfo.returnCode);
    response.write(responseBody.toUtf8(), true);
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
