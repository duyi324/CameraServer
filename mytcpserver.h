#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

/**
 * @brief Tcp服务器，用于实时预览相机画面
 */
class MyTcpServer : public QObject {
    Q_OBJECT

public:

    explicit MyTcpServer();

    void listen();

private:

    /**
     * @brief server 用于监听端口，获取新的tcp连接的描述符
     */
    QTcpServer *server;

    /**
     * @brief clientList 存储已连接的socket对象
     */
    QList<QTcpSocket *>clientList;

private slots:

    void newConnection();

public slots:

    void onLiveview(char         *data,
                    unsigned long size);
};

#endif // MYTCPSERVER_H
