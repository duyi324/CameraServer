#include "mytcpserver.h"

#include <QFile>
#include <QDataStream>


MyTcpServer::MyTcpServer()
{
    server = new QTcpServer();
}

void MyTcpServer::listen() {
    server->listen(QHostAddress::Any, 8888);
    QObject::connect(server,
                     &QTcpServer::newConnection,
                     this,
                     &MyTcpServer::newConnection);
}

void MyTcpServer::newConnection() {
    //
    qDebug() << "new connection...";

    // 如果有新的连接就取出
    while (server->hasPendingConnections())
    {
        // nextPendingConnection返回下一个挂起的连接作为已连接的QTcpSocket对象
        // 套接字是作为服务器的子级创建的，这意味着销毁QTcpServer对象时会自动删除该套接字。
        // 最好在完成处理后显式删除该对象，以避免浪费内存。
        // 返回的QTcpSocket对象不能从另一个线程使用，如有需要可重写incomingConnection().
        QTcpSocket *socket = server->nextPendingConnection();
        clientList.append(socket);


        // 关联相关操作的信号槽
        // 收到数据，触发readyRead
        connect(socket, &QTcpSocket::readyRead, [this, socket]{
            // 没有可读的数据就返回
            if (socket->bytesAvailable() <= 0) return;

            // 注意收发两端文本要使用对应的编解码
            const QString recv_text = QString::fromUtf8(socket->readAll());
            qDebug()<<"receive: "<<recv_text;

            QString reply="got: "+ recv_text;

//            const QByteArray send_data=reply.toUtf8();
//            socket->write(send_data);


            QFile file("/Users/lifayu/Desktop/qt/png360/BA_1.png");
            file.open(QIODevice::ReadOnly);


            QDataStream stream;
            stream << file.readAll();

            socket->write(file.readAll());

        });

        // error信号在5.15换了名字
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)

        // 错误信息
        connect(socket,
                static_cast<void (QAbstractSocket::*)(QAbstractSocket::
                                                      SocketError)>(&
                                                                    QAbstractSocket
                                                                    ::error),
                [this, socket](QAbstractSocket::SocketError){});
#else // if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)

        // 错误信息
        connect(socket, &QAbstractSocket::errorOccurred,
                [this, socket](QAbstractSocket::SocketError){
           );
        });
#endif // if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)

        // 连接断开，销毁socket对象，这是为了开关server时socket正确释放
        connect(socket, &QTcpSocket::disconnected, [this, socket]{
            socket->deleteLater();
            clientList.removeOne(socket);

        });
    }

}

void MyTcpServer::onLiveview(char *data, unsigned long size){
    for(auto i=0;i<clientList.size();i++){
        auto client=clientList.at(i);
        client->write(data,size);
    }
}
