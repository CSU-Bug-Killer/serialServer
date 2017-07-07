#ifndef HANDLECLIENTSOCKETTHREAD_H
#define HANDLECLIENTSOCKETTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QSerialPort>


class HandleClientSocketThread : public QThread

{
    Q_OBJECT

public:
    HandleClientSocketThread(QTcpSocket* soc);
private:
    QTcpSocket *socket;
    void parseMsgFromClient(QString json);
    void controlDirection(QString device,QString direction);
    void controlCarLight(QString device,QString headLeft,QString headRight,QString roofLigntRed,QString rootLightBlue);
    void controlSpeakers(QString device,QString control);
    void controlRoadLight(QString device,QString statusRoadLight1,QString statusRoadLight2);

//    QSerialPort *serialPort;
protected:
    void run();
signals:
    void done();
    void readMessageSignal(QString);
private slots:
    void handleSlot();
    void tcpSocket_error_slot();



};

#endif // HANDLECLIENTSOCKETTHREAD_H
