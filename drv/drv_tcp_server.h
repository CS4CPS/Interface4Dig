#ifndef DRV_TCP_SERVER_H
#define DRV_TCP_SERVER_H

#include "drv/drv_base.h"

#include <QtNetwork>

#include "drv_LoopBuf.h"

#define     DRV_TCP_SERVER_CONNECT_MAX               1

class drv_tcp_server:public QObject,public drv_base
{
    Q_OBJECT

public:
    drv_tcp_server();
    ~drv_tcp_server();

    int32 init(QHostAddress ipv4, uint32 port);
    int32 send(uint32 addr, uint8 *buf, uint32 len);
    int32 rece(uint32 addr, uint8 *buf, uint32 len);

    QHostAddress getIP(uint32 ipv4);
    QHostAddress getIP(QString ipv4);

private:
    QTcpServer *tcpServer;
    QTcpSocket *clientCon;

    int client_connected;
    drv_LoopBuf dev_loopbuf;
    QByteArray sendbuf;

signals:
    void ReceReady(void);

public slots:
    void acceptcon();
    void readcon();
};

#endif // DRV_TCP_SERVER_H
