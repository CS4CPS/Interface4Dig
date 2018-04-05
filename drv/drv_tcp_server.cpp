#include "drv_tcp_server.h"

drv_tcp_server::drv_tcp_server()
{
    tcpServer = new QTcpServer(this);
}

drv_tcp_server::~drv_tcp_server()
{
    delete tcpServer;
}

int32 drv_tcp_server::init(QHostAddress ipv4, uint32 port)
{
    int32 res;

    if(!tcpServer->listen(ipv4,port))
    {
        tcpServer->close();
        res = -1;
    }else
    {
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(acceptcon()));
        client_connected = 0;
        res = 0;
    }

    return res;   
}

QHostAddress drv_tcp_server::getIP(uint32 ipv4)
{
    QHostAddress iptmp;
    iptmp.setAddress((quint32)ipv4);
    return iptmp;
}

QHostAddress drv_tcp_server::getIP(QString ipv4)
{
    QHostAddress iptmp;
    iptmp.setAddress(ipv4);
    return iptmp;
}

int32 drv_tcp_server::send(uint32 addr, uint8 *buf, uint32 len)
{
    int32 res;
    QByteArray tmp;

    if(client_connected>0)
    {
        tmp.append((char*)buf,len);
        clientCon->write(tmp.data(),tmp.length());
        res = len;
    }else
        res = 0;

    return res;
}

int32 drv_tcp_server::rece(uint32 addr, uint8 *buf, uint32 len)
{
    int32 res;

    res = dev_loopbuf.drv_read((char*)buf, len);

    return res;
}

void drv_tcp_server::acceptcon()
{
    clientCon = tcpServer->nextPendingConnection();
    connect(clientCon,SIGNAL(readyRead()),this,SLOT(readcon()));

    client_connected++;
}

void drv_tcp_server::readcon()
{
    char recebuf[512];
    int res = 0;

    if(clientCon->bytesAvailable()!=0)
        res = clientCon->read((char*)&recebuf,512);

    dev_loopbuf.drv_add(recebuf,res);

    emit ReceReady();
}


