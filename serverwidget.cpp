#include "serverwidget.h"
#include <QDebug>
#include <QTcpServer>

ServerWidget::ServerWidget(QWidget *parent) : QWidget(parent)
{
    this->startTcpserver();
}

void ServerWidget::startTcpserver()
{
    qDebug() << "Start Server.";
        //如果m_tcpServer为空则不进行服务器初始化操作
        if(!m_tcpServer){
        m_tcpServer = new QTcpServer(this);
        m_tcpServer->listen(QHostAddress::Any,19999); //监听任何连上19999端口的ip
        connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnect())); //新连接信号触发，调用newConnect()槽函数，这个跟信号函数一样。
        }


}

void ServerWidget::sendMessage()
{
    qDebug() << "send message";
//        QString strMesg= ui->lineEdit_sendmessage->text();
//        qDebug()<<strMesg;
//        for (int i = 0; i < m_tcpSockets.size(); ++i) {
//            m_tcpSockets.at(i)->write(strMesg.toStdString().c_str(),strlen(strMesg.toStdString().c_str())); //发送
//         }

}

void ServerWidget::newConnect()
{
    qDebug() << "new connect";
        QTcpSocket *m_tcpSocket = m_tcpServer->nextPendingConnection(); //得到每个连进来的socket
        m_tcpSockets.append(m_tcpSocket);
        connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage())); //有可读的信息，触发读函数槽

}

void ServerWidget::readMessage()
{
    QByteArray  message;
        for(int i = 0;i < m_tcpSockets.length();i ++)
            {
                qDebug() << "QDataStream " ;
                message = m_tcpSockets.at(i)->readAll();
                if(!(message.isEmpty()))
                {
                    qDebug() << "break " ;
                    break;
                }
            }
        qDebug()<<message;

        emit readMessageSignal(message);
//        QString ss=QVariant(message).toString();
    //    ui->textEdit_rec->setText(ss);

}




