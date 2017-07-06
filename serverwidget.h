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

    void controlDirection(QString device,QString direction);
    void controlCarLight(QString device,QString headLeft,QString headRight,QStirng roofLigntRed,QString rootLightBlue);
    void controlSpeakers(QString device,QString control);
    void controlRoadLight(QString device,QString statusRoadLight1,QString statusRoadLight2);

    ServerWidget(QWidget *parent = 0);

    QTcpServer *m_tcpServer;
//    QTcpSocket *m_tcpSocket;
    QList<QTcpSocket*> m_tcpSockets;

    void parseMsg(QString json);
};

#endif // SERVERWIDGET_H
