#include "serverwidget.h"
#include <QDebug>
#include <QTcpServer>
#include <QJsonParseError>

#include "handleclientsocketthread.h"
#include "deviceinfo.h"

ServerWidget::ServerWidget(QWidget *parent) : QWidget(parent)
{
    this->startTcpserver();
}

void ServerWidget::startTcpserver()
{
    qDebug() << "Start Server.";
    if(!m_tcpServer){
        qDebug() << "sss";
    }
        //如果m_tcpServer为空则不进行服务器初始化操作
//        if(!m_tcpServer){
            qDebug() << "new server";
        m_tcpServer = new QTcpServer(this);
        m_tcpServer->listen(QHostAddress::Any,9527); //监听任何连上19999端口的ip
        connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnect())); //新连接信号触发，调用newConnect()槽函数，这个跟信号函数一样。
//        }


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

    HandleClientSocketThread *socThread = new HandleClientSocketThread(m_tcpSocket);

    socThread->start();

    connect(socThread,SIGNAL(readMessageSignal(QString)),
            this,SLOT(tSlot(QString)));
    connect(socThread, SIGNAL(finished()), socThread, SLOT(deleteLater()));
    connect(socThread, SIGNAL(finished()), this, SLOT(fSlot()));


        //第二个this换成线程
//        connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage())); //有可读的信息，触发读函数槽

}

void ServerWidget::fSlot(){
    qDebug() << "finish    fSlot";

}

void ServerWidget::tSlot(QString ss){
    qDebug() << "tSlot";
    //socket的信号转成server的发出去
    emit readMessageSignal(ss);
}

void ServerWidget::readMessage()
{
    //逻辑复杂就要加线程处理了
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
    qDebug()<<message << "readMessage";
    parseMsgFromClient(message);


    //

//    emit readMessageSignal(message);//要控制串口就去掉注释

//    QString ss=QVariant(message).toString();
//    ui->textEdit_rec->setText(ss);

}

void ServerWidget::parseMsgFromClient(QString json)
{
    qDebug() << "parseMsg";
//        QString json("{"
//                     "\"message\":\"行进方向指令串\","
//                     "\"data\":{"
//                     "\"sendType\":\"direction\","
//                     "\"sendData\":{"
//                     "\"device\":\"car1\","
//                     "\"directionData\":\"QRLRLLRS\""
//                     "}"
//                     "}"
//                     "}"
//                    );
        QJsonParseError error;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8(),&error);
        if(error.error == QJsonParseError::NoError){
            if(jsonDocument.isObject()){
                QVariantMap result = jsonDocument.toVariant().toMap();
                qDebug() << "message:" << result["message"].toString();
                QVariantMap data = result["data"].toMap();
                qDebug() << "sendType:" << data["sendType"].toString();
                QString sendType = data["sendType"].toString();

                if("direction" == sendType){
                    qDebug() << "parse direction";
                }else if("carLights" == sendType){
                    qDebug() << "parse carLights";

                }else if("speakers" == sendType){
                    qDebug() << "parse speakers";
                }else if("roadLight" == sendType){
                    qDebug() << "parse roadLight";

                    QVariantMap sendData = data["sendData"].toMap();

                    qDebug() << sendData["device"].toString();
                    qDebug() << sendData["status1"].toString();
                    qDebug() << sendData["status2"].toString();

                    QString device = sendData["device"].toString();
                    QString status1 =sendData["status1"].toString();
                    QString status2 = sendData["status2"].toString();

                    controlRoadLight(device,status1,status2);


//                    controlRoadLight(device);


                }else{
                    qDebug() << "unknowed sendType";
                }

//                QVariantMap sendData = data["sendData"].toMap();
//                qDebug() << "device:" << sendData["device"].toString();
//                qDebug() << "directionData:" << sendData["directionData"];
            }
        }else{
            qDebug() << "illegal value";
//            qFatal(error.errorString().toUtf8().constData());
//            exit(1);
        }
}

