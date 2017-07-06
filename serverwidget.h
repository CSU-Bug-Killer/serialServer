#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QByteArray>

class ServerWidget : public QWidget
{
    Q_OBJECT
public:

    static ServerWidget*  getInstance()
    {
        static  ServerWidget*  instance = NULL;
        if(NULL == instance)
        {
            instance = new ServerWidget;
        }
        return instance;
    }

signals:
    void readMessageSignal(QByteArray);

public slots:
    void startTcpserver();
    void sendMessage();
    void newConnect();
    void readMessage();
private:

    ServerWidget(QWidget *parent = 0);

    QTcpServer *m_tcpServer;
//    QTcpSocket *m_tcpSocket;
    QList<QTcpSocket*> m_tcpSockets;
};

#endif // SERVERWIDGET_H
