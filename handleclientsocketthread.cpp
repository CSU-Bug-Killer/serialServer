#include "handleclientsocketthread.h"

#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include "deviceinfo.h"

HandleClientSocketThread::HandleClientSocketThread(QTcpSocket *soc)
{
    socket = soc;
//    serialPort = sPort;

    qDebug() << "new socketTherad";

}

void HandleClientSocketThread::run()
{

    connect(socket,SIGNAL(readyRead()),this,SLOT(handleSlot()),Qt::DirectConnection);//线程内传递消息
    connect(socket,SIGNAL(disconnected()),this,SLOT(quit()));//socket断开连接，线程退出
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(tcpSocket_error_slot()));

    exec();//加了这句就不会立即结束，线程结束连接函数也失效
}

void HandleClientSocketThread::handleSlot()
{
    QByteArray  message;
    message = socket->readAll();

    qDebug() << message << " $ Thread output";
    parseMsgFromClient(message);

}

void HandleClientSocketThread::tcpSocket_error_slot()
{
    qDebug() << socket->errorString();
}

void HandleClientSocketThread::parseMsgFromClient(QString json)
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

        }
}

void HandleClientSocketThread::controlDirection(QString device,QString direction)
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

    emit readMessageSignal(command);

}

void HandleClientSocketThread::controlCarLight(QString device,QString headLeft,QString headRight,QString roofLigntRed,QString rootLightBlue)
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

    emit readMessageSignal(command);
}

void HandleClientSocketThread::controlSpeakers(QString device,QString control)
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

    emit readMessageSignal(command);

}

void HandleClientSocketThread::controlRoadLight(QString device,QString statusRoadLight1,QString statusRoadLight2)
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