void ServerWidget::controlDirection(QString device,QString direction)
{
    /*
     * 命令格式
     * 0x00 0x02 0x00 0x$1 0x$2 0x$3 0xff
     * $1 $2 为小车短地址
     * $3设备控制
     * 0x01 启动
     * 0x00 停车
     * 0x02 下个路口左转
     * 0x03 下个路口右转
     * 0x04 下个路口直行
     */
    //
    //这里只是处理单个指令，比如单个"R"或"L"
    //约定 启动:"D" 停车:"S" 直行："H" 左转："L" 右转："R"
    //还需要另外维护一个队列或栈

    QString car = "";//小车短地址会不会经常变,小车对应地址的存储。。。

    QString command = "";
    //添加头
    command.append("000200");
    //拼接小车短地址
    command.append(car);

    QString datafield = "";//数据域

    if("D" == direction){
        datafield.append("01");
    }
    if("S" == direction){
        datafield.append("00");
    }
    if("H" == direction){
        datafield.append("02");
    }
    if("L" == direction){
        datafield.append("03");
    }
    if("R" == direction){
        datafield.append("04");
    }

    //拼接数据域
    command.append(datafield);
    //结束位
    command.append("ff");
    emit readMessageSignal(command.toStdString().c_str());

}

void ServerWidget::controlCarLight(QString device,QString headLeft,QString headRight,QString roofLigntRed,QString rootLightBlue)
{
    /*车灯控制
     * 0x00 0x04 0x00 0x$1 0x$2 0x$3 0xff
     *0位：左大灯；1位：右大灯；2位：车顶蓝；位3：车顶红；置1为开，置0为关，
     * 例如0x0F，即0000  1111代表灯全开
     */

    QString car = "";//小车短地址会不会经常变

    QString command = "";
    //添加头
    command.append("000400");
    //拼接小车短地址
    command.append(car);

    QString datafield = "0";//数据域
    QString temp = headLeft+headRight+roofLigntRed+rootLightBlue;

    bool ok = true;

    qDebug() << QString::number(temp.toInt(&ok,2),16); //str="3f";
    datafield.append(QString::number(temp.toInt(&ok,2),16));

    //拼接数据域
    command.append(datafield);
    //结束位
    command.append("ff");

    emit readMessageSignal(command.toStdString().c_str());
}

void ServerWidget::controlSpeakers(QString device,QString control)
{
    /*蜂鸣器控制
     * 0x00 0x05 0x00 0x$1 0x$2 0x$3 0xff
     *第0位置1为开启蜂鸣器，置0为关闭蜂鸣器
     *
     */

    QString car = "";//小车短地址会不会经常变经常变就不知道是哪辆车了（虽然现在只有一辆）

    QString command = "";
    //添加头
    command.append("000500");
    //拼接小车短地址
    command.append(car);

    QString datafield = "0";//数据域
    datafield.append(control);

    //拼接数据域
    command.append(datafield);
    //结束位
    command.append("ff");

    emit readMessageSignal(command.toStdString().c_str());

}

void ServerWidget::controlRoadLight(QString device,QString statusRoadLight1,QString statusRoadLight2)
{
    /*红绿灯控制
     *0x00 0x06 0x00 0x$1 0x$2 0x$3 0xff
     *数据域XXXX  XXXX对应XX蓝绿  红蓝绿红，位0-2代表第一组红绿灯，
     *位3-5代表第二组，置1代表开启，置0位关闭例如0x09,即为0000 1001，两组灯为红灯
     */

    qDebug() <<"controlRoadLight";
    QString car = DeviceInfo::lightHashset.value("light1");//小车短地址会不会经常变经常变就不知道是哪辆车了（虽然现在只有一辆）
//    car = DeviceInfo::carHashset;
    qDebug() << "device : " << car << " %%%%%%%%%%%%%%%%%";

    QString command = "";
    //添加头
    command.append("000600");
    //拼接小车短地址
    command.append(car);

    QString datafield = "0000";//数据域
    if("red"==statusRoadLight1){
        datafield.append("01");
    }else{
        datafield.append("10");
    }
    if("red"==statusRoadLight2){
        datafield.append("01");
    }else{
        datafield.append("10");
    }
    //拼接数据域
    qDebug() << datafield;

    bool ok = true;
    int cdata = datafield.toInt(&ok,2);
    datafield = QString::number(cdata, 16);
//    int cda = cdata.toInt();
//    QString::to

    qDebug() << datafield;
    if(datafield.size()==1){
        datafield = "0"+datafield;
    }
    command.append(datafield);
    //结束位
    command.append("ff");

    qDebug() << command.toStdString().c_str() << "   $ controlRoadLight";

    emit readMessageSignal(command);
}






