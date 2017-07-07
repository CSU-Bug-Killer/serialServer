#include "handleclientsocketthread.h"

#include <QDebug>

HandleClientSocketThread::HandleClientSocketThread(QTcpSocket *soc,QSerialPort *sPort)
{
    socket = soc;
    serialPort = sPort;


    qDebug() << "new socketTherad";

}

void HandleClientSocketThread::run()
{
    connect(soc,SIGNAL(readyRead()),
            this,SLOT(handleSlot()));


}

void HandleClientSocketThread::handleSlot()
{
    QByteArray  message;
    message = socket->readAll();
    qDebug() << message << " $ Thread output";
}

